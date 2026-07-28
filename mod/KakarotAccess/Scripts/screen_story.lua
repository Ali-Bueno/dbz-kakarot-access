-- Screen adapter: overworld STORY menu (Start_Quest_C -> UAT_UIStartQuest, AT.hpp:36894).
--
-- A tab strip over a scrolling quest list on the left, and a right-hand pane with TWO MODES that
-- X toggles ("Mostrar progreso de la historia" in the key help). The game names the modes itself:
-- `All_Win_Story` @0x3A0 and `All_Win_Task` @0x3A8 (both UBorder), so which one is up is a read,
-- not a guess.
--   * STORY mode — kind, title, recommended level, summary, completion rewards.
--   * TASK mode  — the progress checklist: Txt_Story_Task00..12 (AT.hpp:36916-36928).
--
-- WHY THIS READS THE DETAIL PANE AND NOT THE LIST. The rows are `Xlist_List06_00..06` inside
-- `Xlist_List06_Lay9` (F7 census 2026-07-28), a VIRTUALISED window over a longer list — so slot
-- position is not the ordinal — and nothing in the hierarchy exposes a cursor index for it. The
-- pane, however, is a direct mirror of the cursor: move the selection and `Txt_Quest_title`
-- changes with it. So the selection is read for free, with no index, no marker guess and no
-- scroll arithmetic (the screen_skillcustom trick). The row widgets are never touched.
--
-- Rewards come from the NATIVE list `UIRewardBar_List` @0x6D0 — a real TArray of UAT_UIRewardBar
-- (AT.hpp:36998 / 35551) — rather than the Blueprint node names the census showed
-- (`Xlist_Reward_Bar00..05`): no name guessing, no fixed-array hazard, and correct however many
-- rows a quest awards.
--
-- A locked entry reads "¿?" in the game's own text, which is what a sighted player sees.

local Core = require("ui_core")
local I18n = require("i18n")

local Story = {}

local HOST_BP     = "Start_Quest_C"
local HOST_NATIVE = "AT_UIStartQuest"
local TASK_ROWS   = 13         -- Txt_Story_Task00..12, AT.hpp:36916-36928
-- The pane repaints a frame or two behind the cursor, and the SUMMARY lands a little after the
-- title does (user, 2026-07-28: "alguna que otra está sin leer el resumen"). Three polls covers
-- the gap and still feels immediate at a 100 ms registry tick.
local SETTLE_POLLS = 3

local ann = Core.make_announcer()
local host, tick = nil, 0
local seen_key, seen_count = nil, 0
-- The context line is tab + pane mode. It is a raw engine-derived signal, and its edge is
-- expensive: it calls ann:reset(), which makes the next focus() re-announce everything with
-- interrupt=true. Undebounced, a flap re-speaks the screen every poll (the screen_party
-- flip-flop). Settled, and logged when it fires.
local ctx, ctx_want, ctx_count = nil, nil, 0

local function task_mode()
    return Core.is_visible(Core.member(host, "All_Win_Task"))
end

-- The progress checklist (X mode). nil when that pane is not up or has no rows.
local function task_lines()
    local lines = {}
    for i = 0, TASK_ROWS - 1 do
        local t = Core.first_text(host, string.format("Txt_Story_Task%02d", i))
        if t then lines[#lines + 1] = t end
    end
    if #lines == 0 then return nil end
    return table.concat(lines, ". ")
end

local function detail_title()
    return Core.first_text(host, "Txt_Quest_title", "Txt_Quest_title01")
end

-- Everything the story pane says about the selected entry, spoken after the title.
local function detail_extra()
    local out = {}
    local summary = Core.first_text(host, "Txt_Detail", "Txt_Detail00")
    if summary then
        out[#out + 1] = Core.phrase(Core.first_text(host, "Txt_Cap02"), summary)
    end
    local rewards = {}
    local arr, n = Core.array_of(host, "UIRewardBar_List")
    for i = 1, (arr and n or 0) do
        local bar
        if pcall(function() bar = arr[i] end) and Core.valid(bar) and Core.on_screen(bar) then
            -- Read as it sits on screen: the name, then the amount.
            local line = Core.phrase(Core.first_text(bar, "TextBox_Reward", "Txt_Reward"),
                                     Core.first_text(bar, "TextBox_Num", "Txt_Num"))
            if line and line ~= "" then rewards[#rewards + 1] = line end
        end
    end
    if #rewards > 0 then
        out[#out + 1] = Core.phrase(Core.first_text(host, "Txt_Reward"), table.concat(rewards, ", "))
    end
    if #out == 0 then return nil end
    return table.concat(out, ". ")
end

function Story.is_active()
    tick = tick + 1
    host = Core.first_on_screen(HOST_BP, tick) or Core.first_on_screen(HOST_NATIVE, tick)
    -- pane_rendered, not pane_live: this host is pooled and lingers rendered after closing, so it
    -- needs a liveness gate (CLAUDE.md's cooking-latch rule) — but pane_live also demands
    -- ESlateVisibility Visible(0), which is what held screen_questreward silent forever. Opacity
    -- only; on_screen above has already dropped Collapsed/Hidden.
    if host and not Core.pane_rendered(host) then host = nil end
    if not host then
        seen_key, seen_count = nil, 0
        return false
    end
    -- Settle whichever pane is up before believing it (see SETTLE_POLLS).
    local key = task_mode() and task_lines() or detail_title()
    if key ~= seen_key then
        seen_key, seen_count = key, 1
    elseif seen_count < SETTLE_POLLS then
        seen_count = seen_count + 1
    end
    return key ~= nil
end

function Story.reset()
    ann:reset()
    seen_key, seen_count = nil, 0
    ctx, ctx_want, ctx_count = nil, nil, 0
end

function Story.reannounce() ann:reset() end

function Story.update()
    if seen_count < SETTLE_POLLS or not seen_key then return end
    local task = task_mode()

    -- Tab and pane mode are CONTEXT, not items: either changing changes the whole readout.
    -- Announce them THROUGH the screen-entry branch rather than the announcer's `tab` parameter,
    -- which speaks the label alone and would swallow the entry underneath it (screen_party).
    local now_ctx = Core.phrase(I18n.startlist(4),
        Core.first_text(host, "Txt_Cap01"),
        task and Core.phrase(Core.first_text(host, "Txt_Cap03", "Txt_Cap04"),
                             Core.first_text(host, "Txt_Progress")) or nil)
    if now_ctx ~= ctx_want then
        ctx_want, ctx_count = now_ctx, 1
    elseif ctx_count < SETTLE_POLLS then
        ctx_count = ctx_count + 1
    elseif now_ctx ~= ctx then
        print(string.format("[KakarotAccess] story context -> %s\n", tostring(now_ctx)))
        ctx = now_ctx
        ann:reset()
    end

    if task then
        -- The checklist IS the readout; there is no separate title to head it.
        ann:focus(ctx or now_ctx, nil, seen_key, nil, nil)
        return
    end
    ann:focus(ctx or now_ctx, nil, seen_key,
        Core.phrase(Core.first_text(host, "Txt_Cap00"),
                    Core.phrase(Core.first_text(host, "Txt_Recommend"),
                                Core.first_text(host, "Txt_Num_Lv"))),
        detail_extra)
end

return Story
