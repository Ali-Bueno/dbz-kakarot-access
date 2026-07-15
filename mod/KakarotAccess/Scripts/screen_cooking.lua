-- Screen adapter: cooking menu (Shop_Cook_C -> UAT_UICookingMenu; campfire / cook NPCs).
--
-- SELECTION (revised after the 2026-07-03 live test): the recipe list
-- (CookMenuList.WL_Shop_Cmn_List -> UAT_UIMenuListBase00) exposes a reflected
-- GetSelectValue(), but in-game it FROZE after entry (the entry recipe spoke, cursor
-- moves spoke nothing) — like the pause/overworld menus, the live cursor apparently
-- sits in the class's non-reflected tail (0x450..0x4A0). So the selected recipe is
-- read from the DETAIL PANE instead, which the game repopulates on every cursor move:
--   * name    CookMenuList.WL_CookWin_Cap_Title (the dish title of the right-hand
--             window; its sibling members are that window's icon/rarity, while the
--             static captions "Meal Effect"/"Status Boost"/... have their own Cap_*
--             members — confirmed in dump_1782938804_003). Falls back to the
--             ListPlateCtn row if unreadable.
--   * a selection SIGNATURE (title + first effect + description) invalidates the
--             announcer whenever the pane is rewritten without the spoken name
--             changing, so a stale name source can never silence the reader.
-- Detail pane (the recipe's "tooltip", crafting rules in
-- reference/ui-accessibility/inventories.md): effect lines Txt_Detail00_01..03,
-- perpetual buff Txt_Detail01_01..03, duration Txt_Detail02, ingredients
-- Shop_Cmn_Bar_01_00.. (Xlist_Bar00_C rows: Txt_List + Txt_Num = have/needed),
-- description Txt_Detail03 last. All spoken text goes through A.markup_to_speech:
-- the reflected GetText() returns RAW markup ('<span color="#…">Ki ATK</>') that the
-- screen reader would otherwise read out loud.
-- The genre/category title (CookMenuList.WL_Txt_GenreTitle) is the announcer's tab
-- (LB/RB speaks the new genre). The cooking-complete banner and the result window
-- (WL_CookingComp.WL_Text / WL_CookingResult.TextBoxCtn) are spoken diff-gated when
-- they appear over the menu.

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")
local Speech = require("speech")

local Cooking = {}

-- Appends a line per selection change to dumps/dump_cooking.txt (GetSelectValue vs
-- the detail-pane title) — ground truth for the frozen-index diagnosis. Turn OFF
-- once the user confirms cursor moves are spoken.
local DEBUG = false

-- Entry-confirm debounce (ui_registry): at a cook NPC the pooled pane stays visible
-- through the EXIT animation, so cooking briefly becomes the sweep winner right after
-- "Salir" — 5 consecutive polls (~0.5 s) must confirm it before it may speak, which a
-- transient exit blip never survives (the dialogue takes over first).
local ann = Core.make_announcer()
local host, shoplist, list, overlay = nil, nil, nil, nil
local last_overlay, last_sig = nil, nil
local spoken_key = nil   -- last spoken selection key. NOT cleared by reset(): while
                         -- the pane lingers at a cook NPC the registry flip-flops
                         -- between the dialogue and this adapter, and every flip used
                         -- to re-announce "Cocina, <last dish>" (user 2026-07-15
                         -- evening). Cleared on genuine close (host gone / free-roam /
                         -- the entry menu up) so a real re-entry announces fresh.
local overlay_spoken = nil   -- notice-RELEASE (see screen_dialog): NOT cleared by
                             -- reset(). A pooled complete/result banner lingering
                             -- after leaving the campfire must not hold this adapter
                             -- active forever — it is spoken ONCE, then releases the
                             -- tick to the screens below (the mute ring pause,
                             -- Ctrl+F5 adapter_index=17, 2026-07-06).
local tick = 0

-- Rich text -> speech (drop <span color=…>/</>, resolve <inputicon>); nil if empty.
local function clean(t) return t and A.markup_to_speech(t) or nil end

-- Detail-pane text members in on-screen (top-to-bottom) order; description handled
-- separately so the long text always comes last.
local DETAIL_MEMBERS = {
    "Txt_Detail00_01", "Txt_Detail00_02", "Txt_Detail00_03",   -- effect lines
    "Txt_Detail01_01", "Txt_Detail01_02", "Txt_Detail01_03",   -- perpetual (permanent) buff lines
    "Txt_Detail02",                                            -- effect duration
}
local DESCRIPTION_MEMBER = "Txt_Detail03"

-- Ingredient rows: contiguous blueprint members Shop_Cmn_Bar_01_00, _01, … — probe
-- until the first missing one instead of hardcoding how many the blueprint has.
local function ingredient_parts()
    local parts, i = {}, 0
    while true do
        local bar = host[string.format("Shop_Cmn_Bar_01_%02d", i)]
        if not Core.valid(bar) then break end
        if Core.on_screen(bar) then
            local p = Core.phrase(clean(Core.read_text(bar.Txt_List)),
                                  clean(Core.read_text(bar.Txt_Num)))
            if p ~= "" then parts[#parts + 1] = p end
        end
        i = i + 1
    end
    return parts
end

-- The selected recipe's full detail readout (lazy: the announcer calls this only when
-- the selection actually changes).
local function detail()
    local parts = {}
    for _, m in ipairs(DETAIL_MEMBERS) do
        local node = host[m]
        if Core.on_screen(node) then
            local t = clean(Core.read_text(node))
            if t then parts[#parts + 1] = t end
        end
    end
    for _, p in ipairs(ingredient_parts()) do parts[#parts + 1] = p end
    local desc = host[DESCRIPTION_MEMBER]
    if Core.on_screen(desc) then
        local t = clean(Core.read_text(desc))
        if t then parts[#parts + 1] = t end
    end
    if #parts == 0 then return nil end
    return table.concat(parts, ", ")
end

-- A text member of the CookMenuList controller (a plain UObject, so member access is
-- pcall-guarded), on-screen gated, raw (uncleaned). nil if unreadable.
local function shoplist_text(member)
    if not Core.valid(shoplist) then return nil end
    local t
    pcall(function()
        local node = shoplist[member]
        if Core.on_screen(node) then t = Core.read_text(node) end
    end)
    return t
end

-- The selected dish's name from the detail pane — follows the cursor even while the
-- list's GetSelectValue stays frozen.
local function title() return clean(shoplist_text("WL_CookWin_Cap_Title")) end

-- The current genre/category title, used as the announcer's tab (spoken on change).
local function genre() return clean(shoplist_text("WL_Txt_GenreTitle")) end

-- Signature of the current selection: texts the game rewrites on every cursor move.
-- Raw (uncleaned) — only compared, never spoken.
local function selection_sig()
    local parts = { shoplist_text("WL_CookWin_Cap_Title") }
    if Core.valid(host) then
        parts[#parts + 1] = Core.read_text(host.Txt_Detail00_01)
        parts[#parts + 1] = Core.read_text(host[DESCRIPTION_MEMBER])
    end
    return Core.phrase(table.unpack(parts))
end

-- Text of whichever cooking overlay is up (complete banner / result window), or nil.
local function overlay_text()
    local comp
    pcall(function() comp = host.WL_CookingComp end)
    if Core.valid(comp) and Core.on_screen(comp) then
        local t = clean(Core.read_text(comp.WL_Text))
        if t then return t end
    end
    local res
    pcall(function() res = host.WL_CookingResult end)
    if Core.valid(res) and Core.on_screen(res) then
        local parts = {}
        local boxes, n = Core.array_of(res, "TextBoxCtn")
        if boxes then pcall(function()
            for i = 1, n do
                local b = boxes[i]
                if Core.on_screen(b) then
                    local t = clean(Core.read_text(b))
                    if t then parts[#parts + 1] = t end
                end
            end
        end) end
        if #parts > 0 then return table.concat(parts, ", ") end
    end
    return nil
end

local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    return dir .. "\\dumps\\dump_cooking.txt"
end

local function dump_sample(sig, row)
    local f = io.open(dump_path(), "a")
    if not f then return end
    f:write(string.format("[%d] idx=%s row=%s title=%s sig=%s\n",
        os.time(), tostring(A.list_select_index(list)),
        tostring(row and row.name), tostring(shoplist_text("WL_CookWin_Cap_Title")),
        (sig or ""):sub(1, 160)))
    f:close()
end

Cooking.confirm_ticks = 5   -- see the debounce note above

-- "Genuinely live" test for the pooled pane: Core.pane_live (visibility + opacity —
-- the root fix for the lingering-pane family; VERIFIED in-game on this screen
-- 2026-07-15 night, and now the CLAUDE.md §8 rule shared via ui_core).

-- One line per ACTIVATION (first announce after each reset) to dumps/dump_cooking.txt:
-- if the stale pane still sneaks in somewhere, this names the state that let it
-- through (visibility / opacity / where). Turn OFF with the latch verified dead.
local LATCH_DEBUG = true
local latch_logged = false

local function latch_log(name)
    if latch_logged then return end
    latch_logged = true
    local vis, op = "?", "?"
    pcall(function() vis = tostring(host:GetVisibility()) end)
    pcall(function() op = tostring(host:GetRenderOpacity()) end)
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    local f = io.open(dir .. "\\dumps\\dump_cooking.txt", "a")
    if not f then return end
    f:write(string.format("[latch %s] spoke=%s vis=%s opacity=%s free_roam=%s ring=%s\n",
        os.date("%H:%M:%S"), tostring(name), vis, op,
        tostring(Core.free_roam(tick)), tostring(Core.ring_open(tick) ~= nil)))
    f:close()
end

function Cooking.is_active()
    tick = tick + 1
    host = Core.cached_live("Shop_Cook_C", tick)   -- pooled: cheap cached ref per tick
    if not Core.valid(host) then overlay = nil spoken_key = nil return false end
    -- The complete/result overlays can be up while the menu body is hidden (the cook
    -- demo collapses it), so they keep the adapter active on their own — but only
    -- until spoken once (overlay_spoken): a stale banner must not hold the screen.
    overlay = overlay_text()
    if overlay and overlay ~= overlay_spoken then return true end
    overlay = nil
    if not Core.on_screen(host) then spoken_key = nil return false end
    if not Core.pane_live(host) then spoken_key = nil return false end
    -- Free-roam cross-check (user bug 2026-07-15 evening): at a cook NPC the pooled
    -- cooking pane stays VISIBLE with its last dish after leaving the menu, so the
    -- live-detail gate below can't release — the adapter latched "Cocina, <dish>" and
    -- shadowed the ring pause. The minimap is the game's own "menu closed" signal.
    if Core.free_roam(tick) then spoken_key = nil return false end
    -- The overworld RING (Start_Top_C) and its whole submenu family (items, skill
    -- tree, community, save/load, …) are registered BELOW this adapter, and the cook
    -- pane lingers VISIBLE after a visit — pausing after cooking re-announced the
    -- stale dish and shadowed the ring (user 2026-07-15 night). Ring genuinely open
    -- (Core.ring_open, the live_ring visibility test) = a real menu owns the screen.
    if Core.ring_open(tick) then
        spoken_key = nil
        return false
    end
    -- The embedded ENTRY menu ("Preparar un platillo"/"Salir") owns the screen while
    -- its Shop_Top rows are visible — screen_shoplist (registered above) reads them;
    -- the dish pane behind it is backdrop. Also the moment to forget the spoken key,
    -- so coming back INTO the dish list announces fresh.
    local top
    pcall(function() top = host.WL_CookingTop end)
    if Core.valid(top) and #A.shoptop_rows(top) > 0 then
        spoken_key = nil
        return false
    end
    pcall(function() shoplist = host.CookMenuList end)
    list = Core.valid(shoplist) and shoplist.WL_Shop_Cmn_List or nil
    if A.list_select_index(list) == nil then return false end
    -- Live-detail gate: a genuinely open menu always shows the selected dish in the
    -- detail pane. A pooled closed Shop_Cook_C keeps a FROZEN GetSelectValue() (that
    -- index is known-stale on this screen), which held this adapter ACTIVE and
    -- silently shadowed everything below it — the mute ring pause (2026-07-06).
    return (title() or genre()) ~= nil
end

function Cooking.reset()
    ann:reset()
    last_overlay, last_sig = nil, nil
    latch_logged = false
    -- spoken_key deliberately NOT cleared here: reset() fires on every registry
    -- adapter switch, which is exactly the flip-flop being suppressed.
end

-- F1 (Registry.repeat_current): an explicit repeat overrides the flip-flop
-- suppression — forget the spoken key and let update() re-announce everything.
function Cooking.reannounce()
    spoken_key = nil
    last_sig = nil
    ann:reset()
end

function Cooking.update()
    if overlay then
        if overlay ~= last_overlay then
            last_overlay = overlay
            Speech.say(overlay, true)   -- context change: the result interrupts
        end
        overlay_spoken = overlay        -- release: next tick this banner no longer
                                        -- keeps the adapter active (notice pattern)
        return
    end
    last_overlay = nil
    local row = A.list_selected_row(list)
    -- Detail-pane title first (live), list row as fallback (frozen index).
    local name = title() or (row and clean(row.name)) or nil
    if not name then return end
    -- Selection moved but the spoken name key didn't (stale name source) → force the
    -- announcer to re-speak name + value + detail anyway.
    local sig = selection_sig()
    -- Flip-flop suppression: exactly this selection (incl. the owned count) was
    -- already spoken this visit — a registry switch away and back must stay silent.
    local key = sig .. "|" .. tostring(row and row.num or "")
    if key == spoken_key then return end
    if sig ~= last_sig then
        if last_sig ~= nil then ann:invalidate() end
        last_sig = sig
        if DEBUG then dump_sample(sig, row) end
    end
    -- "Cooking" on entry; genre as the tab; recipe name; its count as the value (so a
    -- post-cook count change speaks just the number); full detail as the tooltip.
    if LATCH_DEBUG then pcall(latch_log, name) end
    ann:focus(I18n.header(13), genre(), name, row and clean(row.num) or nil, detail)
    spoken_key = key
end

return Cooking
