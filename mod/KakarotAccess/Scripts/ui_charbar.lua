-- Shared reader for the character ROW widget (UAT_UIStartCharBar, AT.hpp:36433) used by BOTH
-- overworld screens that list characters:
--
--   * Characters menu — host Start_Char_C  (UAT_UIStartChar),  rows Start_Char_Bar00..05
--   * Party menu      — host Start_Party_C (UAT_UIStartParty), rows Start_Char_Bar00..03
--                       (the right-hand "Personajes en espera" panel)
--
-- Same row class on both, so the knowledge lives here once instead of twice.
--
-- HOW THE ROWS ARE REACHED, and why this is the SECOND design (2026-07-28).
-- The native member is a FIXED C ARRAY (`WL_StartCharBarList` @0x3B0 size 0x30 → 6,
-- `WL_Start_Char_Bar` @0x3C0 size 0x20 → 4). UE4SS reflection COLLAPSES a fixed array to element
-- 0, and `arr[i]` / `GetArrayNum` are garbage and an abort respectively — so the first cut
-- recovered slots 1..n-1 with `RegisterCustomProperty` at `base + i*8`, plus `Core.allow_member`
-- to get them past Core.member's existence gate. **That approach is withdrawn.** It has two
-- defects, and the second one was live in the user's game:
--   (a) `Core.allow_member` whitelists a NAME. If the registration silently failed, the name was
--       whitelisted anyway and the gate turned from a protection into a PERMISSION — a raw fetch
--       of a member the class does not declare, i.e. the uncatchable abort.
--   (b) `RegisterCustomProperty` resolves `BelongsToClass` ONCE and then matches by the raw
--       `UClass*` pointer. Blueprint generated classes unload on a map transition and their
--       addresses get reused (this codebase already flushes `prop_sets` for exactly that reason;
--       `custom_props` is never flushed). After the first map load the registration stops
--       matching while the names stay whitelisted, so **every slot but 0 goes silent** — which is
--       precisely the reported bug: the Characters list read only its first row, so moving the
--       cursor announced nothing.
-- The F7 census showed the way out: these rows are Blueprint WidgetTree children, so the engine
-- already exposes each one as its OWN reflected property, by name —
-- `Start_Party_C_0.WidgetTree_0.Start_Char_Bar00`, `…Bar01`, … So we simply ask for them by name.
-- No offsets, no custom properties, no whitelist, nothing to invalidate on a transition, and a
-- name the class does not declare is REFUSED by Core.member's gate instead of aborting the game.
--
-- SELECTION. The list is a VIRTUALISED window over a longer roster, so slot position is not the
-- ordinal and index arithmetic needs the scroll offset. Hosts that expose a reflected cursor
-- (UAT_UIStartChar.GetCursorIndex/GetViewIndex) should use it — it is deterministic. The row's own
-- cursor markers (`Pnl_Curs_All` @0x410, `Img_Curs00` @0x438) are the fallback, and they are
-- tested with `Core.on_screen`, NEVER `Core.is_visible`: IsVisible reports a widget's OWN slate
-- visibility, so a row in an unfocused/pooled panel keeps reporting itself visible and the marker
-- reads as "selected" forever. That mistake made the Party screen announce the roster over the
-- slot column on every cursor move.
--
-- Every name and offset here comes from the CXX header dump plus the F7 census, never a guess.

local Core = require("ui_core")

local CharBar = {}
CharBar.__index = CharBar

-- cfg.name  = printf pattern for the row widgets, e.g. "Start_Char_Bar%02d"
-- cfg.count = how many rows the host declares (the fixed array's size / 8)
function CharBar.new(cfg)
    return setmetatable({ name = cfg.name, count = cfg.count }, CharBar)
end

-- Every populated row of the visible window, in on-screen order. A row the host does not declare
-- is refused by Core.member's existence gate (logged, not fatal), so an over-long count is safe.
function CharBar:bars(host)
    local list = {}
    for i = 0, self.count - 1 do
        local bar = Core.member(host, string.format(self.name, i))
        if Core.valid(bar) then list[#list + 1] = bar end
    end
    return list
end

-- Does this row carry the cursor? Either marker counts: which one the game drives is not
-- documented, and requiring both would silence the screen if only one is used.
-- on_screen, not is_visible — see the header.
function CharBar.marked(bar)
    return Core.on_screen(Core.member(bar, "Pnl_Curs_All"))
        or Core.on_screen(Core.member(bar, "Img_Curs00"))
end

-- The marked row, or nil when nothing in this list carries the cursor (which on the Party screen
-- is also the signal that the roster panel does not have the focus).
function CharBar:selected(host)
    for _, bar in ipairs(self:bars(host)) do
        if CharBar.marked(bar) then return bar end
    end
    return nil
end

-- Read one row. Uses read_text (mainTxt, else GetText) since Txt_Lv/Txt_Power_Num render their
-- value via the parent FText. `none` is the placeholder label (an empty slot, or the Party
-- roster's "Quitar"/Remove entry) — callers decide whether it counts as a name, because the
-- Characters screen skips a nameless row and that behaviour is verified.
function CharBar.read(bar)
    if not Core.valid(bar) then return nil end
    return {
        name = Core.read_text(Core.member(bar, "Txt_Name"))            -- character
            or Core.read_text(Core.member(bar, "Txt_Name_Guest")),     -- guest-slot variant
        none = Core.read_text(Core.member(bar, "Txt_None")),           -- @0x408, placeholder
        lv   = Core.read_text(Core.member(bar, "Txt_Lv")),             -- e.g. "Nvl. 15"
        pow  = Core.read_text(Core.member(bar, "Txt_Power")),          -- "PB" label
        num  = Core.read_text(Core.member(bar, "Txt_Power_Num")),      -- e.g. "3993"
    }
end

return CharBar
