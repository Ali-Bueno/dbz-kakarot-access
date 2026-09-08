-- Offline regression tests for the Community Board reader (screen_community.lua).
--
-- Pins the two 2026-09-05 findings from the live stuck-session log:
--   1. The one-shot board entry summary must NOT re-fire on dispatcher resets or
--      on the board->grid->board handoff (each registry flip called reset(), which
--      cleared the title latch and could requeue the full summary; repeated
--      dispatcher flips were observed). Genuine close/reopen, a board change, and an explicit F1
--      reannounce must still re-announce it.
--   2. The leader badge must survive strict-gate metadata being unavailable: both
--      names are declared on the native UAT_UICommunityBoard_Panel (AT.hpp,
--      WL_Pnl_Pedestal_Leader @0x4F8 / WL_Ins_Icon_Leader @0x500), so they are
--      normal-gate reads. The strict gate refused both live ("no property set for
--      Brd_Emb_C"), which could omit the visible leader flag.

local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

-- ---- fake world -------------------------------------------------------------

local roam = false            -- Core.free_roam answer
local protected = false       -- Speech.protected answer
local pools = {}              -- class name -> array of fake hosts
local says = {}               -- every Speech.say: { text, interrupt }
local focuses = {}            -- every announcer focus: { tab, name, value }
local resets = 0              -- announcer resets
local invalidated = 0         -- announcer invalidates
local transition_fn = nil     -- screen_community's map-switch flush
local clock_before, now = os.clock, 100
os.clock = function() return now end

local function stub(name, members)
    package.preload[name] = function() return members end
end

-- Behavior-compatible slice of ui_core. Fake widgets are plain tables:
--   __valid=false -> Core.valid false; __vis=false -> is_visible false;
--   __on=false -> on_screen false; __live=false -> pane_live false;
--   __mem[off] -> Mem reads; text -> Core.read_text.
local Core = {}
function Core.valid(o) return type(o) == "table" and o.__valid ~= false end
function Core.is_visible(o) return type(o) == "table" and o.__vis ~= false end
function Core.on_screen(o) return type(o) == "table" and o.__on ~= false end
function Core.pane_live(o) return type(o) == "table" and o.__live ~= false end
function Core.nonnull(o) return o ~= nil end
function Core.read_text(o) return type(o) == "table" and o.text or nil end
function Core.free_roam() return roam end
function Core.scan_quiet() return false end
function Core.watch_for() end
function Core.watch_clear() end
function Core.refresh_all() end
function Core.allow_member() end
function Core.first_on_screen(cls)
    local p = pools[cls]
    return p and p[1] or nil
end
function Core.cached_all(cls) return pools[cls] or {} end
Core.cached_live = Core.first_on_screen
-- Strict metadata UNAVAILABLE is the scenario under test: a strict fetch answers
-- nil (the gate refuses the candidate), a normal fetch reads the declared member.
function Core.member(o, name, strict)
    if type(o) ~= "table" then return nil end
    if strict then return nil end
    return o[name]
end
function Core.member_path(o, ...)
    local v = o
    for i = 1, select("#", ...) do
        if type(v) ~= "table" then return nil end
        v = v[(select(i, ...))]
    end
    return v
end
function Core.struct_member(h, name)
    return type(h) == "table" and h[name] or nil
end
function Core.array_of(o, name)
    if type(o) ~= "table" then return nil end
    local t = o[name]
    if type(t) ~= "table" then return nil end
    return t, #t
end
function Core.phrase(...)
    local parts = {}
    for i = 1, select("#", ...) do
        local p = select(i, ...)
        if p and p ~= "" then parts[#parts + 1] = p end
    end
    return table.concat(parts, ", ")
end
function Core.make_announcer()
    return {
        reset = function() resets = resets + 1 end,
        invalidate = function() invalidated = invalidated + 1 end,
        focus = function(_, tab, name, value)
            focuses[#focuses + 1] = { tab = tab, name = name, value = value }
        end,
    }
end
stub("ui_core", Core)

stub("ui_archetypes", { markup_to_speech = function(t) return t end })
stub("speech", {
    say = function(text, interrupt) says[#says + 1] = { text, interrupt } end,
    protected = function() return protected end,
})
stub("mem", {
    i32 = function(o, off) return type(o) == "table" and o.__mem and o.__mem[off] or nil end,
    u8 = function(o, off) return type(o) == "table" and o.__mem and o.__mem[off] or nil end,
    float = function(o, off) return type(o) == "table" and o.__mem and o.__mem[off] or nil end,
    ptr = function(o, off) return type(o) == "table" and o.__mem and o.__mem[off] or nil end,
    addr = function() return 42 end,
})
local OFF = {
    commuBoard = {
        mode = 0x500, subState = 0x4F3, hoveredCache = 0x5D8,
        cursorX = 0x90, cursorY = 0x94,
        activeCount = 0x628, hitAdjX = 0x428, hitAdjY = 0x42C, hitRange = 0x420,
        leaderAdjX = 0x430, leaderAdjY = 0x434, leaderRange = 0x424,
        panelPosX = 0x3F0, panelPosY = 0x3F4, panelBoardX = 0x550, panelBoardY = 0x554,
        heldEmblem = 0x7B8,
    },
    commuGrid = { cursorIndex = 0x3EC, colIndex = 0x3D0, rowIndex = 0x3D4 },
}
stub("native_offsets", OFF)
stub("transition", { on_begin = function(_, fn) transition_fn = fn end })
stub("ui_directory", { peek = function() return nil end })
stub("ui_registry", {
    hot = function() return false end,
    active_adapter = function() return nil end,
})
local S = {
    board_socket = "panel %d of %d", empty_socket = "Empty panel", lvl = "level %s",
    leader = "leader", placed = "Placed:", board_free = "free",
    board_hint = "%d panels. Press confirm on an empty panel.",
    pos = "%d of %d", commu_lv = "community level %s",
    not_acquired = "Not acquired", new_label = "new",
    board_holding = "Emblem in hand. Move to a panel and confirm to place it.",
}
stub("i18n", {
    t = function(k) return S[k] or k end,
    header = function(n) return "header" .. n end,
})

-- The board itself: two sockets, socket 1 holds Goku (the leader), socket 2 empty.
local goku_tex = { GetFullName = function()
    return "Texture2D /Game/Art/UI/Charicon_Ev/Ev_Gok00_00_00.Ev_Gok00_00_00"
end }
local goku_emb = {
    ImageFace = { Brush = { ResourceObject = {
        TextureParameterValues = { { ParameterValue = goku_tex } },
        GetFullName = function() return "MaterialInstanceDynamic fake" end,
    } } },
}
local panel1 = {
    WL_Emblem = { UIXCmnEmb = goku_emb },
    WL_Lv = { text = "5" },
    WL_Pnl_Pedestal_Leader = { __vis = true },
}
local panel2 = {}   -- empty socket
local frame = {
    WL_PanelTbl = { panel1, panel2 },
    __mem = { [OFF.commuBoard.activeCount] = 2 },
}
local detail = { WL_Txt_Titl00 = { text = "Z Warriors" } }
local board = {
    WL_BrdFrame = frame,
    WL_CommuBrdDetail = detail,
    __mem = { [OFF.commuBoard.mode] = 7, [OFF.commuBoard.hoveredCache] = 1 },
}

local Commu = require("screen_community")

local fails = 0
local function check(cond, what)
    if cond then
        print("ok   - " .. what)
    else
        fails = fails + 1
        print("FAIL - " .. what)
    end
end

-- One reader tick, exactly as the registry drives it.
local function tick()
    local active = Commu.is_active()
    if active then Commu.update() end
    return active
end

local function last_focus() return focuses[#focuses] end

-- ---- entry: summary once, leader read, hover read ---------------------------

pools["Start_Commu_Brd_C"] = { board }
tick()
check(#says == 1, "board entry speaks the summary exactly once")
check(says[1] and says[1][1]:find("Placed:", 1, true) ~= nil,
    "the entry summary lists the placed emblems")
check(says[1] and says[1][1]:find("Goku, level 5, leader", 1, true) ~= nil,
    "the placed list marks the leader when strict metadata is unavailable")
check(last_focus() and last_focus().value
        and last_focus().value:find("Goku, level 5, leader, panel 1 of 2", 1, true) ~= nil,
    "the hovered socket label marks the leader when strict metadata is unavailable")

board.__mem[OFF.commuBoard.hoveredCache] = 2
tick()
check(last_focus() and last_focus().value == "Empty panel, panel 2 of 2",
    "moving the hover announces the newly hovered socket")

-- ---- dispatcher flip (screen_dialog blip): reset must not re-dump -----------

Commu.reset()       -- what ui_registry's safe_reset(cur) does on every screen flip
tick()
check(#says == 1, "a dispatcher reset does not repeat the unchanged entry summary")

-- ---- board -> grid -> board handoff (mode 10 is not closure) ----------------

local grid_slot = {}
local emblist = { EmbAry = { grid_slot },
    __mem = { [OFF.commuGrid.cursorIndex] = 0 } }
local grid_host = { EmbList = emblist }
board.__mem[OFF.commuBoard.mode] = 10
pools["Start_Commu_Emb_C"] = { grid_host }
tick()
check(last_focus() and last_focus().tab == "header5",
    "board mode 10 hands the tick to the emblem grid reader")
board.__mem[OFF.commuBoard.mode] = 7
pools["Start_Commu_Emb_C"] = nil
tick()
check(#says == 1, "returning from the grid does not repeat the entry summary")
check(last_focus() and last_focus().tab == "header1",
    "returning from the grid resumes the board readout")

-- ---- a protected instruction defers, then the readout comes back ------------

protected = true
local n_focus, n_say = #focuses, #says
tick()
check(#focuses == n_focus and #says == n_say,
    "a protected instruction holds the board readout")
protected = false
local n_inv = invalidated
tick()
check(invalidated > n_inv and #focuses > n_focus,
    "the held readout returns fresh once the instruction finishes")

-- ---- a genuine board change still announces ---------------------------------

detail.WL_Txt_Titl00.text = "Turtle School"
tick()
check(#says == 2, "switching to a different board speaks that board's summary")
check(last_focus() and last_focus().name == "Turtle School",
    "the announcer carries the new board title")

-- ---- genuine close (free roam seen) and reopen re-announces -----------------

pools["Start_Commu_Brd_C"] = nil
roam = true
check(tick() == false, "with the board gone and free roam back, the adapter releases")
Commu.reset()       -- the registry resets on the way out too
roam = false
pools["Start_Commu_Brd_C"] = { board }
tick()
check(#says == 3, "a genuine close and reopen speaks the summary again")

-- ---- explicit F1 reannounce always re-speaks --------------------------------

if Commu.reannounce then Commu.reannounce() else Commu.reset() end
tick()
check(#says == 4, "F1 reannounce re-speaks the entry summary on demand")

-- A higher-priority modal stops registry sweeps before they reach this adapter.
-- Even a long gap between probes is not positive evidence that the board closed.
local before = #says
Commu.reset()
now = now + 60
tick()
check(#says == before, "a long popup without Community probes does not re-dump the summary")

-- Missing pools/reads also must not count as closing and reopening the board.
before = #says
pools["Start_Commu_Brd_C"] = nil
now = now + 60
tick()
pools["Start_Commu_Brd_C"] = { board }
tick()
check(#says == before, "a scan miss followed by the same board preserves summary history")

before = #says
board.__mem[OFF.commuBoard.mode] = 10
pools["Start_Commu_Emb_C"] = { grid_host }
for _ = 1, 60 do now = now + 1; tick() end
board.__mem[OFF.commuBoard.mode] = 7
pools["Start_Commu_Emb_C"] = nil
tick()
check(#says == before, "a long mode-10 grid stay is not board closure")

before = #says
board.__mem[OFF.commuBoard.mode] = 10
tick()
check(#says == before, "mode 10 without a materialized grid is not closure")
board.__mem[OFF.commuBoard.mode] = nil
tick()
board.__mem[OFF.commuBoard.mode] = 7
tick()
check(#says == before, "an unreadable native mode does not erase summary history")
board.__mem[OFF.commuBoard.mode] = 42
tick()
board.__mem[OFF.commuBoard.mode] = 7
tick()
check(#says == before, "an unrecognized native mode is not guessed to be closure")

-- The native close routine uses 5 (out animation) then 0 (parked). These are
-- closure evidence only if a live Community detail/grid/board does not win.
board.__mem[OFF.commuBoard.mode] = 5
pools["Start_Commu_Emb_C"] = { grid_host }
tick()
board.__mem[OFF.commuBoard.mode] = 7
pools["Start_Commu_Emb_C"] = nil
tick()
check(#says == before, "a parked board with a live emblem grid preserves history")
for _, closed_mode in ipairs({5, 0}) do
    before = #says
    board.__mem[OFF.commuBoard.mode] = closed_mode
    frame.__on = false
    tick()
    Commu.reset()
    frame.__on = true
    board.__mem[OFF.commuBoard.mode] = 7
    tick()
    check(#says == before + 1,
        "native close mode " .. closed_mode .. " re-arms the summary without free roam")
end

before = #says
transition_fn()
tick()
check(#says == before + 1, "a real map transition re-arms the board summary")

-- Holding is read from the native pointer, and the current reminder is available
-- on F1 even when the pickup itself happened before a protected tutorial line.
local function holding_count()
    local count = 0
    for _, line in ipairs(says) do
        if line[1] == S.board_holding then count = count + 1 end
    end
    return count
end
frame.__mem[OFF.commuBoard.heldEmblem] = 1234
protected = true
tick()
check(holding_count() == 0, "a tutorial instruction defers the pickup reminder")
protected = false
tick()
check(holding_count() == 1, "the pickup reminder reads once after the instruction")
tick()
check(holding_count() == 1, "idling with an emblem does not repeat the pickup reminder")
Commu.reannounce()
tick()
check(holding_count() == 2, "F1 repeats the current emblem-in-hand reminder")
local held_before = holding_count()
frame.__mem[OFF.commuBoard.heldEmblem] = 0
tick()
check(holding_count() == held_before, "a released emblem is not announced as still held")
frame.__mem[OFF.commuBoard.heldEmblem] = 5678
tick()
check(holding_count() == held_before + 1, "a new pickup after release announces the held state")

os.clock = clock_before
if fails > 0 then
    print(("%d check(s) FAILED"):format(fails))
    os.exit(1)
end
print("all checks passed")
