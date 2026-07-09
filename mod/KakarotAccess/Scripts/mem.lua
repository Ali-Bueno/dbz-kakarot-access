-- Native memory reader: the single place that reads non-UPROPERTY C++ members the
-- UE reflection (and therefore UE4SS Lua) can't see. Used ONLY for the handful of
-- menu selection indices that aren't reflected (field-menu ring, battle pause).
--
-- Transport: mem_bridge.dll — an in-process Lua C module (statically links Lua 5.4.4,
-- same ABI trick as prism_bridge) that does GUARDED reads (SEH: a bad address returns
-- nil, never crashes the game). It holds NO mod logic; all behaviour stays in Lua.
-- DELIBERATELY SEPARATE from prism_bridge.dll (that one is the screen reader only).
--
-- Offsets live in native_offsets.lua (data), recovered via Ghidra (see
-- code/hybrid-mod-feasibility.md). A game patch => re-run Ghidra, edit that file.

local Mem = {}

local m = nil
local loaded = false

function Mem.init()
    local ok, mod = pcall(require, "mem_bridge")
    if ok and mod and mod.read_i32 then
        m = mod
        loaded = true
        print("[KakarotAccess] mem_bridge loaded (native reads available)\n")
    else
        loaded = false
        print("[KakarotAccess] mem_bridge FAILED to load: " .. tostring(mod) .. "\n")
    end
    return loaded
end

function Mem.is_loaded() return loaded end

-- Absolute base VA of a live UObject, or nil. Guards every hop (stale objects error).
function Mem.addr(obj)
    if not loaded or not obj then return nil end
    local ok, a = pcall(function()
        if obj.IsValid and not obj:IsValid() then return nil end
        return obj:GetAddress()
    end)
    if ok and a and a ~= 0 then return a end
    return nil
end

-- Read helpers taking a live UObject + byte offset. Return nil on any failure.
local function reader(fn_name)
    return function(obj, off)
        local a = Mem.addr(obj)
        if not a then return nil end
        return m[fn_name](a, off or 0)
    end
end

Mem.i8    = reader("read_i8")
Mem.u8    = reader("read_u8")
Mem.i16   = reader("read_i16")
Mem.u16   = reader("read_u16")
Mem.i32   = reader("read_i32")
Mem.u32   = reader("read_u32")
Mem.i64   = reader("read_i64")
Mem.ptr   = reader("read_ptr")
Mem.float = reader("read_float")

-- Raw bytes at obj+off (string of n bytes) — used by the offset-finding diff tool.
function Mem.bytes(obj, off, n)
    local a = Mem.addr(obj)
    if not a then return nil end
    return m.read_bytes(a, off or 0, n)
end

-- Read directly from an absolute address (when you already have the VA).
function Mem.at_i32(addr, off) if loaded and addr then return m.read_i32(addr, off or 0) end end
function Mem.at_bytes(addr, off, n) if loaded and addr then return m.read_bytes(addr, off or 0, n) end end

-- Guarded WRITES. Same SEH guard as reads (a bad address is a no-op returning false).
-- Used to snap a game cursor onto a target by overwriting the member the game reads back
-- as its own source (world-map fast travel). Return true only if the store succeeded.
local function writer(fn_name)
    return function(obj, off, v)
        local a = Mem.addr(obj)
        if not a or not m[fn_name] then return false end
        return m[fn_name](a, off or 0, v) == true
    end
end

Mem.write_i32   = writer("write_i32")
Mem.write_u32   = writer("write_u32")
Mem.write_float = writer("write_float")

-- Read a non-reflected UE FString member at obj+off. FString = { TCHAR* Data; int32
-- Num; int32 Max }; Data is UTF-16LE, Num includes the null terminator. Returns a
-- UTF-8 Lua string (BMP only), or nil. Used to read hidden label members the game
-- keeps outside reflection (e.g. the field-menu header section label).
function Mem.fstring(obj, off)
    local a = Mem.addr(obj)
    if not a then return nil end
    off = off or 0
    local data = m.read_ptr(a, off)
    local num = m.read_i32(a, off + 8)
    if not data or data == 0 or not num or num <= 1 or num > 1024 then return nil end
    local raw = m.read_bytes(data, 0, num * 2)   -- num includes the trailing null
    if not raw then return nil end
    local out = {}
    for i = 1, #raw - 1, 2 do
        local lo, hi = raw:byte(i, i + 1)
        if not hi then break end
        local cp = lo + hi * 256
        if cp == 0 then break end
        out[#out + 1] = utf8.char(cp)
    end
    local s = table.concat(out)
    if s == "" then return nil end
    return s
end

return Mem
