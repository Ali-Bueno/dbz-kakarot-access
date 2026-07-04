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
 * Lua usage:
 *   local ib = require("input_bridge")
 *   ib.install()                 -- hook XInput (idempotent); returns true/false
 *   local b, lt, rt, lx, ly = ib.poll()  -- wButtons, triggers 0..255, L-stick -1..1
 *   ib.block(true|false)         -- hide/show the pad from the GAME
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
static volatile LONG    g_block = 0;             /* 1 = hide the pad from the game    */
static XI_STATE         g_last;                  /* true latest state (user 0)        */
static volatile LONG    g_haveLast = 0;
static int              g_hooked = 0;

/* Our replacement for XInputGetState: read the truth, cache it, optionally blank it. */
static DWORD WINAPI hookGetState(DWORD idx, XI_STATE *pState) {
    if (!g_realGetState) return (DWORD)-1 /*ERROR_DEVICE_NOT_CONNECTED path*/;
    DWORD r = g_realGetState(idx, pState);
    if (r == 0 /*ERROR_SUCCESS*/ && pState) {
        if (idx == 0) { g_last = *pState; g_haveLast = 1; }
        if (InterlockedCompareExchange(&g_block, 0, 0)) {
            /* Hand the GAME a neutral pad; keep the packet number moving so the
             * game doesn't treat it as a stale/disconnected read. */
            unsigned long pkt = pState->dwPacketNumber + 1;
            memset(pState, 0, sizeof(*pState));
            pState->dwPacketNumber = pkt;
        }
    }
    return r;
}

/* Walk the import descriptors of `mod`, find the entry importing `dll`!`func`,
 * overwrite its thunk with `repl`, and return the original pointer (or NULL). */
static void *iat_hook(HMODULE mod, const char *dll, const char *func, void *repl) {
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

/* poll([userIndex=0]) -> wButtons, leftTrigger, rightTrigger, lx, ly, rx, ry
 * Buttons is the raw XINPUT bitmask; triggers are 0..255; sticks are -1..1
 * (raw shorts /32767). Returns nil if the pad can't be read / isn't connected.
 * When hooked, reads the cached true state (works even while blocking); otherwise
 * calls XInputGetState directly. */
static int l_poll(lua_State *L) {
    int idx = (int)luaL_optinteger(L, 1, 0);
    XI_STATE st;
    int have = 0;
    if (g_hooked && idx == 0 && g_haveLast) {
        st = g_last; have = 1;
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

/* block(on) : while on, the game receives a neutral pad. No-op if not hooked. */
static int l_block(lua_State *L) {
    int on = lua_toboolean(L, 1);
    InterlockedExchange(&g_block, on ? 1 : 0);
    lua_pushboolean(L, g_hooked && on);
    return 1;
}

/* is_hooked() -> bool */
static int l_is_hooked(lua_State *L) {
    lua_pushboolean(L, g_hooked);
    return 1;
}

static const luaL_Reg input_funcs[] = {
    {"install",   l_install},
    {"poll",      l_poll},
    {"block",     l_block},
    {"is_hooked", l_is_hooked},
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
