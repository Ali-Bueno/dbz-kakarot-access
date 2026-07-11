-- Dev-only UNIVERSAL offset probe. Reads a broad hidden window of every candidate
-- UI object on screen and reports which int32 changed since the last F4 press. Open
-- ANY menu, tap F4 (baseline), move the cursor / turn the page, tap F4 again: the
-- "<Class> +0xNNN = v" whose value steps 0->1->2... is that menu's selection/page index.
--
-- State kept in _G so it survives this module being re-required fresh each F4 press.
-- Not shipped. Add a class to CANDIDATES if a menu isn't caught (F7 discover names it).

local Mem = require("mem")

local M = {}

-- Candidate container classes to probe, and a broad tail window that covers their
-- non-reflected members. Reads are SEH-guarded, so an over-long window is harmless.
local CANDIDATES = {
    "Xcmn_Pause_C",            -- battle pause (done: 0x43C)
    "Start_Top_C",             -- overworld / field menu (ring index 0x4e4)
    "Tips_C",                  -- tips book (multi-page)
    "Battle_Tips_Tutorial_C",  -- battle tips
    "Xcmn_Win01_C",            -- generic window (View Controls / conditions?)
    "AT_UIStartSaveLoad",      -- save/load slots (native; hunting cursor idx in tail 0x408..0x430)
}
local FROM, LEN = 0x398, 0x180   -- 0x398 .. 0x518

local function short(cls) return (cls:gsub("_C$", "")) end

local function le_i32(s, i)
    local a, b, c, d = s:byte(i, i + 3)
    if not d then return nil end
    local v = a + b * 256 + c * 65536 + d * 16777216
    if v >= 0x80000000 then v = v - 0x100000000 end
    return v
end

-- Labeled snapshot of Start_Top's candidate "menu depth" fields, so we can compare exact
-- values between ring vs inside-a-submenu (cleaner than the byte-diff, which overlaps).
local function dump_starttop(DevLog)
    local list = FindAllOf("Start_Top_C")
    if not list then return end
    for _, o in ipairs(list) do
        if o and o:IsValid() then
            local ok, fn = pcall(function() return o:GetFullName() end)
            if ok and fn and fn:match("BP_ATGameInstance_C_%d+%.[%w_]+$") then
                local parts = {}
                for off = 0x4b0, 0x4fc, 4 do
                    parts[#parts + 1] = string.format("%X=%s", off, tostring(Mem.i32(o, off)))
                end
                local p = Mem.ptr(o, 0x4d8)
                parts[#parts + 1] = "ptr4d8=" .. tostring(p and p ~= 0)
                DevLog.write("[fields] " .. table.concat(parts, " "))
                return
            end
        end
    end
end

function M.run()
    local Speech = require("speech")
    local DevLog = require("dev_log")
    ExecuteInGameThread(function()
        local store = _G.__memdiff
        if not store then store = { snaps = {} }; _G.__memdiff = store end

        dump_starttop(DevLog)
        local reads, changes = 0, {}
        for _, cls in ipairs(CANDIDATES) do
            local list = FindAllOf(cls)
            if list then
                for _, o in ipairs(list) do
                    if o and o:IsValid() then
                        local ok, key = pcall(function() return o:GetFullName() end)
                        local cur = Mem.bytes(o, FROM, LEN)
                        if ok and key and cur then
                            reads = reads + 1
                            local prev = store.snaps[key]
                            if prev then
                                for off = 0, LEN - 4 do
                                    local a = le_i32(prev, off + 1)
                                    local b = le_i32(cur, off + 1)
                                    if a and b and a ~= b and b >= 0 and b < 64 then
                                        changes[#changes + 1] = string.format(
                                            "%s +0x%X = %d (was %d)", short(cls), FROM + off, b, a)
                                    end
                                end
                            end
                            store.snaps[key] = cur
                        end
                    end
                end
            end
        end

        if reads == 0 then
            Speech.say("memdiff: no candidate object on screen (press F7 to name it)", true)
            DevLog.write("no candidate object on screen")
        elseif not store.primed then
            store.primed = true
            Speech.say(string.format("memdiff baseline on %d objects, move and press again", reads), true)
            DevLog.write(string.format("--- baseline captured on %d objects ---", reads))
        elseif #changes == 0 then
            Speech.say("memdiff: no small-int change", true)
            DevLog.write("no small-int change (" .. reads .. " objects)")
        else
            local msg = table.concat(changes, "; ")
            Speech.say("memdiff: " .. msg, true)
            DevLog.write(msg)
        end
    end)
end

function M.reset() _G.__memdiff = nil end

return M
