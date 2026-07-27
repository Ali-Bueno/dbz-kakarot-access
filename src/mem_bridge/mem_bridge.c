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

/* SEH filter for every guarded dereference in this module.
 *
 * It still swallows EVERYTHING — that immunity is the whole point of the bridge — but a
 * STATUS_GUARD_PAGE_VIOLATION must not be swallowed SILENTLY. The kernel CLEARS the page's
 * PAGE_GUARD flag when it raises one, so probing addresses we do not own (Mem.at_ptr /
 * Mem.readable walk arbitrary pointers and sweep up to 64 KB) can strip the tripwire off
 * another thread's stack guard page — turning a future stack overflow into silent heap
 * corruption. Re-arm the page so the probe stays a pure no-op.
 * ExceptionInformation[1] is the touched address on both AV and guard-page records. */
static int fault_filter(EXCEPTION_POINTERS *ep) {
    if (ep->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION &&
        ep->ExceptionRecord->NumberParameters >= 2) {
        void *addr = (void *)ep->ExceptionRecord->ExceptionInformation[1];
        MEMORY_BASIC_INFORMATION mbi;
        DWORD old;
        if (VirtualQuery(addr, &mbi, sizeof(mbi)) == sizeof(mbi) &&
            mbi.State == MEM_COMMIT && !(mbi.Protect & PAGE_GUARD)) {
            /* VirtualProtect re-arms the whole page containing `addr`. */
            VirtualProtect(addr, 1, mbi.Protect | PAGE_GUARD, &old);
        }
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

/* Copy n bytes from src, guarded by SEH. Returns 1 on success, 0 on access fault. */
static int safe_copy(void *dst, const void *src, size_t n) {
    __try {
        memcpy(dst, src, n);
        return 1;
    } __except (fault_filter(GetExceptionInformation())) {
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
    } __except (fault_filter(GetExceptionInformation())) {
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

/* Largest byte offset a WRITE may carry. The SEH guard only turns an UNMAPPED destination
 * into a no-op; a valid-but-wrong address corrupts live game memory silently and crashes
 * later somewhere unrelated, so the offset itself has to be bounded here — this is the only
 * arbitrary-write primitive the mod exposes to Lua. DERIVED, not invented: the largest
 * offset in the mod's own offset table (Scripts/native_offsets.lua) is
 * skillTree.cursorRow = 0x15FC, so the next power of two above it accepts every offset the
 * mod can legitimately produce while refusing anything an order of magnitude out (a stale
 * offset after a game patch, a Lua arithmetic slip). READS are deliberately NOT bounded:
 * a read of a wrong address is already a harmless nil, and the pointer walkers need it. */
#define MAX_WRITE_OFFSET 0x2000

/* write_TYPE(addr, off, value) -> bool. Coerces the Lua value to CTYPE and stores it,
 * SEH-guarded. Returns false on a faulting address, or on an offset outside
 * MAX_WRITE_OFFSET (no-op in both cases). */
#define WRITE_FN(NAME, CTYPE, GET)                                   \
    static int NAME(lua_State *L) {                                  \
        uintptr_t base = (uintptr_t)(lua_Integer)luaL_checkinteger(L, 1); \
        lua_Integer off = luaL_checkinteger(L, 2);                   \
        CTYPE v = (CTYPE)GET(L, 3);                                  \
        uintptr_t p = base + (uintptr_t)off;                         \
        int ok = (p != 0) && off >= 0 &&                             \
                 off <= (lua_Integer)(MAX_WRITE_OFFSET - sizeof(v)) && \
                 safe_store((void *)p, &v, sizeof(v));               \
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
    luaL_Buffer b;
    char *buf;
    if (n <= 0 || n > 65536 || p == 0) { lua_pushnil(L); return 1; }
    /* Lua's own buffer instead of malloc/free: lua_pushlstring can raise LUA_ERRMEM, and a
     * Lua error longjmps straight past any free() after it — leaking up to 64 KB each time.
     * The buffer's storage belongs to the Lua stack, so an error releases it for us. */
    buf = luaL_buffinitsize(L, &b, (size_t)n);
    if (!safe_copy(buf, (const void *)p, (size_t)n)) {
        luaL_pushresultsize(&b, 0);   /* close the buffer, then discard it */
        lua_pop(L, 1);
        lua_pushnil(L);
        return 1;
    }
    luaL_pushresultsize(&b, (size_t)n);
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

/* ---- crash black box ---------------------------------------------------------------
 *
 * WHY (2026-07-26): the mod's crashes kill the process from inside UE4SS via an uncatchable
 * C++ throw, so there is no Lua traceback, and the UE crash dump only names UE4SS.dll offsets
 * in a stripped 19 MB binary. Every round of diagnosis so far has been inference. This records
 * WHAT THE MOD WAS DOING, so the next crash names its own site instead of costing another
 * blind round.
 *
 * HOW: a ring buffer in a MEMORY-MAPPED FILE. `mark()` is a memcpy into a mapped page — cheap
 * enough to call on every adapter probe (hundreds/second) — and because the page is backed by
 * a file, the memory manager writes it out when the process dies. No exception handler is
 * involved: a vectored handler was considered and rejected, because it fires on every
 * first-chance exception in the process (the game and UE4SS both throw routinely and catch
 * their own), so it would be both noisy and a new way to destabilise a crashing process.
 *
 * HONEST LIMIT: dirty pages of a file mapping are flushed on process teardown, which the OS
 * performs even for an unhandled exception. That covers a crash. It does NOT cover a hard
 * power loss or a bugcheck. `mark_flush()` exists for the paranoid case; it is not needed on
 * the normal path and should not be called per tick.
 */
#define MARK_MAGIC    0x314B414Bu    /* "KAK1" */
#define MARK_VERSION  1u
#define MARK_SLOTS    64u            /* ~180 ms of trail at the busiest marking rate */
#define MARK_TEXT     112u

typedef struct {
    uint32_t magic, version, slots, text_size;
    uint64_t seq;                    /* total marks written this session */
    uint64_t reserved;
} MarkHeader;

typedef struct {
    uint64_t seq;                    /* 0 = slot never written */
    uint64_t tick_ms;                /* GetTickCount64 at write time */
    char     text[MARK_TEXT];
} MarkSlot;

/* The mapping must hold the header AND every slot. Getting this wrong is not a subtle bug: the
 * first build used a round 8192 while header+slots came to 8224, so the last two slots wrote
 * past the end of the view and the RECOVERY read faulted — a crash-diagnostic that crashed, at
 * boot, on every launch. Caught only because the black box was tested standalone (kill the
 * writer with TerminateProcess, recover from a second process) instead of being shipped on the
 * strength of compiling. Hence the size is DERIVED and a compile-time assert enforces it. */
#define MARK_BYTES    16384u
typedef char mark_size_check[(sizeof(MarkHeader) + MARK_SLOTS * sizeof(MarkSlot) <= MARK_BYTES) ? 1 : -1];

static HANDLE     g_mark_file = NULL;
static HANDLE     g_mark_map  = NULL;
static MarkHeader *g_mark     = NULL;
static MarkSlot   *g_slots    = NULL;

/* Wipe the ring and stamp this session's header. Guarded like every other touch of the
 * section (file-backed -> EXCEPTION_IN_PAGE_ERROR). Returns 1 on success. */
static int mark_reset(MarkHeader *h) {
    __try {
        memset(h, 0, MARK_BYTES);
        h->magic = MARK_MAGIC; h->version = MARK_VERSION;
        h->slots = MARK_SLOTS; h->text_size = MARK_TEXT; h->seq = 0;
        return 1;
    } __except (fault_filter(GetExceptionInformation())) {
        return 0;
    }
}

/* Write one trail entry, guarded for the same reason as mark_reset. Returns 1 on success. */
static int mark_write(MarkHeader *h, MarkSlot *slots, const char *t, size_t len) {
    __try {
        uint64_t n = h->seq + 1;
        MarkSlot *sl = &slots[(n - 1) % MARK_SLOTS];
        /* Text first, then the slot's seq, then the header: a torn write during a crash then
         * leaves a slot that looks unwritten rather than one with mismatched text. */
        memcpy(sl->text, t, len);
        sl->text[len] = '\0';
        sl->tick_ms = GetTickCount64();
        sl->seq = n;
        h->seq = n;
        return 1;
    } __except (fault_filter(GetExceptionInformation())) {
        return 0;
    }
}

/* mark_open(path) -> table|nil, err
 * Opens/creates the ring, RETURNS THE PREVIOUS SESSION'S TRAIL (oldest -> newest) and then
 * resets it for this session. Reading before resetting is the whole point: if the last run
 * crashed, its final operations are what this returns. */
static int l_mark_open(lua_State *L) {
    const char *path = luaL_checkstring(L, 1);
    if (g_mark) { lua_pushnil(L); lua_pushstring(L, "already open"); return 2; }

    /* Share read AND write: the trail must be inspectable while the game is running. */
    g_mark_file = CreateFileA(path, GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (g_mark_file == INVALID_HANDLE_VALUE) {
        g_mark_file = NULL;
        lua_pushnil(L); lua_pushstring(L, "cannot open marker file"); return 2;
    }
    g_mark_map = CreateFileMappingA(g_mark_file, NULL, PAGE_READWRITE, 0, MARK_BYTES, NULL);
    if (!g_mark_map) {
        CloseHandle(g_mark_file); g_mark_file = NULL;
        lua_pushnil(L); lua_pushstring(L, "cannot map marker file"); return 2;
    }
    void *view = MapViewOfFile(g_mark_map, FILE_MAP_ALL_ACCESS, 0, 0, MARK_BYTES);
    if (!view) {
        CloseHandle(g_mark_map); CloseHandle(g_mark_file);
        g_mark_map = NULL; g_mark_file = NULL;
        lua_pushnil(L); lua_pushstring(L, "cannot create view"); return 2;
    }

    MarkHeader *h = (MarkHeader *)view;
    MarkSlot   *s = (MarkSlot *)((char *)view + sizeof(MarkHeader));
    MarkHeader hdr;
    int haveHdr;

    /* Take ownership of the mapping BEFORE anything that can raise. Every Lua allocation
     * below (lua_newtable, lua_pushfstring) can raise LUA_ERRMEM, and a Lua error longjmps
     * out of this function past any local cleanup: with the globals already published such
     * an unwind leaves the module CONSISTENT — the "already open" guard above catches the
     * retry — instead of leaking the file, the mapping and the view with g_mark still NULL. */
    g_mark = h; g_slots = s;

    /* Recover the previous trail before touching anything. A mismatched magic means a fresh
     * or foreign file: no trail, not an error. Every access to the section goes through
     * safe_copy: it is FILE-backed, so a bad sector or a lost path raises
     * EXCEPTION_IN_PAGE_ERROR — and the crash black box must never be what crashes. */
    haveHdr = safe_copy(&hdr, h, sizeof(hdr));
    lua_newtable(L);
    int n = 0;
    if (haveHdr && hdr.magic == MARK_MAGIC && hdr.version == MARK_VERSION &&
        hdr.slots == MARK_SLOTS && hdr.text_size == MARK_TEXT) {
        uint64_t total = hdr.seq;
        uint64_t first = (total > MARK_SLOTS) ? (total - MARK_SLOTS) : 0;
        for (uint64_t i = first; i < total; i++) {
            MarkSlot sl;
            if (!safe_copy(&sl, &s[i % MARK_SLOTS], sizeof(sl))) break;
            if (sl.seq == 0) continue;
            sl.text[MARK_TEXT - 1] = '\0';
            lua_pushfstring(L, "%d\t%s", (int)(sl.tick_ms & 0x7fffffff), sl.text);
            lua_rawseti(L, -2, ++n);
        }
    }

    mark_reset(h);
    return 1;
}

/* mark(text) -> boolean. A memcpy; safe to call from any hot path. */
static int l_mark(lua_State *L) {
    if (!g_mark) { lua_pushboolean(L, 0); return 1; }
    size_t len = 0;
    const char *t = luaL_checklstring(L, 1, &len);
    if (len > MARK_TEXT - 1) len = MARK_TEXT - 1;

    lua_pushboolean(L, mark_write(g_mark, g_slots, t, len));
    return 1;
}

/* mark_flush() -> boolean. Forces the page out now. Not needed for crash survival. */
static int l_mark_flush(lua_State *L) {
    lua_pushboolean(L, g_mark && FlushViewOfFile((void *)g_mark, MARK_BYTES));
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
    {"mark_open", l_mark_open}, {"mark", l_mark}, {"mark_flush", l_mark_flush},
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
