-- Screen adapter: the character STATUS page (UAT_UIStartStatus) — the full stats sheet you
-- get by confirming a character in the Characters menu ("PERSONAJES": level, EXP to next,
-- HP/Ki gauges and the five attribute blocks; X opens the skill palette, Y the skill tree).
--
-- Everything on it is REFLECTED on the native class (AT.hpp, UAT_UIStartStatus):
--   WL_Txt_Name / WL_Txt_Lv                 — character, level
--   WL_Txt_Title / WL_Txt_Num               — the EXP-to-next caption + value ("Next", 1212)
--   WL_Txt_Power / WL_Txt_Power_Num         — the BP caption + value
--   WL_Start_Char_Hud_Hp / …_Sp             — UAT_UIStartStatusHud: the gauge (HpGauge /
--       SpGauge → TextBox_Number, "19591/19591") plus its breakdown rows in two parallel
--       TArrays, TxtStatus (labels) / TxtStatusNum (values)
--   the five attribute blocks               — UAT_UIStartStatusList01: WL_Txt_Power(+_Num) is
--       the TOTAL row, TxtStatusList / TxtStatusNumList the breakdown rows
--
-- WL_Start_Char_List01 (the blocks) is a fixed C array, and UE4SS COLLAPSES those to a single
-- object — indexing or GetArrayNum on one ABORTS the game (the WL_StartCharBarList lesson in
-- screen_characters). So the blocks are taken from the cached instance list of their own class,
-- kept to the on-screen ones and ordered by where they actually sit (Core.slot_pos), with the
-- collapsed member as the fallback if the class can't be found at all.
--
-- Read shape (user spec, 2026-07-14): entering speaks the HEADER (name, level, next, HP, Ki,
-- BP) — reading all ~25 values on entry would be half a minute of speech — and the blocks are
-- then walked one at a time, each as TOTAL + breakdown, with D-PAD down/up on the gamepad
-- (F11 / Shift+F11 do the same from the keyboard). The page itself uses the right stick
-- (rotate) and X/Y/B, so the d-pad is free here; the pad is only READ, never blocked.

local Core = require("ui_core")
local I18n = require("i18n")
local Speech = require("speech")
local Input = require("input")
local Registry = require("ui_registry")
local Transition = require("transition")

local Status = {}

local CLASS = "AT_UIStartStatus"
local BLOCK_CLASS = "AT_UIStartStatusList01"

local ann = Core.make_announcer()
local host, tick = nil, 0
local cursor = 0   -- 0 = nothing walked yet; 1..#blocks = the block last read

-- ---- reading ---------------------------------------------------------------

-- A reflected member of the current screen, as text (nil if absent/empty/unreadable).
local function field(obj, name)
    local node
    if not pcall(function() node = obj[name] end) or node == nil then return nil end
    return Core.read_text(node)
end

-- The gauges render as "19591/19591", which a screen reader says as a date or a fraction.
local function say_gauge(t)
    if not t then return nil end
    local cur, max = t:match("^%s*([%d%.,]+)%s*/%s*([%d%.,]+)%s*$")
    if cur and max then return I18n.t("gauge_fmt"):format(cur, max) end
    return t
end

-- The breakdown rows of a block: two parallel TArrays (labels, values) → {"PS base 19541",
-- "Aumento de estado +50", …}. Both are real TArrays here (unlike the collapsed C arrays), so
-- they can be indexed — but only through Core.array_of, which validates the array before asking
-- its length (a raw GetArrayNum on an invalid one is the uncatchable throw that killed the game
-- on 2026-07-14). Hidden rows are skipped: the widgets are pooled and a collapsed row keeps its
-- last text.
local function rows_of(obj, labels_name, values_name)
    local labels, n = Core.array_of(obj, labels_name)
    if not labels then return {} end
    local values, vn = Core.array_of(obj, values_name)
    vn = values and vn or 0
    local out = {}
    for i = 1, n do
        local lab, val
        pcall(function() lab = labels[i] end)
        if i <= vn then pcall(function() val = values[i] end) end
        local label = Core.read_text(lab)
        if label and Core.on_screen(lab) then
            out[#out + 1] = Core.phrase(label, Core.read_text(val))
        end
    end
    return out
end

local function hud_of(member)
    local hud
    if not pcall(function() hud = host[member] end) or not Core.valid(hud) then return nil end
    return hud
end

-- The gauge value of an HP/Ki HUD. Each HUD declares both gauge members but fills only its
-- own, so the first one that reads is the right one.
local function hud_gauge(hud)
    for _, g in ipairs({ "HpGauge", "SpGauge" }) do
        local gauge
        if pcall(function() gauge = hud[g] end) and Core.valid(gauge) then
            local t = say_gauge(field(gauge, "TextBox_Number"))
            if t then return t end
        end
    end
    return nil
end

-- An HP/Ki block: the gauge is the "total", the TxtStatus rows the breakdown.
local function hud_block(member)
    local hud = hud_of(member)
    if not hud then return nil end
    local total = hud_gauge(hud)
    local rows = rows_of(hud, "TxtStatus", "TxtStatusNum")
    if not total and #rows == 0 then return nil end
    return { total = total, rows = rows }
end

-- The five attribute blocks, in the order they appear on screen.
local function stat_widgets()
    local out = {}
    for _, w in ipairs(Core.cached_all(BLOCK_CLASS, tick)) do
        if Core.on_screen(w) then out[#out + 1] = w end
    end
    if #out == 0 then
        -- Class not found at all: fall back to the collapsed member (the first block only).
        local w
        if pcall(function() w = host.WL_Start_Char_List01 end) and Core.valid(w) then out[1] = w end
        return out
    end
    -- Construction order is not screen order. Sort by the block's own vertical placement, and
    -- only if EVERY block knows where it is — a partial sort would interleave placed and
    -- unplaced blocks into an order matching nothing (the keyhelp-bar lesson).
    local y, placed = {}, true
    for _, w in ipairs(out) do
        y[w] = Core.slot_pos(w, "Y")
        if not y[w] then placed = false end
    end
    if placed then table.sort(out, function(a, b) return y[a] < y[b] end) end
    return out
end

local function list_block(w)
    local total = Core.phrase(field(w, "WL_Txt_Power"), field(w, "WL_Txt_Power_Num"))
    local rows = rows_of(w, "TxtStatusList", "TxtStatusNumList")
    if total == "" and #rows == 0 then return nil end
    return { total = total ~= "" and total or nil, rows = rows }
end

-- Every block the page shows, in screen order: HP, Ki, then the five attributes.
local function blocks()
    local out = {}
    for _, m in ipairs({ "WL_Start_Char_Hud_Hp", "WL_Start_Char_Hud_Sp" }) do
        local b = hud_block(m)
        if b then out[#out + 1] = b end
    end
    for _, w in ipairs(stat_widgets()) do
        local b = list_block(w)
        if b then out[#out + 1] = b end
    end
    return out
end

-- The entry readout: who this is and the totals. nil while the page is still filling in — the
-- widgets populate over a frame or two, and announcing a half-built header would speak twice.
local function header()
    local name, power = field(host, "WL_Txt_Name"), field(host, "WL_Txt_Power_Num")
    if not name or not power then return nil end
    local hp, sp = hud_of("WL_Start_Char_Hud_Hp"), hud_of("WL_Start_Char_Hud_Sp")
    return Core.phrase(
        name,
        field(host, "WL_Txt_Lv"),                                              -- "Nvl. 9"
        Core.phrase(field(host, "WL_Txt_Title"), field(host, "WL_Txt_Num")),   -- "Next 1212"
        hp and hud_gauge(hp) or nil,
        sp and hud_gauge(sp) or nil,
        Core.phrase(field(host, "WL_Txt_Power"), power))                       -- "BP 3094"
end

-- ---- adapter ---------------------------------------------------------------

function Status.is_active()
    tick = tick + 1
    host = Core.first_on_screen(CLASS, tick)
    return host ~= nil
end

function Status.reset()
    ann:reset()
    cursor = 0
end

function Status.update()
    local h = header()
    if not h then return end
    ann:focus(nil, nil, h, nil, nil)
end

-- Step to the next/previous stat block and read it (total + breakdown). Wraps around. Driven
-- by the d-pad (below) and by F11 / Shift+F11. Silent when the page isn't up — both belong to
-- this screen only.
function Status.step(dir)
    if not (Core.valid(host) and Core.on_screen(host)) then return end
    local bs = blocks()
    if #bs == 0 then return end
    cursor = cursor + dir
    if cursor < 1 then cursor = #bs elseif cursor > #bs then cursor = 1 end
    local b = bs[cursor]
    Speech.say(Core.phrase(b.total, table.concat(b.rows, ", "),
        I18n.t("status_pos"):format(cursor, #bs)), true)
end

-- ---- gamepad ---------------------------------------------------------------
-- A short loop of its own, like the map's travel d-pad: the registry polls at 100 ms, which
-- is coarse enough to miss a quick tap. It only READS the pad (input_bridge), so the game
-- keeps its own controls — nothing is blocked and nothing can be stranded blocked.
--
-- Gated on OWNING the screen (Registry.active_adapter): the skill palette and the skill tree
-- open OVER this page, which stays on_screen underneath, so the host check alone would keep
-- the d-pad talking while the player is really in one of those.

local PAD_TICK_MS = 20
local pad_running = false
local pad_prev = 0        -- button mask last seen, for edge detection

local function pad_step()
    if Transition.active() or Registry.active_adapter() ~= Status then pad_prev = 0 return end
    local snap = Input.read()
    if not snap then pad_prev = 0 return end
    local B = Input.BTN
    local function pressed(m) return (snap.buttons & m) ~= 0 and (pad_prev & m) == 0 end
    if pressed(B.DPAD_DOWN) then
        Status.step(1)
    elseif pressed(B.DPAD_UP) then
        Status.step(-1)
    end
    pad_prev = snap.buttons
end

function Status.start()
    if pad_running then return end
    if not Input.is_loaded() then
        print("[KakarotAccess] screen_status: input bridge not loaded, stat-block d-pad disabled\n")
        return
    end
    pad_running = true
    _G.__KakarotStatusPadGen = (_G.__KakarotStatusPadGen or 0) + 1
    local myGen = _G.__KakarotStatusPadGen
    local busy = false
    LoopAsync(PAD_TICK_MS, function()
        if _G.__KakarotStatusPadGen ~= myGen then return true end
        if not busy then
            busy = true
            ExecuteInGameThread(function()
                -- busy cleared on ENTRY (radar_menu/screen_map lesson): an uncatchable C++
                -- abort in here must not leave the guard stuck and the d-pad dead all session.
                busy = false
                local ok, err = pcall(pad_step)
                if not ok then
                    print("[KakarotAccess] screen_status pad step error: " .. tostring(err) .. "\n")
                end
            end)
        end
        return false
    end)
end

function Status.stop()
    pad_running = false
    _G.__KakarotStatusPadGen = (_G.__KakarotStatusPadGen or 0) + 1
end

return Status
