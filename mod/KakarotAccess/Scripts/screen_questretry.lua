-- Screen adapter: the QUEST RETRY / time-machine story replay menu — the EPISODE list
-- (UAT_UIQuestRetry, AT.hpp:35355; Blueprint Art/UI/Quest_Retry/Quest_Retry, never visited so no
-- BP header exists, hence the NATIVE class name below).
--
-- Owner chain, no scan (ui_directory): AATGameMode.QuestRetryManager @0x4A8 (AATQuestRetryActor)
-- -> MenuManager @0x348 (UATQuestRetryManager) -> UIEpisodeMenu @0x90. The chapter picker sits on
-- the same manager (UIChapterMenu @0x88) and is a plain UAT_UIShopTop, so it is mapped under
-- "Shop_Top_C" and read by screen_shoplist — nothing here touches it.
--
-- Selected row: the list is Xlist_List07_Lay8 (UAT_UIQuestRetryList : UAT_UIMenuListBase00), whose
-- rows are UAT_UIQuestRetryBar (title Txt_List00, episode Txt_List01). The row comes from the
-- reflected GetSelectValue() through the shared A.list_select_index, BOUNDS-CHECKED against
-- ListPlateCtn. That index has been seen FROZEN on other lists of this family, and this screen
-- was coded from headers only (never observed live), so the detail pane — which mirrors the cursor
-- on every screen of this game (screen_story) — is a second, independent signal: its text is part
-- of the change signature, so a frozen index still re-announces when the pane moves.
--
-- Detail pane: Txt_Story_Main / Txt_Story_Detail, the sub-stories (native TArray
-- Txt_Story_Sub_List) and the battles (Txt_List_Battle_List), read on-screen-gated because unused
-- slots are collapsed. Clear/rate icons are NOT read: their meaning is unobserved.

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")

local QuestRetry = {}

local HOST_CLASS = "AT_UIQuestRetry"   -- native name: the live BP subclass has never been seen
local LIST       = "Xlist_List07_Lay8"
local SETTLE_POLLS = 3

-- Deliberately opened full-screen submenu with a manager-owned instance: no boot-flash debounce.
QuestRetry.confirm_ticks = 1

local ann = Core.make_announcer()
local host, tick = nil, 0
local row_text, main_text, detail_text = nil, nil, nil
local seen_key, seen_count = nil, 0
local last_sig = nil
local logged_entry = false

-- i18n keys may land after this file; fall back to English rather than speaking a raw key.
local function tr(key, default)
    local s = I18n.t(key)
    if s == nil or s == key then return default end
    return s
end

local function clean(t) return t and A.markup_to_speech(t) or nil end

local function clear_snapshot()
    host = nil
    row_text, main_text, detail_text = nil, nil, nil
end

local function clear_observation()
    clear_snapshot()
    seen_key, seen_count = nil, 0
    logged_entry = false
end

-- "<title>, <episode>" of the selected list row, or nil. Index validated against the live array
-- count before it is used (an out-of-range TArray index is an uncatchable abort).
local function selected_row_text()
    local list = Core.member(host, LIST)
    local idx = A.list_select_index(list)
    if not idx then return nil end
    local plates, n = Core.array_of(list, "ListPlateCtn")
    if not plates or idx >= n then return nil end
    local row
    if not pcall(function() row = plates[idx + 1] end) or not Core.valid(row) then return nil end
    return clean(Core.phrase(
        clean(Core.read_text(Core.member(row, "Txt_List00"))),
        clean(Core.read_text(Core.member(row, "Txt_List01")))))
end

-- Texts of an on-screen native TArray of text widgets.
local function array_lines(name)
    local out = {}
    local arr, n = Core.array_of(host, name)
    for i = 1, (arr and n or 0) do
        local node
        if pcall(function() node = arr[i] end) and Core.valid(node) and Core.on_screen(node) then
            local t = clean(Core.read_text(node))
            if t then out[#out + 1] = t end
        end
    end
    return out
end

local function detail_lines()
    local parts = {}
    if main_text and main_text ~= row_text then parts[#parts + 1] = main_text end
    if detail_text then parts[#parts + 1] = detail_text end
    local subs = array_lines("Txt_Story_Sub_List")
    if #subs > 0 then
        parts[#parts + 1] = Core.phrase(tr("questretry_substories", "Episodes"), table.concat(subs, ", "))
    end
    local battles = array_lines("Txt_List_Battle_List")
    if #battles > 0 then
        parts[#parts + 1] = Core.phrase(tr("questretry_battles", "Battles"), table.concat(battles, ", "))
    end
    return #parts > 0 and table.concat(parts, ". ") or nil
end

function QuestRetry.is_active()
    tick = tick + 1
    -- COST GATE: a menu can only be open when a menu owns the screen (minimap up = free roam).
    if Core.free_roam(tick) then
        clear_observation()
        return false
    end
    host = Core.first_on_screen(HOST_CLASS, tick)
    -- Pooled interactive pane: genuine Visible(0) ownership AND opacity, so a parked instance
    -- cannot shadow the adapters below it (CLAUDE.md cooking-latch rule).
    if host and not Core.pane_live(host) then host = nil end
    if not host then
        clear_observation()
        return false
    end

    row_text    = selected_row_text()
    main_text   = clean(Core.first_text(host, "Txt_Story_Main"))
    detail_text = clean(Core.first_text(host, "Txt_Story_Detail"))

    -- Claim the tick only on READABLE text: the selected row, else the pane's own title.
    local key = row_text or main_text
    if not key then
        seen_key, seen_count = nil, 0
        return false
    end
    -- Settle: the pane repaints a frame or two behind the cursor.
    local sig = Core.phrase(key, main_text, detail_text)
    if sig ~= seen_key then
        seen_key, seen_count = sig, 1
    elseif seen_count < SETTLE_POLLS then
        seen_count = seen_count + 1
    end
    return true
end

function QuestRetry.update()
    if not host or seen_count < SETTLE_POLLS then return end
    local name = row_text or main_text
    if not name then return end
    if seen_key ~= last_sig then
        -- The pane moved while the row text did not (frozen index, or same title across pages):
        -- force a re-read of name + detail.
        if last_sig ~= nil then ann:invalidate() end
        last_sig = seen_key
        print(string.format("[KakarotAccess] questretry: row=%q main=%q detail=%q\n",
            tostring(row_text), tostring(main_text), tostring(detail_text)))
    end
    if not logged_entry then
        logged_entry = true
        print("[KakarotAccess] screen_questretry: host live\n")
    end
    local caption = clean(Core.first_text(host, "Txt_Cap00")) or tr("questretry_screen", "Time machine")
    ann:focus(caption, nil, name, nil, detail_lines)
end

-- F1: repeat the settled selection; keeps the settle state (unlike reset()).
function QuestRetry.reannounce()
    ann:reset()
    last_sig = nil
end

function QuestRetry.reset()
    ann:reset()
    last_sig = nil
    clear_observation()
end

return QuestRetry
