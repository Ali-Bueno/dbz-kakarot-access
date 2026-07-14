/*
 * mem_bridge.c - Lua 5.4 C module exposing guarded in-process memory reads to
 * UE4SS Lua. This is the NATIVE HELPER for the accessibility mod's two Lua dead
 * ends (battle-pause selection, field-menu section): their selected index is a
 * non-UPROPERTY C++ member, invisible to UE reflection. Lua gets the object's
 * address via `obj:GetAddress()`; this module reads an int at address+offset.
 *
 * DELIBERATELY SEPARATE from prism_bridge.dll: that one is the screen reader ONLY.
 * This one holds NO accessibility logic — it only moves bytes. All the diff-gating,
 * i18n and menus.md behaviour stays in Lua. Offsets live in a Lua data file.
 *
 * Same ABI trick as prism_bridge: STATICALLY links its own Lua 5.4.4 (UE4SS's exact
 * version, so the lua_State ABI matches). UE4SS does `require("mem_bridge")` ->
 * luaopen_mem_bridge with UE4SS's own lua_State.
 *
 * SAFETY: every dereference is wrapped in Structured Exception Handling. A bad
 * address returns nil instead of faulting — reads can never hard-crash the game.
 *
 * Lua usage:
 *   local mem = require("mem_bridge")
 *   local sel = mem.read_i32(pause_addr, 0x4a0)      -- int32 at addr+off (off optional)
 *   local p   = mem.read_ptr(obj_addr, 0)            -- 64-bit pointer value
 *   local b   = mem.read_bytes(obj_addr, 0x438, 200) -- raw bytes (string) for diffing
 *   local base= mem.module_base()                    -- main exe base VA
 *   mem.write_float(curs_addr, 0x94, 512.0)          -- write; returns true/false (guarded)
 *   mem.write_i32(obj_addr, 0x4e4, 2)
 * Every read returns nil on a faulting/invalid address; every write returns false on one.
 *
 * WRITES are opt-in and equally SEH-guarded (a bad address is a no-op returning false,
 * never a crash). Used ONLY to snap a game cursor onto a chosen target (world-map fast
 * travel) by writing its widget RenderTransform.Translation — the same member the game
 * reads back as the cursor's own source (see native_offsets.lua). No mod logic here.
 */

#include <windows.h>
#include <string.h>
#include <stdint.h>

#include "lua.h"
#include "lauxlib.h"

static HMODULE g_self = NULL;

/* Copy n bytes from src, guarded by SEH. Returns 1 on success, 0 on access fault. */
static int safe_copy(void *dst, const void *src, size_t n) {
    __try {
        memcpy(dst, src, n);
        return 1;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return 0;
    }
}

/* Store n bytes into dst, guarded by SEH. Returns 1 on success, 0 on access fault.
 * Same guard as safe_copy but the destination is game memory, so a faulting address
 * is a no-op instead of an AV. (A valid-but-wrong address still writes — callers must
 * only pass offsets validated against a live UObject.) */
static int safe_store(void *dst, const void *src, size_t n) {
    __try {
        memcpy(dst, src, n);
        return 1;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return 0;
    }
}

/* base address = arg1 (integer). optional arg2 = byte offset (integer, default 0). */
static uintptr_t addr_of(lua_State *L) {
    uintptr_t base = (uintptr_t)(lua_Integer)luaL_checkinteger(L, 1);
    lua_Integer off = luaL_optinteger(L, 2, 0);
    return base + (uintptr_t)off;
}

#define READ_FN(NAME, CTYPE, PUSH)                                   \
    static int NAME(lua_State *L) {                                  \
        uintptr_t p = addr_of(L);                                    \
        CTYPE v;                                                     \
        if (p == 0 || !safe_copy(&v, (const void *)p, sizeof(v))) {  \
            lua_pushnil(L); return 1;                                \
        }                                                            \
        PUSH;                                                        \
        return 1;                                                    \
    }

READ_FN(l_read_i8,  int8_t,   lua_pushinteger(L, (lua_Integer)v))
READ_FN(l_read_u8,  uint8_t,  lua_pushinteger(L, (lua_Integer)v))
READ_FN(l_read_i16, int16_t,  lua_pushinteger(L, (lua_Integer)v))
READ_FN(l_read_u16, uint16_t, lua_pushinteger(L, (lua_Integer)v))
READ_FN(l_read_i32, int32_t,  lua_pushinteger(L, (lua_Integer)v))
READ_FN(l_read_u32, uint32_t, lua_pushinteger(L, (lua_Integer)v))
READ_FN(l_read_i64, int64_t,  lua_pushinteger(L, (lua_Integer)v))
READ_FN(l_read_ptr, uintptr_t, lua_pushinteger(L, (lua_Integer)v))
READ_FN(l_read_float,  float,  lua_pushnumber(L, (lua_Number)v))
READ_FN(l_read_double, double, lua_pushnumber(L, (lua_Number)v))

/* write_TYPE(addr, off, value) -> bool. Coerces the Lua value to CTYPE and stores it,
 * SEH-guarded. Returns false on a faulting address (no-op). */
#define WRITE_FN(NAME, CTYPE, GET)                                   \
    static int NAME(lua_State *L) {                                  \
        uintptr_t base = (uintptr_t)(lua_Integer)luaL_checkinteger(L, 1); \
        lua_Integer off = luaL_checkinteger(L, 2);                   \
        CTYPE v = (CTYPE)GET(L, 3);                                  \
        uintptr_t p = base + (uintptr_t)off;                         \
        int ok = (p != 0) && safe_store((void *)p, &v, sizeof(v));   \
        lua_pushboolean(L, ok);                                      \
        return 1;                                                    \
    }

WRITE_FN(l_write_i8,     int8_t,   luaL_checkinteger)
WRITE_FN(l_write_u8,     uint8_t,  luaL_checkinteger)
WRITE_FN(l_write_i16,    int16_t,  luaL_checkinteger)
WRITE_FN(l_write_u16,    uint16_t, luaL_checkinteger)
WRITE_FN(l_write_i32,    int32_t,  luaL_checkinteger)
WRITE_FN(l_write_u32,    uint32_t, luaL_checkinteger)
WRITE_FN(l_write_i64,    int64_t,  luaL_checkinteger)
WRITE_FN(l_write_float,  float,    luaL_checknumber)
WRITE_FN(l_write_double, double,   luaL_checknumber)

/* read_bytes(addr, off, n) -> string of n bytes, or nil on fault. n capped at 65536. */
static int l_read_bytes(lua_State *L) {
    uintptr_t base = (uintptr_t)(lua_Integer)luaL_checkinteger(L, 1);
    lua_Integer off = luaL_optinteger(L, 2, 0);
    lua_Integer n = luaL_checkinteger(L, 3);
    uintptr_t p = base + (uintptr_t)off;
    if (n <= 0 || n > 65536 || p == 0) { lua_pushnil(L); return 1; }
    char *buf = (char *)malloc((size_t)n);
    if (!buf) { lua_pushnil(L); return 1; }
    if (!safe_copy(buf, (const void *)p, (size_t)n)) {
        free(buf); lua_pushnil(L); return 1;
    }
    lua_pushlstring(L, buf, (size_t)n);
    free(buf);
    return 1;
}

/* readable(addr [,n=1]) -> bool : true if the range can be read without faulting. */
static int l_readable(lua_State *L) {
    uintptr_t p = (uintptr_t)(lua_Integer)luaL_checkinteger(L, 1);
    lua_Integer n = luaL_optinteger(L, 2, 1);
    if (n <= 0) n = 1;
    if (n > 65536) n = 65536;
    char tmp[64];
    size_t left = (size_t)n;
    const char *src = (const char *)p;
    int ok = (p != 0);
    while (ok && left > 0) {
        size_t chunk = left < sizeof(tmp) ? left : sizeof(tmp);
        ok = safe_copy(tmp, src, chunk);
        src += chunk; left -= chunk;
    }
    lua_pushboolean(L, ok);
    return 1;
}

/* module_base([name]) -> integer base VA of a loaded module (default: main exe). */
static int l_module_base(lua_State *L) {
    const char *name = luaL_optstring(L, 1, NULL);
    HMODULE h = GetModuleHandleA(name);
    if (!h) { lua_pushnil(L); return 1; }
    lua_pushinteger(L, (lua_Integer)(uintptr_t)h);
    return 1;
}

/* thread_id() -> integer : the OS thread this call is running on (a TEB read, free).
 *
 * Diagnostic for the intermittent AV inside UE4SS.dll (2026-07-14): UE4SS does not document
 * which thread it delivers a NotifyOnNewObject callback on. If it runs on the engine's ASYNC
 * LOADING thread, our Lua callback executes concurrently with the poll loop on the same
 * lua_State — which corrupts it, and would explain a crash at a garbage address during a map
 * load. Comparing this inside the callback against its value on the game thread answers it. */
static int l_thread_id(lua_State *L) {
    lua_pushinteger(L, (lua_Integer)GetCurrentThreadId());
    return 1;
}

static const luaL_Reg mem_funcs[] = {
    {"read_i8",  l_read_i8},   {"read_u8",  l_read_u8},
    {"read_i16", l_read_i16},  {"read_u16", l_read_u16},
    {"read_i32", l_read_i32},  {"read_u32", l_read_u32},
    {"read_i64", l_read_i64},  {"read_ptr", l_read_ptr},
    {"read_float", l_read_float}, {"read_double", l_read_double},
    {"read_bytes", l_read_bytes},
    {"write_i8",  l_write_i8},   {"write_u8",  l_write_u8},
    {"write_i16", l_write_i16},  {"write_u16", l_write_u16},
    {"write_i32", l_write_i32},  {"write_u32", l_write_u32},
    {"write_i64", l_write_i64},
    {"write_float", l_write_float}, {"write_double", l_write_double},
    {"readable", l_readable},
    {"module_base", l_module_base},
    {"thread_id", l_thread_id},
    {NULL, NULL}
};

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)lpvReserved;
    if (fdwReason == DLL_PROCESS_ATTACH) { g_self = hinstDLL; }
    return TRUE;
}

__declspec(dllexport) int luaopen_mem_bridge(lua_State *L) {
    luaL_newlib(L, mem_funcs);
    return 1;
}
