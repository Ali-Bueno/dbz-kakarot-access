-- Screen adapter: overworld Party menu (Start_Party_C -> UAT_UIStartParty).
--
-- The LEFT column is the three slots the cursor navigates: Jugador / Apoyo 1 / Apoyo 2. They hang
-- off WL_Start_Party_List (Start_Party_List_C) and the F7 census names them
-- `Start_Party_List.WidgetTree_0.{Start_Party_Bar00, Start_Party_Spo00, Start_Party_Spo01}` — so
-- they are fetched by WidgetTree NAME, in cursorIndex order. (Until 2026-07-28 they were recovered
-- from the native fixed array `WL_Start_Party_Bar` @0x398 with RegisterCustomProperty; see
-- ui_charbar.lua for why that whole mechanism was withdrawn — it silently stops resolving after a
-- map transition while leaving the names whitelisted past Core.member's gate.)
-- partySelectData.cursorIndex (0x420 on the host) selects. Each bar exposes Txt_Player/Txt_Suport
-- (role), Txt_Name (character) or Txt_None ("Sin establecer"), and Txt_Lv.
--
-- The RIGHT panel — "Personajes en espera", the list you land on after confirming a slot — is the
-- host's own 4-row array WL_Start_Char_Bar @0x3C0 (AT.hpp:36660), rows named
-- `Start_Party_C_0.WidgetTree_0.Start_Char_Bar00..03`. It went unread until 2026-07-28 (user:
-- "al entrar a los submenús de jugador o apoyo no me lee la lista de personajes"), which meant the
-- half of this screen where you actually choose someone was silent. Same row class as the
-- Characters menu, hence the shared ui_charbar module.
--
-- WHICH PANEL HAS FOCUS is not a stored flag — the whole hierarchy exposes exactly one small
-- integer (partySelectData.cursorIndex, and that one belongs to the left column; the rest of that
-- 0xB0 struct is two TMaps of icon textures). The game drives the panels with Focus()/LoseFocus()
-- on the rows instead (AT.hpp:36457-36459), so the signal is the row's cursor MARKER.
--
-- THAT SIGNAL FLAPS, and the first cut proved it (user, 2026-07-28: "el anuncio se solapa porque
-- se lee nombre de personaje, que no debería"). Two defects fed each other: the marker was tested
-- with `Core.is_visible` (own slate visibility — a row in an unfocused panel keeps reporting
-- itself visible, so the roster looked focused permanently), and the side change re-announced
-- immediately, so every left-column move was followed one tick later by a spurious roster
-- readout. Both are fixed: the marker goes through `Core.on_screen` (ui_charbar), and a side
-- change must now SETTLE for SIDE_SETTLE_S of wall clock before it is believed — except a left
-- cursor move, which is a real user action and switches at once. One log line per accepted
-- change, because a signal that gates a whole panel deserves to be visible when it misfires.

local Core = require("ui_core")
local CharBar = require("ui_charbar")
local Mem = require("mem")
local OFF = require("native_offsets")
local I18n = require("i18n")

local Party = {}

local ann = Core.make_announcer()
local host, list, cur_idx, tick = nil, nil, nil, 0
local prev_idx, side, roster_bar = nil, nil, nil
-- `side` is the SETTLED panel; want_side/want_since are the candidate awaiting confirmation, and
-- spoken_side is the panel the announcer last spoke (they differ for one tick when the settled
-- panel has no readable row yet, and only spoken_side may reset the announcer).
local want_side, want_since, spoken_side = nil, nil, nil

-- AT.hpp:36660 — UAT_UIStartParty.WL_Start_Char_Bar @0x3C0, size 0x20 → 4 rows, named
-- Start_Char_Bar00..03 in the WidgetTree (F7 census 2026-07-28).
local roster = CharBar.new({ name = "Start_Char_Bar%02d", count = 4 })

-- How long a panel-focus change must hold before it is believed. The marker is a derived engine
-- signal, and derived signals flap; this is the debounce the standing rule asks for, in WALL
-- CLOCK rather than in calls (a poll driven from several loops confirms a call count instantly
-- and confirms nothing).
local SIDE_SETTLE_S = 0.30

-- cursorIndex -> the WidgetTree name of that slot's bar on WL_Start_Party_List.
local SLOT_PROP = { [0] = "Start_Party_Bar00", [1] = "Start_Party_Spo00", [2] = "Start_Party_Spo01" }

local function cursor_index()
    if not Core.valid(host) then return nil end
    local idx
    pcall(function() idx = host.partySelectData.cursorIndex end)
    if type(idx) ~= "number" then idx = Mem.i32(host, OFF.startParty.cursorIndex) end
    if type(idx) == "number" and idx >= 0 then return idx end
    return nil
end

-- The UAT_UIStartPartyBar for a slot index, by WidgetTree name. Through Core.member, so a name the
-- class does not declare is refused by the existence gate instead of aborting the game.
local function slot_bar(idx)
    if not Core.valid(list) then return nil end
    local prop = SLOT_PROP[idx]
    if not prop then return nil end
    local bar = Core.member(list, prop)
    if Core.valid(bar) then return bar end
    return nil
end

local function selected_row()
    local bar = slot_bar(cur_idx)
    if not bar then return nil end
    local role = Core.read_text(Core.member(bar, "Txt_Player")) or Core.read_text(Core.member(bar, "Txt_Suport"))
    local name = Core.read_text(Core.member(bar, "Txt_Name")) or Core.read_text(Core.member(bar, "Txt_None"))
    if not role and not name then return nil end
    return { role = role, name = name, lv = Core.read_text(Core.member(bar, "Txt_Lv")) }
end

-- The roster row under the cursor, or nil when the roster panel is not focused. "Remove" and empty
-- placeholders live in Txt_None, and here — unlike the Characters screen — they ARE selectable
-- rows, so they count as the name.
local function roster_row()
    if not roster_bar then return nil end
    local r = CharBar.read(roster_bar)
    if not r then return nil end
    local name = r.name or r.none
    if not name then return nil end
    return Core.phrase(name, r.lv, Core.phrase(r.pow, r.num))
end

function Party.is_active()
    tick = tick + 1
    host = Core.first_on_screen("AT_UIStartParty", tick)
    if not host then return false end
    list = Core.member(host, "WL_Start_Party_List")
    cur_idx = cursor_index()
    -- The roster is resolved HERE, not only in update(): if the game parks cursorIndex at -1 while
    -- the RIGHT panel owns the focus (the Characters screen's GetCursorIndex does exactly that when
    -- its list is unfocused), gating solely on the left column's index would put this adapter to
    -- sleep on precisely the half of the screen we are trying to read. Four guarded member reads,
    -- and only once the Party screen is already on screen.
    roster_bar = roster:selected(host)
    return Core.valid(list) and (cur_idx ~= nil or roster_bar ~= nil)
end

function Party.reset()
    ann:reset()
    prev_idx, side, roster_bar = nil, nil, nil
    want_side, want_since, spoken_side = nil, nil, nil
end

-- Accept a panel-focus change only once it has held for SIDE_SETTLE_S, so a flapping marker can
-- no longer speak the other panel between two cursor moves. `immediate` is for a change the USER
-- provably caused (the left cursor moved), which needs no confirmation.
local function settle_side(want, immediate)
    if side == nil then side, want_side = want, nil return side end
    if want == side then want_side = nil return side end
    if immediate then
        side, want_side = want, nil
    elseif want_side ~= want then
        want_side, want_since = want, os.clock()
    elseif os.clock() - (want_since or 0) >= SIDE_SETTLE_S then
        side, want_side = want, nil
    end
    return side
end

function Party.update()
    -- A left-column move is a real user action: it owns the focus whatever the roster markers say,
    -- and it switches sides with no settle delay. This also covers the first tick after entry
    -- (prev_idx is nil), which is where the game starts you.
    local moved = (cur_idx ~= prev_idx)
    prev_idx = cur_idx

    local want, immediate = "slots", true
    if not moved then want, immediate = (roster_bar and "roster" or "slots"), false end
    local prev_side = side
    local cur = settle_side(want, immediate)
    if prev_side ~= nil and cur ~= prev_side then
        print(string.format("[KakarotAccess] party panel -> %s (marker=%s, cursor=%s)\n",
            cur, tostring(roster_bar ~= nil), tostring(cur_idx)))
    end

    local now, screen, body
    local rrow = (cur == "roster") and roster_row() or nil
    if rrow then
        now, screen, body = "roster", I18n.t("party_charlist"), rrow
    else
        local row = selected_row()
        if not row then return end
        -- role (Player / Support), character (or "Not Set"), level.
        now, screen, body = "slots", I18n.startlist(3), Core.phrase(row.role, row.name, row.lv)
    end

    -- Switching panels is a context change, so it re-announces with the panel's own name in front
    -- of the first row. Reusing the announcer's "entering a screen" branch (rather than its `tab`
    -- parameter) is deliberate: `tab` speaks the label ALONE and swallows the row underneath it,
    -- which is precisely the readout the user is missing.
    if now ~= spoken_side then
        spoken_side = now
        ann:reset()
    end
    ann:focus(screen, nil, body, nil, nil)
end

return Party
