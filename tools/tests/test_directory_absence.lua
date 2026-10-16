-- Offline tests for ui_directory's gameplay absence rules (2026-10-15 cinematic report):
-- no title-actor scans in a gameplay world, and a live root with a null owner reads absent
-- except inside the registry's hot window.

local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local clock = 0
os.clock = function() return clock end

local hot = false
local Core = {
    POLL_MS = 100,
    valid = function(o) return type(o) == "table" and o.live == true end,
    member = function(o, name) return o and o[name] or nil end,
    take_scan_slot = function() return true end,
    begin_scan_tick = function() end,
    hot = function() return hot end,
}
package.preload.ui_core = function() return Core end
package.preload.transition = function()
    return { active = function() return false end, on_begin = function() end }
end
package.preload.mem = function()
    return { raw_addr = function(o) return o.addr end }
end

local menu_manager = { live = true, addr = 0x1000 }   -- m_xDlcMenu stays null
local gameplay = false
local scans = {}
Core.findall = function(cls)
    scans[cls] = (scans[cls] or 0) + 1
    if cls == "MenuManager" and gameplay then return { menu_manager } end
    return {}
end
local function obj_name(o) return o == menu_manager and "MenuManager /Game/Map.MenuManager_0" end
menu_manager.GetFullName = function(self) return obj_name(self) end

local Directory = require("ui_directory")

local fails = 0
local function check(cond, what)
    if cond then
        print("ok   - " .. what)
    else
        fails = fails + 1
        print("FAIL - " .. what)
    end
end

-- Quarter seconds: exact in binary, so every step lands in a new 100 ms memo bucket.
local function next_tick(s) clock = clock + (s or 0.25) end

do
    local r = Directory.resolve("Start_DLC_C")
    check(r == nil, "at the title (no MenuManager) an unreachable owner keeps the scan fallback")
    next_tick()
    Directory.resolve("Gametitle_C")
    check((scans["ATTitleLevelScriptActor"] or 0) == 1, "at the title the title actor is looked for")
end

do
    gameplay = true
    next_tick(5)   -- past every root backoff
    local r = Directory.resolve("Start_DLC_C")
    check(type(r) == "table" and #r == 0, "in gameplay a live MenuManager with a null owner reads absent")
    check(Directory.ownerless("Start_DLC_C"), "the class is remembered as ownerless")
    local before = scans["ATTitleLevelScriptActor"]
    next_tick(5)
    local t = Directory.resolve("Gametitle_C")
    check(scans["ATTitleLevelScriptActor"] == before, "in gameplay the title actor is never scanned for")
    check(type(t) == "table" and #t == 0, "a title-only screen reads absent in gameplay")
end

do
    hot = true
    next_tick()
    check(Directory.resolve("Start_DLC_C") == nil, "inside the hot window the old scan fallback runs")
    hot = false
    next_tick()
    local r = Directory.resolve("Start_DLC_C")
    check(type(r) == "table" and #r == 0, "after the hot window it reads absent again")
end

do
    menu_manager.m_xDlcMenu = { live = true, addr = 0x2000, m_UIStartDlc = { live = true, addr = 0x3000 } }
    next_tick()
    local r = Directory.resolve("Start_DLC_C")
    check(r and #r == 1, "a reachable owner still resolves its widget")
    check(not Directory.ownerless("Start_DLC_C"), "reaching the owner clears the ownerless mark")
end

if fails > 0 then
    print(("%d check(s) FAILED"):format(fails))
    os.exit(1)
end
print("all checks passed")
