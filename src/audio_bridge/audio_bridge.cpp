/*
 * audio_bridge.cpp - Lua 5.4 C module for the navigation radar's audio cues.
 *
 * Port of the validated XAudio2 cue player from the DBXV2 accessibility mod
 * (D:\code\dbxv2 AudioCueService): one engine, one mastering voice, and one
 * source voice per cue. The beacon ping is a retriggered one-shot whose CADENCE
 * is controlled by the Lua caller; this module only applies per-ping pan
 * (output matrix, equal-power), volume, and pitch (SetFrequencyRatio).
 *
 * DELIBERATELY SEPARATE from prism_bridge (screen reader) and mem_bridge
 * (memory reads): this one plays cue sounds ONLY. No accessibility logic here —
 * distance->rate, behind->pitch etc. live in Lua (nav_tracker.lua).
 *
 * Same ABI trick as the other bridges: statically links its own Lua 5.4.4
 * (UE4SS's exact version) and dynamically loads its worker DLL (xaudio2_9.dll,
 * a Windows 10/11 system component — no redistributable needed).
 *
 * Sounds: WAV PCM 16-bit (mono for pannable cues), loaded from <dll dir>\sounds\.
 * The two shipped files come from the XV2 mod: beacon.wav (radar ping) and
 * arrived.wav (objective reached).
 *
 * Lua usage:
 *   local audio = require("audio_bridge")
 *   local ok, err = audio.init()          -- load engine + sounds (once)
 *   audio.ping(pan, volume, pitch)        -- pan -1..1, volume 0..1, pitch 0.5..2
 *   audio.arrival()                       -- interrupt beacon, play arrival cue
 *   audio.stop()                          -- silence everything
 *   audio.ready() -> bool
 */

#include <windows.h>
#include <xaudio2.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

static HINSTANCE g_self = NULL;
static HMODULE g_xaudioDll = NULL;
static IXAudio2 *g_engine = NULL;
static IXAudio2MasteringVoice *g_master = NULL;
static UINT32 g_dstChannels = 2;
static bool g_ready = false;

/* A loaded cue: raw PCM + a dedicated source voice created with its format. */
struct Cue {
    BYTE *pcm;
    UINT32 bytes;
    WAVEFORMATEX fmt;
    IXAudio2SourceVoice *voice;
};
static Cue g_beacon = {0};
static Cue g_arrival = {0};
/* Synthesized continuous tone (a seamless 1 s sine loop; frequency via
 * SetFrequencyRatio, so pitch 0.5..2.0 spans 110..440 Hz on the 220 Hz base).
 * Used for smooth "closing in" feedback (fishing timing) — gentler on the ears
 * than retriggered pings. */
static Cue g_tone = {0};
static bool g_toneOn = false;

/* Category cues for the exploration radar (nav_tracker explore mode), played by NAME
 * via cue(). Loaded non-fatally in do_init — a missing file just makes that name
 * silent (play_cue guards on a NULL voice). Add a WAV + an entry here to grow the set. */
static Cue g_item = {0};
static Cue g_enemy = {0};
struct NamedCue { const char *name; Cue *cue; };
static NamedCue g_named[] = { { "item", &g_item }, { "enemy", &g_enemy }, { NULL, NULL } };

/* ---- WAV loading (PCM only, same constraint as the XV2 player) ------------ */

static bool load_wav(const wchar_t *path, Cue *out) {
    HANDLE h = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, NULL,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return false;
    DWORD size = GetFileSize(h, NULL);
    if (size == INVALID_FILE_SIZE || size < 44) { CloseHandle(h); return false; }
    BYTE *buf = (BYTE *)malloc(size);
    DWORD got = 0;
    if (!buf || !ReadFile(h, buf, size, &got, NULL) || got != size) {
        free(buf); CloseHandle(h); return false;
    }
    CloseHandle(h);

    if (memcmp(buf, "RIFF", 4) != 0 || memcmp(buf + 8, "WAVE", 4) != 0) {
        free(buf); return false;
    }
    /* Walk chunks for "fmt " and "data". */
    WAVEFORMATEX fmt = {0};
    BYTE *data = NULL;
    UINT32 dataLen = 0;
    /* All chunk arithmetic is 64-bit ON PURPOSE. In DWORD math `off + 8 + len` WRAPS, so a
     * chunk declaring a length near 0xFFFFFFFF passed the bounds check below and then drove
     * a ~4 GB out-of-bounds memcpy, and the advance wrapped too (the walk could loop
     * forever). Both operands are 32-bit values, so no 64-bit sum here can overflow. Any
     * malformed or truncated WAV dropped in sounds\ reaches this. */
    uint64_t off = 12;
    while (off + 8 <= (uint64_t)size) {
        const BYTE *id = buf + off;
        DWORD len = *(const DWORD *)(buf + off + 4);
        if (off + 8 + (uint64_t)len > (uint64_t)size) break;
        if (memcmp(id, "fmt ", 4) == 0 && len >= 16) {
            memcpy(&fmt, buf + off + 8, sizeof(WAVEFORMATEX) < len ? sizeof(WAVEFORMATEX) : len);
            fmt.cbSize = 0;
        } else if (memcmp(id, "data", 4) == 0) {
            data = buf + off + 8;
            dataLen = len;
        }
        off += (uint64_t)8 + len + (len & 1);
    }
    if (fmt.wFormatTag != WAVE_FORMAT_PCM || fmt.wBitsPerSample != 16 || !data || dataLen == 0) {
        free(buf); return false;
    }
    /* Release whatever this cue held before: on a retried init load_wav would otherwise
     * orphan the previous buffer. */
    free(out->pcm);
    out->pcm = (BYTE *)malloc(dataLen);
    if (!out->pcm) { out->bytes = 0; free(buf); return false; }
    memcpy(out->pcm, data, dataLen);
    out->bytes = dataLen;
    out->fmt = fmt;
    free(buf);
    return true;
}

/* ---- helpers --------------------------------------------------------------- */

static float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

/* Equal-power stereo pan gains (from the XV2 player): pan -1 (left) .. +1 (right),
 * center = ~0.707 in both channels so perceived loudness is constant. */
static void pan_gains(float pan, float *l, float *r) {
    pan = clampf(pan, -1.0f, 1.0f);
    const float angle = (pan + 1.0f) * 0.5f * 1.5707963f; /* 0..pi/2 */
    *l = cosf(angle);
    *r = sinf(angle);
}

/* Restart a cue's voice from the beginning with the given params. A mono source
 * is panned through the output matrix; other channel counts play as-is. */
static bool play_cue(Cue *c, float pan, float volume, float pitch, bool applyPan) {
    if (!g_ready || !c->voice) return false;
    volume = clampf(volume, 0.0f, 1.0f);
    /* 0.5..2.0 stays inside XAudio2's default max frequency ratio (2.0). */
    pitch = clampf(pitch, 0.5f, 2.0f);

    if (applyPan && c->fmt.nChannels == 1) {
        float l, r;
        pan_gains(pan, &l, &r);
        /* Sized for the largest legal destination, not for 8 (crash audit RANK 9, 2026-07-31).
         * XAudio2 requires DestinationChannels to equal the destination voice's ACTUAL channel
         * count, so the old `float matrix[8]` forced g_dstChannels to be clamped to 8 to fit —
         * and on a >7.1 endpoint (a multi-output interface or a spatial-audio device, i.e.
         * exactly a streaming rig) that mismatch made this call return E_INVALIDARG. Nothing
         * checked the result, so play_cue went on to Submit/Start and reported success while the
         * mod's ONLY spatial cue silently lost its direction, with nothing in the log. */
        float matrix[XAUDIO2_MAX_AUDIO_CHANNELS] = {0};
        if (g_dstChannels >= 1) matrix[0] = l;
        if (g_dstChannels >= 2) matrix[1] = r;
        /* The HRESULT is still not checked, deliberately: this function has no route to a log
         * (hr_err only formats a buffer that do_init returns to Lua, and play_cue answers a plain
         * bool that means "the cue played"). The fix is to remove the CAUSE rather than to report
         * it — with the destination count now honest and the matrix sized for it, the argument
         * mismatch that produced E_INVALIDARG cannot occur. */
        c->voice->SetOutputMatrix(NULL, 1, g_dstChannels, matrix);
    }
    c->voice->SetVolume(volume);
    c->voice->SetFrequencyRatio(pitch);

    c->voice->Stop(0);
    c->voice->FlushSourceBuffers();
    XAUDIO2_BUFFER xb = {0};
    xb.AudioBytes = c->bytes;
    xb.pAudioData = c->pcm;
    xb.Flags = XAUDIO2_END_OF_STREAM;
    if (FAILED(c->voice->SubmitSourceBuffer(&xb))) return false;
    return SUCCEEDED(c->voice->Start(0));
}

/* ---- init / shutdown -------------------------------------------------------- */

typedef HRESULT(__stdcall *XAudio2CreateFn)(IXAudio2 **, UINT32, XAUDIO2_PROCESSOR);

static char g_errbuf[128];

static const char *hr_err(const char *what, HRESULT hr) {
    _snprintf(g_errbuf, sizeof(g_errbuf), "%s (hr=0x%08lX)", what, (unsigned long)hr);
    return g_errbuf;
}

/* Undo a partial (or complete) init: source voices first, then the mastering voice, then the
 * engine, then the DLL — the XAudio2 teardown order. Every handle is NULLed and every PCM
 * buffer freed, so a later init() starts from scratch instead of stacking a second engine and
 * a second mastering voice on top of the first. Safe at any stage: every step is NULL-checked.
 * CoInitializeEx is deliberately NOT balanced here — do_init does not know whether COM was
 * already up on this thread (inside the game it is), and uninitialising someone else's
 * apartment would be worse than the leak. */
static void release_all(void) {
    Cue *cues[] = { &g_beacon, &g_arrival, &g_tone, &g_item, &g_enemy };
    for (int i = 0; i < (int)(sizeof(cues) / sizeof(cues[0])); ++i) {
        if (cues[i]->voice) { cues[i]->voice->Stop(0); cues[i]->voice->DestroyVoice(); }
        free(cues[i]->pcm);
        memset(cues[i], 0, sizeof(Cue));
    }
    g_toneOn = false;
    if (g_master) { g_master->DestroyVoice(); g_master = NULL; }
    if (g_engine) { g_engine->Release(); g_engine = NULL; }
    if (g_xaudioDll) { FreeLibrary(g_xaudioDll); g_xaudioDll = NULL; }
    g_ready = false;
}

/* Every failure return in do_init goes through here, so no error path can leak. */
static const char *init_fail(const char *msg) {
    release_all();
    return msg;
}

static const char *do_init(void) {
    if (g_ready) return NULL;

    /* XAudio2 2.9 enumerates the output device through WASAPI/COM; make sure COM
     * is up on this thread. Inside the game it already is (S_FALSE / mode-change
     * results are fine to ignore). */
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    g_xaudioDll = LoadLibraryW(L"xaudio2_9.dll");
    if (!g_xaudioDll) return init_fail("xaudio2_9.dll not found");
    XAudio2CreateFn create = (XAudio2CreateFn)GetProcAddress(g_xaudioDll, "XAudio2Create");
    if (!create) return init_fail("XAudio2Create export not found");
    HRESULT hr = create(&g_engine, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) return init_fail(hr_err("XAudio2Create failed", hr));
    hr = g_engine->CreateMasteringVoice(&g_master);
    if (FAILED(hr)) return init_fail(hr_err("CreateMasteringVoice failed", hr));

    XAUDIO2_VOICE_DETAILS det;
    g_master->GetVoiceDetails(&det);
    /* The device's TRUE channel count — SetOutputMatrix rejects anything else (see the matrix in
     * play_cue). The old `> 8` clamp existed only to fit a fixed float[8]; that array is now sized
     * XAUDIO2_MAX_AUDIO_CHANNELS, so the clamp is gone. The bound that remains is the array's own,
     * and it cannot legally trip — XAudio2 will not report more channels than it supports. */
    g_dstChannels = det.InputChannels;
    if (g_dstChannels > XAUDIO2_MAX_AUDIO_CHANNELS) g_dstChannels = XAUDIO2_MAX_AUDIO_CHANNELS;

    /* Sounds live in <this dll's folder>\sounds\ (deployed with the mod). */
    wchar_t dir[MAX_PATH];
    GetModuleFileNameW((HMODULE)g_self, dir, MAX_PATH);
    wchar_t *slash = wcsrchr(dir, L'\\');
    if (slash) *(slash + 1) = 0;

    /* _snwprintf_s(_TRUNCATE), not _snwprintf: the plain form does NOT NUL-terminate when
     * the result fills the buffer, and `path` goes straight to CreateFileW. A long enough
     * install path would hand the API an unterminated buffer. Truncation now yields a
     * terminated (and simply nonexistent) path, i.e. the normal "missing file" outcome. */
    wchar_t path[MAX_PATH];
    _snwprintf_s(path, MAX_PATH, _TRUNCATE, L"%ssounds\\beacon.wav", dir);
    if (!load_wav(path, &g_beacon)) return init_fail("sounds\\beacon.wav missing or not PCM16");
    _snwprintf_s(path, MAX_PATH, _TRUNCATE, L"%ssounds\\arrived.wav", dir);
    if (!load_wav(path, &g_arrival)) return init_fail("sounds\\arrived.wav missing or not PCM16");

    if (FAILED(g_engine->CreateSourceVoice(&g_beacon.voice, &g_beacon.fmt)))
        return init_fail("CreateSourceVoice(beacon) failed");
    if (FAILED(g_engine->CreateSourceVoice(&g_arrival.voice, &g_arrival.fmt)))
        return init_fail("CreateSourceVoice(arrival) failed");

    /* Named category cues for explore mode (optional — a missing/invalid file stays
     * silent instead of failing init, so the core radar always comes up). */
    _snwprintf_s(path, MAX_PATH, _TRUNCATE, L"%ssounds\\item.wav", dir);
    if (load_wav(path, &g_item))
        g_engine->CreateSourceVoice(&g_item.voice, &g_item.fmt);
    _snwprintf_s(path, MAX_PATH, _TRUNCATE, L"%ssounds\\enemy.wav", dir);
    if (load_wav(path, &g_enemy))
        g_engine->CreateSourceVoice(&g_enemy.voice, &g_enemy.fmt);

    /* Generate the sine loop: 220 Hz over exactly 1 s = 220 whole cycles, so the
     * loop point is click-free. Soft amplitude; the caller shapes loudness. */
    {
        const UINT32 rate = 44100;
        const float freq = 220.0f;
        const float amp = 0.35f;
        g_tone.fmt.wFormatTag = WAVE_FORMAT_PCM;
        g_tone.fmt.nChannels = 1;
        g_tone.fmt.nSamplesPerSec = rate;
        g_tone.fmt.wBitsPerSample = 16;
        g_tone.fmt.nBlockAlign = 2;
        g_tone.fmt.nAvgBytesPerSec = rate * 2;
        g_tone.bytes = rate * 2;
        g_tone.pcm = (BYTE *)malloc(g_tone.bytes);
        if (!g_tone.pcm) return init_fail("tone buffer alloc failed");
        int16_t *p = (int16_t *)g_tone.pcm;
        for (UINT32 i = 0; i < rate; i++)
            p[i] = (int16_t)(sinf(6.2831853f * freq * (float)i / (float)rate) * amp * 32767.0f);
        if (FAILED(g_engine->CreateSourceVoice(&g_tone.voice, &g_tone.fmt)))
            return init_fail("CreateSourceVoice(tone) failed");
    }

    g_ready = true;
    return NULL;
}

/* ---- Lua API ---------------------------------------------------------------- */

static int l_init(lua_State *L) {
    const char *err = do_init();
    if (err) {
        lua_pushboolean(L, 0);
        lua_pushstring(L, err);
        return 2;
    }
    lua_pushboolean(L, 1);
    return 1;
}

/* ping(pan, volume, pitch) — one radar ping. Caller controls the cadence. */
static int l_ping(lua_State *L) {
    float pan = (float)luaL_optnumber(L, 1, 0.0);
    float vol = (float)luaL_optnumber(L, 2, 1.0);
    float pitch = (float)luaL_optnumber(L, 3, 1.0);
    lua_pushboolean(L, play_cue(&g_beacon, pan, vol, pitch, true));
    return 1;
}

/* cue(name, pan, volume, pitch) — play a named category cue (explore radar). Same
 * pan/volume/pitch mapping as ping(); unknown or unloaded names are a silent no-op. */
static int l_cue(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    float pan = (float)luaL_optnumber(L, 2, 0.0);
    float vol = (float)luaL_optnumber(L, 3, 1.0);
    float pitch = (float)luaL_optnumber(L, 4, 1.0);
    for (NamedCue *n = g_named; n->name; ++n) {
        if (strcmp(n->name, name) == 0) {
            lua_pushboolean(L, play_cue(n->cue, pan, vol, pitch, true));
            return 1;
        }
    }
    lua_pushboolean(L, 0);
    return 1;
}

/* arrival() — cut the beacon and play the arrival cue centered. */
static int l_arrival(lua_State *L) {
    if (g_ready && g_beacon.voice) {
        g_beacon.voice->Stop(0);
        g_beacon.voice->FlushSourceBuffers();
    }
    lua_pushboolean(L, play_cue(&g_arrival, 0.0f, 1.0f, 1.0f, true));
    return 1;
}

/* tone(volume, pitch) — start (if needed) and shape the continuous sine loop.
 * Call repeatedly with new values; it updates in place, no retrigger. */
static int l_tone(lua_State *L) {
    float vol = clampf((float)luaL_optnumber(L, 1, 0.3), 0.0f, 1.0f);
    float pitch = clampf((float)luaL_optnumber(L, 2, 1.0), 0.5f, 2.0f);
    if (!g_ready || !g_tone.voice) { lua_pushboolean(L, 0); return 1; }
    g_tone.voice->SetVolume(vol);
    g_tone.voice->SetFrequencyRatio(pitch);
    if (!g_toneOn) {
        XAUDIO2_BUFFER xb = {0};
        xb.AudioBytes = g_tone.bytes;
        xb.pAudioData = g_tone.pcm;
        xb.LoopCount = XAUDIO2_LOOP_INFINITE;
        if (FAILED(g_tone.voice->SubmitSourceBuffer(&xb)) || FAILED(g_tone.voice->Start(0))) {
            lua_pushboolean(L, 0);
            return 1;
        }
        g_toneOn = true;
    }
    lua_pushboolean(L, 1);
    return 1;
}

static int l_tone_stop(lua_State *L) {
    (void)L;
    if (g_ready && g_toneOn && g_tone.voice) {
        g_tone.voice->Stop(0);
        g_tone.voice->FlushSourceBuffers();
        g_toneOn = false;
    }
    return 0;
}

static int l_stop(lua_State *L) {
    (void)L;
    if (g_ready) {
        if (g_beacon.voice) { g_beacon.voice->Stop(0); g_beacon.voice->FlushSourceBuffers(); }
        if (g_arrival.voice) { g_arrival.voice->Stop(0); g_arrival.voice->FlushSourceBuffers(); }
        for (NamedCue *n = g_named; n->name; ++n) {
            if (n->cue->voice) { n->cue->voice->Stop(0); n->cue->voice->FlushSourceBuffers(); }
        }
        if (g_toneOn && g_tone.voice) {
            g_tone.voice->Stop(0);
            g_tone.voice->FlushSourceBuffers();
            g_toneOn = false;
        }
    }
    return 0;
}

static int l_ready(lua_State *L) {
    lua_pushboolean(L, g_ready);
    return 1;
}

static const luaL_Reg audio_funcs[] = {
    {"init", l_init},
    {"ping", l_ping},
    {"cue", l_cue},
    {"arrival", l_arrival},
    {"tone", l_tone},
    {"tone_stop", l_tone_stop},
    {"stop", l_stop},
    {"ready", l_ready},
    {NULL, NULL}
};

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)lpvReserved;
    if (fdwReason == DLL_PROCESS_ATTACH) g_self = hinstDLL;
    return TRUE;
}

extern "C" __declspec(dllexport) int luaopen_audio_bridge(lua_State *L) {
    luaL_newlib(L, audio_funcs);
    return 1;
}
