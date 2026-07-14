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
    local arr
    if not pcall(function() arr = host.WL_Skilltree_Zorb00 end) or arr == nil then return nil end
    local n
    if not pcall(function() n = arr:GetArrayNum() end) or type(n) ~= "number" then return nil end
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

-- LOCKED nodes. The game marks a lock in exactly ONE place: the tree cursor's own padlock
-- mini-icon (Skilltree_Cursor.WL_ImgIconMicon), and ONLY on a skill's ENTRY node (level 1)
-- when the skill isn't owned at all. Established across 6 capture rounds (2026-07-14): no
-- panel index/pointer/position is readable, the key-help bar is identical on every node,
-- and no cover/lock widget exists on screen. The character-level gate ("you need level 10")
-- lives only in the message window that opens on confirm — which the dialog reader speaks.
--
-- So a higher level of a NOT-OWNED skill carries no marker of its own, even though it is
-- just as unreachable. We propagate it: once the entry node of a skill reads locked, every
-- node of that same skill announces "bloqueada" too (user request — redundant but keeps the
-- information complete while browsing). The map is per screen visit (cleared in reset), so
-- switching character can never carry a stale lock over.
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

-- A skill's family name, so levels of the same skill share a key. The tree names each
-- level node with the SAME skill name (only Txt_Lv_Num differs), so the name IS the key.
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

    if DEBUG then
        dbg(string.format("skill=%s lv=%s ki=%s locked=%d", name,
            tostring(field("Txt_Lv_Num")), tostring(field("Txt_Energy_Num")),
            hovered_locked(name) and 1 or 0))
    end

    local screen = Core.phrase(field("Txt_Name"), I18n.t("skilltree_screen"))
    screen = screen ~= "" and screen or nil

    local lvl = field("Txt_Lv_Num")
    local ki = field("Txt_Energy_Num")
    local need = orbs and grid_phrase(orbs, false)
    local value = Core.phrase(
        hovered_locked(name) and I18n.t("skill_locked") or nil,
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
