-- Screen adapter: the Z ENCYCLOPEDIA (the book opened with "Abrir enciclopedia Z" from the
-- overworld ring). One adapter covers all three levels, because they are three DIFFERENT widget
-- classes that share one shape (F7 census + CXXHeaderDump, 2026-07-28):
--
--   INDEX     "Contenidos"    CompZ_Page_Contents00_C  < UAT_UICompZPageContents (AT.hpp:31999)
--   CATEGORY  two-page spread CompZ_Page_Items_L/R_C   < UAT_UICompZPageItems    (AT.hpp:32080)
--   ENTRY     two-page spread CompZ_Page_Img_C         < UAT_UICompZPageImg      (AT.hpp:32069)
--                             CompZ_Page_Detail_R_C    < UAT_UICompZPageDetail   (AT.hpp:32051)
--
-- WHY THIS ONE NEEDED NO GUESSWORK, unlike the other list screens in this mod. The row class
-- `UAT_UICompZList` (AT.hpp:31861) DECLARES its own cursor node — `Canvas_Cursor` @0x3A0 — so the
-- selected row is a reflected read, not a hypothesis about which image the game happens to drive.
-- And the rows are reached through `UAT_UICompZListController.Item_List` @0x0030, which is a REAL
-- `TArray<UAT_UICompZList*>` (size 0x10), not one of this game's fixed C arrays: no collapse, no
-- RegisterCustomProperty, no offsets — just Core.array_of. Both list levels use the same
-- controller, so they share one code path.
--
-- THE BOOK IS NOT IN THE VIEWPORT (round 2, 2026-07-28 — round 1 never activated once, and the
-- log showed nothing at all because nothing errored). `UAT_UICompZPageBase` owns a
-- `RenderTarget` (AT.hpp:31974) which `UCompZMenu.UMGRender` draws onto an `AZCW_BookActor`
-- (AT.hpp:40272-73): the pages are rendered into TEXTURES and mapped onto the 3D book mesh, so
-- they are never parented into the viewport widget tree. Every viewport-based liveness test —
-- `Core.on_screen`, `Core.first_on_screen`, `IsInViewport` — reports false for a page the player
-- is reading. (The F7 census listing their text proved nothing: `discover.lua:830` tests bare
-- `IsVisible()`, not the ancestor walk.) So this adapter uses own-slate-visibility throughout
-- (`Core.first_text_offviewport`, `Core.is_visible`), which is weaker, and earns the screen
-- another way instead: readable text AND a row whose cursor node is showing.
--
-- The two PAGES of a spread are separate host widgets (`_L`/`_R` on the category level, `Img` /
-- `Detail_R` on the entry level). That distinction is irrelevant to us: the cursor lives on
-- whichever page owns it, so rows are gathered from every live page and the marked one wins.
--
-- NODE NAMES — native first, Blueprint twin second, every fetch through Core.member's existence
-- gate (asking a class for an undeclared member is the uncatchable abort):
--   row       TextBox_Label / Txt_List     +  TextBox_Num / Txt_Num   (+ the _DAIMA variants,
--             which are the alternate skin the CollectionTypeSwitcher can select)
--   contents  TextBox_Title / Txt_Title
--   items     TextBox_Contents / Txt_Caterory00  (the category, and yes the game misspells it)
--             TextBox_Items    / Txt_Category01  (the group heading)
--   entry     m_Name/CompZ_Name -> m_Name/Txt_Name, m_Category/CompZ_Temp_L -> m_Text/Txt_Category,
--             m_Detail/CompZ_Detail00 -> m_Title/Txt_Title_Detail, m_Text, m_TextCond/Txt_Cond
--
-- A locked entry has no summary — only `m_TextCond` ("Completa el juego principal…"), which is
-- exactly what a sighted player is shown, so it is read in the summary's place.

local Core = require("ui_core")
local I18n = require("i18n")

local CompZ = {}

-- Checked in this order: a spread opens OVER the level that spawned it, and the pages underneath
-- stay on screen. { blueprint class, native class }
--
-- The NOTE ("nota", A on an entry) is a slip that overlays the entry spread with the detail text
-- still showing behind it, so it is checked FIRST — it is what the player just opened.
-- `CompZ_Memo_C` (F7 census 2026-07-28) < `UAT_UICompZPageMemo` (AT.hpp:32086): `TextBox_Title`
-- @0x468 / `TextBox_Memo` @0x470 natively, `Txt_Title` / `Txt_Detail` in the Blueprint tree — note
-- the BP body node is `Txt_Detail`, NOT the `Txt_Memo` the native name would suggest, which is
-- exactly why both spellings are offered.
-- BLUEPRINT NAMES ONLY. The native fallbacks this list used to carry
-- (`AT_UICompZPageMemo`, `AT_UICompZPageDetail`, …) were never observed — the census names every
-- live host as a `CompZ_*_C` Blueprint — and each speculative name is not free: a class that is
-- never present joins the ABSENT scan set and costs a full `FindAllOf` every ~4 s FOREVER
-- (ui_core's ABSENT_BACKOFF, and its own comment explains this set is kept deliberately small).
-- Eleven names became six, and the six only run when a menu is actually up (see is_active).
local MEMO_PAGES = {
    { "CompZ_Memo_C" },
}
local DETAIL_PAGES = {
    { "CompZ_Page_Detail_R_C" },
    { "CompZ_Page_Img_C" },
}
local LIST_PAGES = {
    { "CompZ_Page_Items_L_C" },
    { "CompZ_Page_Items_R_C" },
    { "CompZ_Page_Contents00_C" },
}

local ann = Core.make_announcer()
local tick = 0
local view = nil          -- what is on screen this tick (computed in is_active)
local context = nil       -- the settled page heading; a change to it re-announces
local warned = false      -- one "rows but no cursor" line per visit, never per tick
-- The heading is a raw engine-derived signal (a text read that returns nil on any transient
-- invisibility) and its edge is expensive — it calls ann:reset(), which re-announces the whole
-- page with interrupt=true. Undebounced, a nil-to-text flap re-speaks everything every poll: the
-- screen_party flip-flop, two days old. Settle it, and log when it fires.
local SETTLE_POLLS = 2
local ctx_want, ctx_count = nil, 0

-- Find a live page of the given class. NOT Core.first_on_screen — see the header: these pages are
-- rendered into a TEXTURE and mapped onto the 3D book, so they are never parented into the
-- viewport and every viewport-based test (on_screen, IsInViewport) reports false for a page the
-- player is reading. Round 1 used first_on_screen and the adapter therefore never activated once:
-- no error, no log line, nothing to see. Own slate visibility plus opacity is all there is here.
local function live(entry)
    for _, cls in ipairs({ entry[1], entry[2] }) do
        for _, o in ipairs(Core.cached_all(cls, tick)) do
            if Core.valid(o) and Core.is_visible(o) and Core.pane_rendered(o) then return o end
        end
    end
    return nil
end

-- The row under the cursor, out of every live list page. nil = no page, or no row marked.
-- Returns the row plus whether ANY row was seen at all, so the caller can tell "the book is not
-- on a list page" from "it is, but the cursor node did not answer".
local function selected_row(pages)
    local any = false
    for _, host in ipairs(pages) do
        local owner = host
        -- The category level nests its list one level down, inside UAT_UICompZItems.
        local items = Core.first_member(host, "UICompZItems", "CompZ_Items")
        if items then owner = items end
        local ctrl = Core.first_member(owner, "ListController")
        if ctrl then
            local arr, n = Core.array_of(ctrl, "Item_List")
            if arr then
                for i = 1, n do
                    local row
                    if pcall(function() row = arr[i] end) and Core.valid(row) then
                        any = true
                        -- is_visible, not on_screen: the row lives on an off-viewport page too.
                        if Core.is_visible(Core.member(row, "Canvas_Cursor")) then
                            return row, true, owner
                        end
                    end
                end
            end
        end
    end
    return nil, any, nil
end

local function row_text(row)
    local label = Core.first_text_offviewport(row, "TextBox_Label", "Txt_List", "TextBox_Label_DAIMA")
    local num   = Core.first_text_offviewport(row, "TextBox_Num", "Txt_Num", "TextBox_Num_DAIMA")
    if not label then return nil end
    -- Read as it sits on the page: the entry, then its collected count ("Historia, 7 de 43").
    return Core.phrase(label, num)
end

-- The heading of whichever list page owns the cursor: "Contenidos", or the category plus its
-- group ("Historia, ¡La batalla por la Tierra contra la invasión saiyajin!").
local function list_heading(owner)
    return Core.phrase(
        Core.first_text_offviewport(owner, "TextBox_Title", "Txt_Title",
                               "TextBox_Contents", "Txt_Caterory00"),
        Core.first_text_offviewport(owner, "TextBox_Items", "Txt_Category01"))
end

-- The note slip. Claimed only when its BODY reads, so an empty/parked slip cannot shadow the
-- entry underneath it.
local function read_memo()
    for _, entry in ipairs(MEMO_PAGES) do
        local host = live(entry)
        if host then
            local body = Core.first_text_offviewport(host, "TextBox_Memo", "Txt_Detail", "Txt_Memo")
            if body then
                return {
                    heading = Core.first_text_offviewport(host, "TextBox_Title", "Txt_Title"),
                    name    = body,
                }
            end
        end
    end
    return nil
end

local function read_detail()
    local name, category, detail
    for _, entry in ipairs(DETAIL_PAGES) do
        local host = live(entry)
        if host then
            name = name or Core.first_text_offviewport(
                Core.first_member(host, "m_Name", "CompZ_Name"), "m_Name", "Txt_Name")
            category = category or Core.first_text_offviewport(
                Core.first_member(host, "m_Category", "CompZ_Temp_L"), "m_Text", "Txt_Category")
            -- Blueprint name FIRST here, against this file's native-first convention, and the
            -- reason is worth keeping: `UAT_UICompZPageDetail.m_Detail` (AT.hpp:32053) is a FIXED
            -- C ARRAY of 3 (size 0x18), which reflection collapses to element 0 — so the native
            -- name can only ever reach one of the three detail parts. `CompZ_Detail00` is the node
            -- the F7 census actually saw carrying the text. NEVER pass "m_Detail" to array_of.
            detail = detail or Core.first_member(host, "CompZ_Detail00", "m_Detail")
        end
    end
    local body = Core.first_text_offviewport(detail, "m_Text", "Txt_Text")
        or Core.first_text_offviewport(detail, "m_TextCond", "Txt_Cond")   -- locked entries show only this
    -- Claim the screen on READABLE TEXT, never on a live handle: a rendered-but-textless detail
    -- page would otherwise commit this adapter and announce nothing but the book's name, while
    -- shadowing every adapter registered below it (save/load, tutorials, community, field).
    if not name and not body then return nil end
    return {
        heading = category,
        name    = name,
        extra   = Core.phrase(Core.first_text_offviewport(detail, "m_Title", "Txt_Title_Detail"), body),
    }
end

local function read_list()
    local pages = {}
    for _, entry in ipairs(LIST_PAGES) do
        local h = live(entry)
        if h then pages[#pages + 1] = h end
    end
    if #pages == 0 then return nil end
    local row, any, owner = selected_row(pages)
    if not row then
        -- Rows exist but none carries the cursor: say nothing rather than announce the wrong
        -- entry, and leave one line in the log so the next report names the cause instead of
        -- costing a round of guessing.
        if any and not warned then
            warned = true
            print("[KakarotAccess] compz: list page with rows but no Canvas_Cursor marked\n")
        end
        return nil
    end
    local text = row_text(row)
    if not text then return nil end
    return { heading = list_heading(owner), name = text }
end

function CompZ.is_active()
    tick = tick + 1
    -- COST GATE (user, 2026-07-28: small stutters after this adapter shipped). The book can only
    -- be open when a menu owns the screen, and `Core.free_roam` is the game's own signal for that
    -- — the minimap is up while walking around and hidden the moment any real menu, battle or
    -- cutscene takes over. It reads a class that is always present, so it is a cached pointer walk
    -- rather than a scan. Without this gate the six page classes are ABSENT the whole time the
    -- player is in the field and each one costs a full FindAllOf every ~4 s; ui_core's own comment
    -- (see SCANS_PER_TICK) records that a cluster of absent-class rescans expiring on the same
    -- tick is exactly what a periodic stutter feels like.
    if Core.free_roam(tick) then view = nil return false end
    view = read_memo() or read_detail() or read_list()
    return view ~= nil
end

function CompZ.reset()
    ann:reset()
    view, context, warned = nil, nil, false
    ctx_want, ctx_count = nil, 0
end

function CompZ.reannounce() ann:reset() end

function CompZ.update()
    local v = view
    if not v then return end
    -- The page heading is context, not an item. Announce it through the screen-entry branch, not
    -- the announcer's `tab` parameter, which speaks the label alone and swallows the entry
    -- underneath it (the screen_party lesson).
    if v.heading ~= ctx_want then
        ctx_want, ctx_count = v.heading, 1
    elseif ctx_count < SETTLE_POLLS then
        ctx_count = ctx_count + 1
    elseif v.heading ~= context then
        print(string.format("[KakarotAccess] compz page -> %s\n", tostring(v.heading)))
        context = v.heading
        ann:reset()
    end
    ann:focus(Core.phrase(I18n.t("compz_title"), v.heading), nil, v.name, v.extra, nil)
end

return CompZ
