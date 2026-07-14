-- Screen adapter: Skill Tree / learn Super Attacks (Start_Skilltree_C < UAT_UISkillTreeMenu).
--
-- Opened with Y/triangle from the character menu: a node graph of learnable super attacks.
-- The tree cursor itself has no reflected index, but the game repopulates a DETAIL PANE on
-- every node move, and ALL of it is REFLECTED on the native base (AT.hpp, UAT_UISkillTreeMenu):
--   Txt_Skillname / Txt_Lv_Num / Txt_Energy_Num / Txt_Detail / Txt_Name (character)
--   WL_Skilltree_Zorb00 : TArray<UAT_UISkilltreeZorb*> — 12 orb counters, each with
--     Txt_Zorb_Num (count) and Ins_Item (icon image → texture reveals the orb COLOR).
-- Grid split (verified against screenshot 91 + F7 dump, 2026-07-14: owned values 1410/1571/
-- 3234/46/10/5 sat in entries 7..12): first 6 entries = REQUIRED cost, last 6 = OWNED.
--
-- Read shape (user spec): name + level + Ki + non-zero cost ("necesita: rojo 2"), then the
-- description, and the OWNED orbs at the very end.

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")

local Tree = {}

-- Deliberately opened sub-screen: one registry confirmation tick suffices (the global
-- CONFIRM_TICKS=2 exists for screens that FLASH at boot).
Tree.confirm_ticks = 1

local ann = Core.make_announcer()
local host, tick = nil, 0

-- Diagnostic: logs the 12 orb counters per node to dumps/dump_skilltree.txt.
-- Re-armed 2026-07-14 with the HOVERED-NODE HUNT (see hunt_hovered below).
-- Diagnostic: logs skill / level / Ki / lock state per node to dumps/dump_skilltree.txt.
local DEBUG = false
local last_dbg = nil
local function dbg(s)
    if not DEBUG or s == last_dbg then return end
    last_dbg = s
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_skilltree.txt", "a")
        if f then f:write(string.format("[%d] %s\n", os.time(), s)) f:close() end
    end)
end

-- Reflected text read off the host by property name (nil if missing/empty). Guarded.
local function field(name)
    local node
    if not pcall(function() node = host[name] end) or node == nil then return nil end
    return Core.read_text(node)
end

-- Orb color, spoken: the slot's position within its 6-orb grid, in the fixed on-screen
-- order red, blue, green, purple, silver, gold — verified TWICE (screenshot 91 and the
-- 7-node debug capture, both 2026-07-14; costs/counts matched throughout). The icon
-- textures do NOT encode the color (every Brush.ResourceObject is named "Ins_Item"), so
-- position is the game's own ordering signal here.
local ORB_KEYS = { "orb_red", "orb_blue", "orb_green", "orb_purple", "orb_silver", "orb_gold" }
local function orb_color(slot)
    return I18n.t(ORB_KEYS[(slot % 6) + 1])
end

-- The 12 orb counters as { color = spoken color, num = count } (nil if unreadable).
-- Entry i (0-based): i < 6 → required grid, i >= 6 → owned grid; slot within grid = i % 6.
local function zorbs()
    local arr, n = Core.array_of(host, "WL_Skilltree_Zorb00")
    if not arr then return nil end
    local out = {}
    for i = 0, n - 1 do
        local z
        if pcall(function() z = arr[i + 1] end) and Core.valid(z) then
            local num = tonumber((Core.read_text(z.Txt_Zorb_Num) or ""):match("%d+") or "")
            out[#out + 1] = { i = i, num = num, color = orb_color(i) }
        end
    end
    return out
end

-- "color count, color count" for one grid (required: skip zeros; owned: skip zeros too —
-- a zero count adds nothing the cost line doesn't already imply). nil if nothing non-zero.
local function grid_phrase(list, owned)
    local parts = {}
    for _, e in ipairs(list) do
        local in_grid = owned and (e.i >= 6) or (not owned and e.i < 6)
        if in_grid and e.num and e.num > 0 then
            parts[#parts + 1] = string.format("%s %d", e.color, e.num)
        end
    end
    if #parts == 0 then return nil end
    return table.concat(parts, ", ")
end

-- NODE STATE (locked / purchasable / acquired). Reflection exposes none of it: the only
-- visible marker is the tree cursor's padlock, and it lights up ONLY on a skill's entry node
-- (6 capture rounds, 2026-07-14). But the game caches the answer per node: it stores its own
-- tri-state on each node widget (zorb + 0x460), computed from HaveSkillTreeId / OpenSkillTreeId,
-- and OnInputDecide gates on exactly that byte. So we read it natively (Ghidra 2026-07-14; the
-- offsets and the proof live in native_offsets.skillTree) — correct on EVERY node, whatever
-- path the player took to reach it.
local NO = require("native_offsets").skillTree
local Mem = require("mem")

local ACQUIRED, OPEN, LOCKED = "acquired", "open", "locked"
local STATE = {
    [0] = LOCKED, [1] = LOCKED,                    -- 1 = hard-gated (the cursor even skips it)
    [2] = ACQUIRED, [5] = ACQUIRED, [8] = ACQUIRED,
    [3] = OPEN, [4] = OPEN, [6] = OPEN, [7] = OPEN, [9] = OPEN, [10] = OPEN,
}

-- The node under the cursor, as the game itself resolves it:
--   cell = grid[row*30 + col] (1-based node index) -> zorb widget -> its state byte.
-- Returns nil if anything is off, so a wrong offset degrades to the old heuristic instead of
-- announcing a lie. SELF-CHECK: the FName the widget caches must equal the one in the parallel
-- id array at the same index — if the index arithmetic were wrong, they wouldn't match.
local function hovered_state()
    if not Mem.is_loaded() then return nil end
    local tree
    if not pcall(function() tree = host.UISkillTree end) or not Core.valid(tree) then return nil end

    local col, row = Mem.i32(tree, NO.cursorCol), Mem.i32(tree, NO.cursorRow)
    local max_col, max_row = Mem.i32(tree, NO.maxCol), Mem.i32(tree, NO.maxRow)
    if not col or not row or col < 0 or row < 0 or col >= NO.gridCols then return nil end
    if (max_col and max_col > 0 and col > max_col) or (max_row and max_row > 0 and row > max_row) then
        return nil
    end

    local cell = Mem.i32(tree, NO.grid + (row * NO.gridCols + col) * 4)
    if not cell or cell <= 0 then return nil end          -- empty grid cell: no node hovered
    local i = cell - 1

    local ids, count = Mem.ptr(tree, NO.nodeIdData), Mem.i32(tree, NO.nodeIdCount)
    local data = Mem.ptr(tree, NO.zorbData)
    if not ids or not count or not data or i >= count then return nil end

    local zorb = Mem.at_ptr(data, i * 8)
    if not zorb or zorb == 0 then return nil end
    if Mem.at_bytes(zorb, NO.zorbNodeId, 8) ~= Mem.at_bytes(ids, i * 8, 8) then return nil end

    return STATE[Mem.at_u8(zorb, NO.zorbState) or -1]
end

-- FALLBACK (native read unavailable): the pre-Ghidra heuristic. The cursor padlock only marks a
-- skill's entry node, so we propagate the lock by skill name within a screen visit — every level
-- of the same skill then reads locked. Blind to a level-2/3 node reached without passing level 1;
-- that limit is exactly what the native read above removes.
local locked_skills = {}

local function cursor_padlock()
    local locked = false
    pcall(function()
        local tree = host.UISkillTree
        if Core.valid(tree) then
            locked = Core.is_visible(tree.Skilltree_Cursor.WL_ImgIconMicon)
        end
    end)
    return locked
end

-- The tree names every level node of a skill with the SAME skill name, so the name is the key.
local function hovered_locked(name)
    if cursor_padlock() then
        if name then locked_skills[name] = true end
        return true
    end
    return name ~= nil and locked_skills[name] == true
end

function Tree.is_active()
    tick = tick + 1
    host = Core.first_on_screen("Start_Skilltree_C", tick)
    return host ~= nil
end

function Tree.reset() ann:reset() locked_skills = {} end

function Tree.update()
    local name = A.markup_to_speech(field("Txt_Skillname"))
    if not name then return end

    local orbs = zorbs()

    -- Native tri-state when available; the name-propagation heuristic only as a fallback.
    local state = hovered_state()
    local locked = (state == LOCKED) or (state == nil and hovered_locked(name))

    if DEBUG then
        dbg(string.format("skill=%s lv=%s ki=%s state=%s", name,
            tostring(field("Txt_Lv_Num")), tostring(field("Txt_Energy_Num")),
            tostring(state or (locked and "locked?" or "unknown"))))
    end

    local screen = Core.phrase(field("Txt_Name"), I18n.t("skilltree_screen"))
    screen = screen ~= "" and screen or nil

    local lvl = field("Txt_Lv_Num")
    local ki = field("Txt_Energy_Num")
    local need = orbs and grid_phrase(orbs, false)
    local value = Core.phrase(
        locked and I18n.t("skill_locked") or nil,
        state == ACQUIRED and I18n.t("skill_acquired") or nil,
        lvl and I18n.t("skill_level"):format(lvl) or nil,
        ki and I18n.t("skill_ki"):format(ki) or nil,
        need and I18n.t("skill_needs"):format(need) or nil)
    if value == "" then value = nil end

    -- Description, then the OWNED orbs at the very end (user spec). Lazy: computed only
    -- when the announcer actually speaks the tooltip.
    local tip = function()
        if A.markup_to_speech(field("Txt_Skillname")) ~= name then return nil end
        local desc = A.markup_to_speech(field("Txt_Detail"))
        local o = zorbs()
        local owned = o and grid_phrase(o, true)
        return Core.phrase(desc, owned and I18n.t("skill_owned"):format(owned) or nil)
    end

    ann:focus(screen, nil, name, value, tip)
end

return Tree
