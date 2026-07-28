/*
 * input_bridge.c - Lua 5.4 C module that lets the accessibility mod read the
 * gamepad AND temporarily hide it from the game, so the hold-R2 radar target
 * picker can be navigated without the game reacting to the same buttons.
 *
 * HOW: the game reads the pad through XINPUT1_3.dll!XInputGetState (verified in
 * the exe import table). We install an IAT hook on that import — replacing the
 * thunk in the main module's import table with our wrapper. The wrapper calls the
 * ORIGINAL XInputGetState, stashes the true latest state for Lua to read, and —
 * while "block" is set — zeroes the pad struct handed back to the GAME (buttons,
 * triggers, thumbsticks). So our menu sees the real stick/buttons while the game
 * sees a neutral controller and triggers nothing.
 *
 * DELIBERATELY SEPARATE from the other bridges (prism = screen reader, mem =
 * memory reads, audio = cues). This one only touches input. No mod logic here;
 * all menu behaviour stays in Lua (radar_menu.lua).
 *
 * Same ABI trick as the other bridges: statically links its own Lua 5.4.4 (UE4SS's
 * exact version), so `require("input_bridge")` gets UE4SS's own lua_State.
 *
 * We never link xinput: the original function pointer is taken straight from the
 * IAT slot before we overwrite it. If the hook can't be installed (import not
 * found), poll() still works by falling back to a LoadLibrary'd XInputGetState,
 * and block() is simply a no-op (read-only mode).
 *
 * The KEYBOARD gets the same treatment through a second hook (user32!PeekMessageW); see
 * the block comment above hookPeekMessageW for the evidence behind that choice.
 *
 * Lua usage:
 *   local ib = require("input_bridge")
 *   ib.install()                 -- hook XInput + the message pump (idempotent)
 *   local b, lt, rt, lx, ly = ib.poll()  -- wButtons, triggers 0..255, L-stick -1..1
 *   ib.block(true|false)         -- hide/show the pad from the GAME
 *   ib.kb_block(ms)              -- hide the KEYBOARD for ms (a lease: renew every tick)
 */

#include <windows.h>
#include <tlhelp32.h>
#include <string.h>
#include <stdint.h>

#include "lua.h"
#include "lauxlib.h"

/* Minimal XInput types (avoid depending on the SDK header). */
typedef struct {
    unsigned short wButtons;
    unsigned char  bLeftTrigger;
    unsigned char  bRightTrigger;
    short sThumbLX, sThumbLY, sThumbRX, sThumbRY;
} XI_GAMEPAD;
typedef struct {
    unsigned long dwPacketNumber;
    XI_GAMEPAD    Gamepad;
} XI_STATE;

typedef DWORD (WINAPI *XInputGetState_t)(DWORD, XI_STATE *);

static XInputGetState_t g_realGetState = NULL;   /* original, from the IAT/LoadLibrary */
static XI_STATE         g_last;                  /* true latest state (user 0)        */
static volatile LONG    g_haveLast = 0;
/* g_last is WRITTEN on the game's input-pump thread (inside hookGetState) and READ on the
 * game thread (l_poll), so a plain 16-byte struct copy can tear — half of one pad frame and
 * half of the next. A SEQLOCK is the cheap correct fix: the writer sits inside the game's own
 * input hook and must never block, and a seqlock writer never waits (two counter bumps around
 * the copy; InterlockedIncrement is a full barrier on x64). The reader retries while the
 * counter is odd (write in flight) or changed across its copy. */
static volatile LONG    g_lastSeq = 0;           /* even = stable, odd = write in progress */
/* Bounded so a reader can never spin on a wedged writer. The pump updates the pad once per
 * frame while poll() runs every 20 ms (pad_poll.lua TICK_MS), so a single retry already
 * covers real contention; giving up simply reports no snapshot for one tick — the same
 * outcome the caller already handles for an unplugged pad. */
#define PAD_SNAPSHOT_TRIES 8
static int              g_hooked = 0;
/* Left-stick INJECTION (map-cursor auto-move). g_injTTL = game frames the injected value
 * stays live; it counts DOWN every poll, so if Lua stops refreshing (map closed, error)
 * the stick auto-releases in ~a frame or two and can NEVER stay stuck. Buttons/triggers/
 * right-stick pass through untouched, so the player can still press X to confirm. */
static volatile LONG    g_injTTL = 0;
static volatile LONG    g_injLX = 0;             /* injected left-stick X (-32767..32767) */
static volatile LONG    g_injLY = 0;

/* ---- PAD blocking: a LEASE, like the keyboard's (see g_kbUntil) -----------------------
 * block(true) used to be a plain latch, so a Lua error unwinding — or a mod reload —
 * between block(true) and block(false) left the game permanently deaf to the pad, with no
 * way out but killing the process. It now carries a wall-clock DEADLINE which poll() renews,
 * and poll() is exactly what both pad menus call every tick while they are open
 * (radar_menu / config_menu step through Input.read()). So the Lua API is unchanged: nothing
 * stops working, but nothing keeps the pad blocked either once the mod stops stepping.
 * Renewal only EXTENDS a LIVE lease, never resurrects an expired one — poll() also runs every
 * tick with no menu open, so renewing unconditionally would make a stale block immortal. */
#define PAD_BLOCK_LEASE_MS 1000   /* the pad step runs every 20 ms (pad_poll.lua TICK_MS), so
                                     50 ticks of slack: long enough to ride out a frame hitch,
                                     short enough to self-heal within a second. */
static volatile LONG64  g_blockUntil = 0;        /* GetTickCount64 deadline; 0 = not blocking */

static int pad_blocking(void) {
    LONG64 until = InterlockedCompareExchange64((volatile LONG64 *)&g_blockUntil, 0, 0);
    return until != 0 && (LONG64)GetTickCount64() < until;
}

static void pad_block_renew(void) {
    if (pad_blocking())
        InterlockedExchange64((volatile LONG64 *)&g_blockUntil,
                              (LONG64)GetTickCount64() + PAD_BLOCK_LEASE_MS);
}

/* ---- KEYBOARD blocking ---------------------------------------------------------------
 * Same goal as block() above, for the keyboard: while the radar menu is open, our Lua must
 * still see the keys while the GAME sees nothing (so Page Down doesn't also dismount you).
 *
 * WHERE we intercept, and why it is safe (investigated 2026-07-25 against this exe and the
 * RE-UE4SS v3.0.1 source, not assumed):
 *   - This exe pumps messages with user32!PeekMessageW (6 call sites; GetMessageW is not
 *     even imported), and it registers RawInput for the MOUSE ONLY (the single
 *     RegisterRawInputDevices call passes usUsagePage=1, usUsage=2). It imports no
 *     DirectInput. So every keystroke reaches the game as a POSTED WM_KEY... / WM_CHAR
 *     message, and draining those from the pump hides the keyboard completely.
 *   - UE4SS does NOT read those messages: Input/Handler.cpp polls GetAsyncKeyState on its
 *     own UE4SS-UpdateThread. Draining the game's queue therefore cannot break our own
 *     keybinds — which matters more than it sounds, since the key that CLOSES the menu is
 *     one of them. (This is also why hooking GetAsyncKeyState itself would be a disaster:
 *     it is exactly what UE4SS polls, so it would silence the mod process-wide.)
 *
 * A LEASE, NOT A LATCH. If the mod dies or a Lua error unwinds while the block is set, a
 * plain flag would leave the game permanently deaf to the keyboard — including its own
 * pause and Escape — with no way out but killing the process. So the block carries a
 * wall-clock DEADLINE that Lua must keep renewing (radar_menu renews it every 20 ms tick);
 * anything that stops the mod releases the keyboard by itself within a few hundred ms.
 * Set from Lua on the game thread, read on the pump thread -> interlocked. */
typedef BOOL (WINAPI *PeekMessageW_t)(LPMSG, HWND, UINT, UINT, UINT);
static PeekMessageW_t   g_realPeek = NULL;
static volatile LONG64  g_kbUntil = 0;           /* GetTickCount64 deadline; 0 = not blocking */
static int              g_kbHooked = 0;

static int kb_blocking(void) {
    LONG64 until = InterlockedCompareExchange64((volatile LONG64 *)&g_kbUntil, 0, 0);
    return until != 0 && (LONG64)GetTickCount64() < until;
}

/* WM_KEYDOWN..WM_UNICHAR — the whole keyboard block, including the WM_CHAR messages
 * TranslateMessage synthesises (swallowing only the key-downs would still let text
 * through). Written as literals: WM_KEYLAST differs between SDK versions. */
static int is_key_msg(UINT m) { return m >= 0x0100 && m <= 0x0109; }

/* Our replacement pump entry. Swallows keyboard messages while the lease is live and
 * returns the next NON-keyboard message instead, so the game's frame still gets its
 * paint/input/timer traffic. */
static BOOL WINAPI hookPeekMessageW(LPMSG lpMsg, HWND hWnd, UINT minMsg, UINT maxMsg, UINT rm) {
    int guard;
    if (!g_realPeek) return FALSE;
    /* Bounded: a queue that somehow only ever yields keys must not spin the game thread. */
    for (guard = 0; guard < 256; ++guard) {
        BOOL got = g_realPeek(lpMsg, hWnd, minMsg, maxMsg, rm);
        if (!got || !lpMsg) return got;
        if (!is_key_msg(lpMsg->message) || !kb_blocking()) return got;
        /* Alt+F4 always reaches the game: no accessibility feature may take away the
         * player's ability to quit. (WM_SYSCOMMAND / WM_CLOSE / WM_ACTIVATE are not
         * keyboard messages, so they were never candidates for swallowing.) */
        if (lpMsg->wParam == VK_F4) return got;
        /* PM_NOREMOVE peeks (this exe has three) leave the message queued, and a filter
         * that only hides it would let the whole burst hit the game the instant we
         * unblock. Consume it for real, then look for the next message. */
        if (!(rm & PM_REMOVE)) {
            MSG tmp;
            g_realPeek(&tmp, lpMsg->hwnd, lpMsg->message, lpMsg->message, PM_REMOVE);
        }
    }
    return FALSE;
}

/* Our replacement for XInputGetState: read the truth, cache it, optionally blank it. */
static DWORD WINAPI hookGetState(DWORD idx, XI_STATE *pState) {
    if (!g_realGetState) return (DWORD)-1 /*ERROR_DEVICE_NOT_CONNECTED path*/;
    DWORD r = g_realGetState(idx, pState);
    if (r == 0 /*ERROR_SUCCESS*/ && pState) {
        if (idx == 0) {
            InterlockedIncrement(&g_lastSeq);      /* -> odd: write in progress */
            g_last = *pState;
            InterlockedIncrement(&g_lastSeq);      /* -> even: stable again */
            g_haveLast = 1;
        }
        if (pad_blocking()) {
            /* Hand the GAME a neutral pad; keep the packet number moving so the
             * game doesn't treat it as a stale/disconnected read. */
            unsigned long pkt = pState->dwPacketNumber + 1;
            memset(pState, 0, sizeof(*pState));
            pState->dwPacketNumber = pkt;
        } else if (idx == 0 && g_injTTL > 0) {
            /* Drive ONLY the left stick to the injected value; everything else is the
             * real pad (so X/confirm and the d-pad still reach us and the game). */
            pState->Gamepad.sThumbLX = (short)g_injLX;
            pState->Gamepad.sThumbLY = (short)g_injLY;
            pState->dwPacketNumber++;
            InterlockedDecrement(&g_injTTL);
        }
    }
    return r;
}

/* Walk the import descriptors of `mod`, find the entry importing `dll`!`func`,
 * overwrite its thunk with `repl`, and return the original pointer (or NULL).
 * Call it through iat_hook() below, never directly — see the guard there. */
static void *iat_hook_unguarded(HMODULE mod, const char *dll, const char *func, void *repl) {
    if (!mod) return NULL;
    unsigned char *base = (unsigned char *)mod;
    IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)base;
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return NULL;
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return NULL;
    IMAGE_DATA_DIRECTORY imp = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (!imp.VirtualAddress) return NULL;
    IMAGE_IMPORT_DESCRIPTOR *desc = (IMAGE_IMPORT_DESCRIPTOR *)(base + imp.VirtualAddress);

    void *original = NULL;
    /* Resolve the target address once, to match by pointer as well as by name. */
    HMODULE hdll = GetModuleHandleA(dll);
    if (!hdll) hdll = LoadLibraryA(dll);
    FARPROC target = hdll ? GetProcAddress(hdll, func) : NULL;

    for (; desc->Name; ++desc) {
        const char *name = (const char *)(base + desc->Name);
        if (_stricmp(name, dll) != 0) continue;
        IMAGE_THUNK_DATA *oft = desc->OriginalFirstThunk
            ? (IMAGE_THUNK_DATA *)(base + desc->OriginalFirstThunk) : NULL;
        IMAGE_THUNK_DATA *ft = (IMAGE_THUNK_DATA *)(base + desc->FirstThunk);
        for (; ft->u1.Function; ++ft, oft ? ++oft : oft) {
            int match = 0;
            if (target && (FARPROC)ft->u1.Function == target) {
                match = 1;
            } else if (oft && !(oft->u1.Ordinal & IMAGE_ORDINAL_FLAG)) {
                IMAGE_IMPORT_BY_NAME *ibn =
                    (IMAGE_IMPORT_BY_NAME *)(base + oft->u1.AddressOfData);
                if (_stricmp((const char *)ibn->Name, func) == 0) match = 1;
            }
            if (!match) continue;
            original = (void *)(uintptr_t)ft->u1.Function;
            DWORD old;
            if (VirtualProtect(&ft->u1.Function, sizeof(void *), PAGE_READWRITE, &old)) {
                ft->u1.Function = (uintptr_t)repl;
                VirtualProtect(&ft->u1.Function, sizeof(void *), old, &old);
            }
            return original;
        }
    }
    return NULL;
}

/* SEH guard around that walk. It parses the PE headers and import tables of modules we do
 * not control, following RVAs it cannot validate, and iat_hook_any() feeds it a Toolhelp
 * SNAPSHOT that can go stale — a module unloaded between the snapshot and the walk leaves a
 * base address that is no longer mapped. A fault here would kill the game at mod init, so it
 * degrades to "this module does not import it" instead. */
static void *iat_hook(HMODULE mod, const char *dll, const char *func, void *repl) {
    __try {
        return iat_hook_unguarded(mod, dll, func, repl);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return NULL;
    }
}

/* Try the IAT hook on the exe first; if the import lives in some other loaded module
 * (a UE build that pulls XInput in via a DLL), scan every module and hook the first
 * that imports it. Returns the original pointer, or NULL if nothing imports it. */
static void *iat_hook_any(const char *dll, const char *func, void *repl) {
    void *orig = iat_hook(GetModuleHandleA(NULL), dll, func, repl);
    if (orig) return orig;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
    if (snap == INVALID_HANDLE_VALUE) return NULL;
    MODULEENTRY32 me;
    me.dwSize = sizeof(me);
    if (Module32First(snap, &me)) {
        do {
            orig = iat_hook((HMODULE)me.hModule, dll, func, repl);
            if (orig) break;
        } while (Module32Next(snap, &me));
    }
    CloseHandle(snap);
    return orig;
}

/* install() -> bool : idempotent. Installs the IAT hook (exe, then any module); falls
 * back to a plain LoadLibrary'd XInputGetState (read-only, no blocking) if the
 * import can't be patched. */
static int l_install(lua_State *L) {
    /* A fresh mod session always starts with the pad free: install() is called once from
     * Input.init(), so a block left over from a previous session (reload after a Lua error)
     * is released here rather than waiting for its lease to lapse. */
    InterlockedExchange64((volatile LONG64 *)&g_blockUntil, 0);
    /* The keyboard hook is independent of the XInput one (a keyboard player may have no
     * pad at all), so it is installed on every call until it takes, and never removed.
     * PIN our own module first: the IAT slot points into this DLL, so unloading it while
     * the game's pump is live would be an instant crash. */
    if (!g_kbHooked) {
        /* C89 build (see build.ps1): every declaration at the top of its block. */
        HMODULE self = NULL;
        void *pk = NULL;
        GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN,
                           (LPCWSTR)(void *)&hookPeekMessageW, &self);
        pk = iat_hook_any("USER32.dll", "PeekMessageW", (void *)hookPeekMessageW);
        if (pk) { g_realPeek = (PeekMessageW_t)pk; g_kbHooked = 1; }
    }
    if (g_hooked) { lua_pushboolean(L, 1); return 1; }
    void *orig = iat_hook_any("XINPUT1_3.dll", "XInputGetState", (void *)hookGetState);
    if (orig) {
        g_realGetState = (XInputGetState_t)orig;
        g_hooked = 1;
        lua_pushboolean(L, 1);
        return 1;
    }
    /* Read-only fallback: resolve the real function so poll() still works. */
    if (!g_realGetState) {
        HMODULE h = GetModuleHandleA("XINPUT1_3.dll");
        if (!h) h = LoadLibraryA("XINPUT1_3.dll");
        if (h) g_realGetState = (XInputGetState_t)GetProcAddress(h, "XInputGetState");
    }
    lua_pushboolean(L, 0);   /* not hooked: block() will be a no-op */
    return 1;
}

/* Seqlock reader for g_last (see the note above g_lastSeq). Returns 1 on a torn-free copy. */
static int pad_snapshot(XI_STATE *out) {
    int i;
    if (!InterlockedCompareExchange(&g_haveLast, 0, 0)) return 0;
    for (i = 0; i < PAD_SNAPSHOT_TRIES; ++i) {
        LONG s = InterlockedCompareExchange(&g_lastSeq, 0, 0);
        if (s & 1) continue;                       /* writer mid-update */
        *out = g_last;
        if (InterlockedCompareExchange(&g_lastSeq, 0, 0) == s) return 1;
    }
    return 0;
}

/* poll([userIndex=0]) -> wButtons, leftTrigger, rightTrigger, lx, ly, rx, ry
 * Buttons is the raw XINPUT bitmask; triggers are 0..255; sticks are -1..1
 * (raw shorts /32767). Returns nil if the pad can't be read / isn't connected.
 * When hooked, reads the cached true state (works even while blocking); otherwise
 * calls XInputGetState directly. */
static int l_poll(lua_State *L) {
    int idx = (int)luaL_optinteger(L, 1, 0);
    XI_STATE st;
    int have = 0;
    /* Renew the pad-block lease: the Lua menus poll every tick while they hold the block,
     * so this is what keeps it alive — and stopping is what releases it. */
    pad_block_renew();
    if (g_hooked && idx == 0 && pad_snapshot(&st)) {
        have = 1;
    } else if (g_realGetState) {
        if (g_realGetState((DWORD)idx, &st) == 0) have = 1;
    }
    if (!have) { lua_pushnil(L); return 1; }
    lua_pushinteger(L, (lua_Integer)st.Gamepad.wButtons);
    lua_pushinteger(L, (lua_Integer)st.Gamepad.bLeftTrigger);
    lua_pushinteger(L, (lua_Integer)st.Gamepad.bRightTrigger);
    lua_pushnumber(L, (lua_Number)st.Gamepad.sThumbLX / 32767.0);
    lua_pushnumber(L, (lua_Number)st.Gamepad.sThumbLY / 32767.0);
    lua_pushnumber(L, (lua_Number)st.Gamepad.sThumbRX / 32767.0);
    lua_pushnumber(L, (lua_Number)st.Gamepad.sThumbRY / 32767.0);
    return 7;
}

/* block(on) : while on, the game receives a neutral pad. No-op if not hooked. The block is
 * a lease that poll() renews — see the note above g_blockUntil. */
static int l_block(lua_State *L) {
    int on = lua_toboolean(L, 1);
    InterlockedExchange64((volatile LONG64 *)&g_blockUntil,
                          on ? (LONG64)GetTickCount64() + PAD_BLOCK_LEASE_MS : 0);
    lua_pushboolean(L, g_hooked && on);
    return 1;
}

/* inject(lx, ly) : drive the game's LEFT stick to (lx,ly) in -1..1 for the next few
 * frames (auto-releases if not refreshed). No-op if the hook isn't installed. Returns
 * whether injection can actually take effect (i.e. we're hooked). */
static int l_inject(lua_State *L) {
    double lx = luaL_checknumber(L, 1);
    double ly = luaL_checknumber(L, 2);
    if (lx < -1.0) lx = -1.0; else if (lx > 1.0) lx = 1.0;
    if (ly < -1.0) ly = -1.0; else if (ly > 1.0) ly = 1.0;
    InterlockedExchange(&g_injLX, (LONG)(lx * 32767.0));
    InterlockedExchange(&g_injLY, (LONG)(ly * 32767.0));
    InterlockedExchange(&g_injTTL, 12);   /* ~12 frames; the Lua driver refreshes each tick */
    lua_pushboolean(L, g_hooked);
    return 1;
}

/* inject_off() : release the injected stick immediately. */
static int l_inject_off(lua_State *L) {
    (void)L;
    InterlockedExchange(&g_injTTL, 0);
    return 0;
}

/* kb_block(ms) : hide the KEYBOARD from the game for the next `ms` milliseconds. This is a
 * LEASE — call it again every tick to hold the block; ms <= 0 releases it at once. See the
 * note above hookPeekMessageW for why it expires instead of latching. Returns whether the
 * block can actually take effect (i.e. the pump is hooked). */
static int l_kb_block(lua_State *L) {
    lua_Integer ms = luaL_optinteger(L, 1, 0);
    LONG64 until = (ms > 0) ? (LONG64)GetTickCount64() + (LONG64)ms : 0;
    InterlockedExchange64((volatile LONG64 *)&g_kbUntil, until);
    lua_pushboolean(L, g_kbHooked && ms > 0);
    return 1;
}

/* is_hooked() -> bool  (the pad hook) */
static int l_is_hooked(lua_State *L) {
    lua_pushboolean(L, g_hooked);
    return 1;
}

/* kb_is_hooked() -> bool  (the keyboard/pump hook) */
static int l_kb_is_hooked(lua_State *L) {
    lua_pushboolean(L, g_kbHooked);
    return 1;
}

static const luaL_Reg input_funcs[] = {
    {"install",      l_install},
    {"poll",         l_poll},
    {"block",        l_block},
    {"inject",       l_inject},
    {"inject_off",   l_inject_off},
    {"is_hooked",    l_is_hooked},
    {"kb_block",     l_kb_block},
    {"kb_is_hooked", l_kb_is_hooked},
    {NULL, NULL}
};

BOOL WINAPI DllMain(HINSTANCE h, DWORD reason, LPVOID reserved) {
    (void)h; (void)reserved;
    if (reason == DLL_PROCESS_ATTACH) { memset(&g_last, 0, sizeof(g_last)); }
    return TRUE;
}

__declspec(dllexport) int luaopen_input_bridge(lua_State *L) {
    luaL_newlib(L, input_funcs);
    return 1;
}
