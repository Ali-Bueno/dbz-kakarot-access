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

-- Deliberately opened sub-screen: one registry confirmation tick suffices (the global
-- CONFIRM_TICKS=2 exists for screens that FLASH at boot) — shaves ~100 ms off entry.
Skill.confirm_ticks = 1

local ann = Core.make_announcer()
local host, tick = nil, 0
local last_slot, last_idx = nil, nil
local last_group, cross_slot = nil, nil
local assign_mode, last_mode = false, nil
local SETTLE_TICKS = 3            -- entry debounce: slot must repeat this many polls
local settle_slot, settle_n = nil, 0

-- Diagnostic: log slot/border/plate/mode state per cursor move to dumps/dump_skill.txt.
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
    -- Core.array_of, never a raw GetArrayNum: an invalid array here is an UNCATCHABLE C++ throw
    -- that killed the game (2026-07-14) — see the note on Core.array_of.
    local arr, n = Core.array_of(host, "CategoryPlateCtn")
    if not arr then return out end
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

local host_seen = false
function Skill.is_active()
    tick = tick + 1
    host = Core.first_on_screen("Start_Skillcustom_C", tick)
    if DEBUG and (host ~= nil) ~= host_seen then
        dbg(host ~= nil and "HOST on" or "HOST off")
    end
    host_seen = host ~= nil
    return host_seen
end

function Skill.reset()
    if DEBUG then dbg("RESET") end
    ann:reset()
    last_slot, last_idx, last_group, cross_slot = nil, nil, nil, nil
    assign_mode, last_mode = false, nil
    settle_slot, settle_n = nil, 0
end

-- Level + Ki cost from the detail pane, ONLY while the pane names `base` (it lags the
-- cursor and goes stale on empty slots — never let it mix another skill's data in).
local function pane_value(pane, base)
    if pane ~= base then return nil end
    local lvl = field("SkillLevelTxt")
    local ki = field("SkillSpTxt")
    local v = Core.phrase(
        lvl and I18n.t("skill_level"):format(lvl) or nil,
        ki and I18n.t("skill_ki"):format(ki) or nil)
    return v ~= "" and v or nil
end

-- Lazy tooltip (the announcer retries it briefly): the description, same pane gate.
local function pane_tip(base)
    return function()
        if A.markup_to_speech(field("SkillNameTxt")) ~= base then return nil end
        local desc = field("SkillDetailTxt")
        return desc and A.markup_to_speech(desc) or nil
    end
end

function Skill.update()
    local pane = A.markup_to_speech(field("SkillNameTxt"))

    local list
    pcall(function() list = host.SkillListMenu end)
    local idx = A.list_select_index(list)
    local all = plates()
    local sel, slot, border_count = selected_plate(all)

    -- MODE. Pressing A on a slot moves the focus into the acquired-skill list
    -- (SkillListMenu) to pick the attack. There is NO static signal for which side has
    -- the focus (dump 2026-07-13): the list is a permanent panel, its AllCurs highlight
    -- renders in both modes, and the slot plate keeps its border while assigning. What
    -- discriminates is which cursor MOVES: the plate border sweeps in slot mode with
    -- GetSelectValue frozen, and GetSelectValue comes ALIVE in assign mode with the
    -- plate frozen. So the mode follows whichever cursor moved last.
    local slot_moved = slot ~= nil and last_slot ~= nil and slot ~= last_slot
    local idx_moved = idx ~= nil and last_idx ~= nil and idx ~= last_idx
    if slot ~= nil then last_slot = slot end
    if idx ~= nil then last_idx = idx end
    if slot_moved then assign_mode = false end
    if idx_moved then assign_mode = true end
    if last_mode ~= nil and assign_mode ~= last_mode then
        -- Full context switch: re-open the announcer so entering speaks the "choose
        -- skill" prompt and returning re-orients with character/category/slot.
        ann:reset()
        last_group, cross_slot = nil, nil
    end
    last_mode = assign_mode

    if assign_mode then
        local row = A.list_selected_row(list)
        local rowname = row and row.name and A.markup_to_speech(row.name) or nil

        if DEBUG then
            dbg(string.format("ASSIGN o=%d idx=%s row=%s pane=%s slot=%s bc=%d",
                ann.open and 1 or 0, tostring(idx), tostring(row and row.name),
                tostring(pane), tostring(slot), border_count))
        end

        -- The row is cursor-synchronous (indexed by the live GetSelectValue); the pane
        -- lags a beat behind it, so it only supplies the gated level/Ki/description.
        local base = rowname or pane
        if base == nil or is_dashes(base) then return end
        -- Two rows may carry the same skill name — force the re-announce on index move.
        if idx_moved then ann:invalidate() end
        ann:focus(I18n.t("skill_assign"), nil, base, pane_value(pane, base), pane_tip(base))
        return
    end

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
        dbg(string.format("SLOT o=%d slot=%s bc=%d idx=%s pane=%s | %s",
            ann.open and 1 or 0, tostring(slot), border_count, tostring(idx),
            tostring(pane), table.concat(ps, " ")))
    end

    -- Nothing readable this tick (screen mid-build, or a selected plate whose text hasn't
    -- populated yet): keep quiet rather than guess. Empty slots always carry literal "---",
    -- so requiring non-nil raw never silences a real empty slot.
    if raw == nil then return end

    -- Same spoken text on a different slot (e.g. two empty slots) must still re-announce.
    if slot_moved then ann:invalidate() end

    local category = field("CategoryTitleTxt")
    local screen = Core.phrase(field("CharNameTxt"), category)
    screen = screen ~= "" and screen or nil

    -- The plate carousel spans TWO groups read seamlessly (4 main slots + the support/second
    -- group, plates without a button glyph). Announce the game's own group title when the
    -- cursor crosses into the other group. The prefix must stay on the WHOLE announcement of
    -- the crossing row (the announcer diff-gates on the spoken name, so a one-tick prefix
    -- would immediately re-announce the row without it), hence it sticks until the cursor
    -- leaves that row.
    -- ENTRY SETTLE: for the FIRST announcement after a reset, the plate border (and the
    -- whole pooled widget) can still hold a STALE previous state during the opening
    -- animation — the speech capture 2026-07-13 shows an entry announcing a leftover
    -- slot, then the border landing on the real one ~0.6 s later, whose re-announce CUT
    -- the entry phrase (perceived as "menus don't announce on entry"). Hold the first
    -- announcement until the slot read repeats SETTLE_TICKS consecutive polls.
    if not ann.open then
        if slot ~= nil and slot == settle_slot then settle_n = settle_n + 1
        else settle_slot, settle_n = slot, 1 end
        if settle_n < SETTLE_TICKS then return end
    end

    local gpfx = nil
    if sel and slot ~= nil then
        local group = field(btn and "Active1stGroupTitleText" or "Active2ndGroupTitleText")
        -- The group title can EQUAL the category tab text (speech capture 2026-07-13:
        -- "Súper Ataque, Súper Ataque") — skip the prefix when it adds nothing.
        if group == category then group = nil end
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

    ann:focus(screen, category, Core.phrase(gpfx, btn, base), pane_value(pane, base), pane_tip(base))
end

return Skill
