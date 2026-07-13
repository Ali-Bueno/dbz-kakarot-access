-- Screen adapter: Skill Palette / Super Attack equip (Start_Skillcustom_C -> UAT_UISkillCustomize).
--
-- The "Paleta de Habilidades" opened from a character: equip Super Attacks (and their support
-- versions) to the palette slots.
--
-- Source truth (dump_skill 2026-07-13): SkillListMenu:GetSelectValue() is DEAD on this screen
-- (always 0, its rows never move), so the list is NOT the cursor. The DETAIL PANE (host
-- SkillNameTxt / SkillLevelTxt / SkillSpTxt / SkillDetailTxt) is what follows the cursor —
-- except on an EMPTY slot, where it goes stale and keeps the previous skill.
--
-- So the reader works off two sources:
--   * slot plates (CategoryPlateCtn, one UAT_UISkillCustomizePlate per palette slot): the
--     SELECTED plate — marked by its SelectActiveBorder being the only visible one — gives the
--     row's own name ("---" literally on an empty slot -> "ranura vacía") and the slot's
--     BUTTON glyph (ButtonIconImage -> A.platbtn_name).
--   * detail pane: level, Ki cost and description — attached ONLY while the pane shows the
--     same skill as the announced name, so its lag can't mix another slot's data in.
-- If the border heuristic is unreliable (0 or >1 visible), we fall back to the pane as the
-- name source (populated slots still read; the plate is then matched BY NAME just for the
-- button). DEBUG logs every plate's border/blink state to validate the heuristic in-game.

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")

local Skill = {}

local ann = Core.make_announcer()
local host, tick = nil, 0
local last_slot = nil
local last_group, cross_slot = nil, nil

-- Diagnostic: log slot/border/plate state per cursor move to dumps/dump_skill.txt.
local DEBUG = false
local last_dbg = nil
local function dbg(s)
    if not DEBUG or s == last_dbg then return end
    last_dbg = s
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_skill.txt", "a")
        if f then f:write(string.format("[%d] %s\n", os.time(), s)) f:close() end
    end)
end

-- Reflected text read off the host by property name (nil if missing/empty). Guarded.
local function field(name)
    local node
    if not pcall(function() node = host[name] end) or node == nil then return nil end
    return Core.read_text(node)
end

-- Text of a reflected child property on any widget (nil if missing/empty). Guarded.
local function node_text(obj, prop)
    local node
    if not pcall(function() node = obj[prop] end) or node == nil then return nil end
    return Core.read_text(node)
end

-- Visibility of a reflected child property, guarded (false if missing/invalid).
local function node_visible(obj, prop)
    local vis = false
    pcall(function() vis = Core.is_visible(obj[prop]) end)
    return vis
end

local function plate_name(p)
    return node_text(p, "SkillNameTxt")
        or node_text(p, "SkillNameRegistrationTxt")
        or node_text(p, "SuperPassiveNameTxt")
end

local function plate_button(p)
    local glyph
    if not pcall(function() glyph = p.ButtonIconImage end) or glyph == nil then return nil end
    return A.platbtn_name(glyph)
end

-- All live slot plates (0-based ordinal preserved). Bounds-safe TArray walk.
local function plates()
    local out = {}
    local arr, n
    if not pcall(function() arr = host.CategoryPlateCtn end) or arr == nil then return out end
    if not pcall(function() n = arr:GetArrayNum() end) or type(n) ~= "number" then return out end
    for i = 0, n - 1 do
        local p
        if pcall(function() p = arr[i + 1] end) and Core.valid(p) then
            out[#out + 1] = { i = i, p = p }
        end
    end
    return out
end

-- The plate under the cursor: the ONE plate with SelectActiveBorder visible AND
-- BaseBlinkImage hidden. The container also holds STRUCTURAL plates (dump 2026-07-13:
-- ordinals 4 and 7 — nil name, border AND blink permanently on) that would otherwise
-- drown the real cursor: slot plates track the cursor with border on / blink off.
-- Returns plate, ordinal, candidate count (count ~= 1 means the heuristic failed).
local function selected_plate(all)
    local sel, count = nil, 0
    for _, e in ipairs(all) do
        if node_visible(e.p, "SelectActiveBorder")
            and not node_visible(e.p, "BaseBlinkImage") then
            count = count + 1
            if not sel then sel = e end
        end
    end
    if count == 1 then return sel.p, sel.i, count end
    return nil, nil, count
end

-- An unequipped slot renders its name as dashes ("---").
local function is_dashes(s)
    return s == nil or s == "" or s:match("^[%-–—%s]+$") ~= nil
end

function Skill.is_active()
    tick = tick + 1
    host = Core.first_on_screen("Start_Skillcustom_C", tick)
    return host ~= nil
end

function Skill.reset() ann:reset() last_slot = nil last_group = nil cross_slot = nil end

function Skill.update()
    local pane = A.markup_to_speech(field("SkillNameTxt"))
    local all = plates()
    local sel, slot, border_count = selected_plate(all)

    local raw, btn
    if sel then
        raw = plate_name(sel)
        btn = plate_button(sel)
    else
        -- Border heuristic failed: the pane still names populated slots correctly (it only
        -- goes stale on the empty one). Match the plate by name just to recover the button.
        raw = pane
        for _, e in ipairs(all) do
            if A.markup_to_speech(plate_name(e.p)) == pane then
                if btn then btn = nil break end   -- ambiguous (two slots, same skill)
                btn = plate_button(e.p)
            end
        end
    end
    local base = not is_dashes(raw) and A.markup_to_speech(raw) or nil

    if DEBUG then
        local ps = {}
        for _, e in ipairs(all) do
            ps[#ps + 1] = string.format("%d:%s(b%d,k%d,%s)", e.i,
                tostring(plate_name(e.p)),
                node_visible(e.p, "SelectActiveBorder") and 1 or 0,
                node_visible(e.p, "BaseBlinkImage") and 1 or 0,
                tostring(plate_button(e.p)))
        end
        dbg(string.format("slot=%s bc=%d pane=%s | %s",
            tostring(slot), border_count, tostring(pane), table.concat(ps, " ")))
    end

    -- Nothing readable this tick (screen mid-build, or a selected plate whose text hasn't
    -- populated yet): keep quiet rather than guess. Empty slots always carry literal "---",
    -- so requiring non-nil raw never silences a real empty slot.
    if raw == nil then return end

    -- Same spoken text on a different slot (e.g. two empty slots) must still re-announce.
    if slot ~= nil and last_slot ~= nil and slot ~= last_slot then ann:invalidate() end
    if slot ~= nil then last_slot = slot end

    local category = field("CategoryTitleTxt")
    local screen = Core.phrase(field("CharNameTxt"), category)
    screen = screen ~= "" and screen or nil

    -- The plate carousel spans TWO groups read seamlessly (4 main slots + the support/second
    -- group, plates without a button glyph). Announce the game's own group title when the
    -- cursor crosses into the other group. The prefix must stay on the WHOLE announcement of
    -- the crossing row (the announcer diff-gates on the spoken name, so a one-tick prefix
    -- would immediately re-announce the row without it), hence it sticks until the cursor
    -- leaves that row.
    local gpfx = nil
    if sel and slot ~= nil then
        local group = field(btn and "Active1stGroupTitleText" or "Active2ndGroupTitleText")
        if group and group ~= "" then
            if group ~= last_group then
                last_group = group
                cross_slot = slot
            elseif slot ~= cross_slot then
                cross_slot = nil
            end
            if cross_slot == slot then gpfx = group end
        end
    end

    if not base then
        -- Empty slot: "botón A, ranura vacía". (Only reachable via a selected plate — the
        -- pane fallback never yields dashes.)
        ann:focus(screen, category, Core.phrase(gpfx, btn, I18n.t("skill_empty")), nil, nil)
        return
    end

    -- Detail-pane data is attached only while the pane shows THIS skill.
    local value = nil
    if pane == base then
        local lvl = field("SkillLevelTxt")
        local ki = field("SkillSpTxt")
        value = Core.phrase(
            lvl and I18n.t("skill_level"):format(lvl) or nil,
            ki and I18n.t("skill_ki"):format(ki) or nil)
        if value == "" then value = nil end
    end

    -- Lazy tooltip: the announcer retries it for a short window, so a description that
    -- arrives after the pane catches up is still spoken (queued, no interrupt).
    local tip = function()
        if A.markup_to_speech(field("SkillNameTxt")) ~= base then return nil end
        local desc = field("SkillDetailTxt")
        return desc and A.markup_to_speech(desc) or nil
    end

    ann:focus(screen, category, Core.phrase(gpfx, btn, base), value, tip)
end

return Skill
