-- R3 radar target picker (controller overlay, modal toggle).
--
-- Click the RIGHT STICK (R3) in the RPG overworld to OPEN a target list; the game is
-- frozen while it's open (input_bridge hides the pad from the game, see input.lua) so
-- no field command fires. R3 (not a trigger) is used so the flight controls stay free
-- (R2 = descend / L2 = ascend on the Flying Nimbus). It's modal — you don't hold
-- anything:
--   * L1 / R1  — previous / next CATEGORY (like menu tabs): Quests, Collectibles,
--                Characters, Enemies, Sites, Fishing, Gathering, Shops, Minigames,
--                Dragon Balls, Other (GROUP_ORDER in nav_tracker).
--   * D-pad up / down — move through the items in the category (nearest first).
--   * A / Cross — SELECT the focused item: it becomes the audio-radar target, menu closes.
--   * R3 again  — close WITHOUT changing anything (cancel).
--   * B / Circle — close AND stop tracking the current target (the "stop" shortcut).
--
-- RPG-ONLY, NEVER COMBAT: opening is gated on Nav.field_ready() (world alive AND no
-- pausing menu). The minimap — the game's own "free-roaming" signal — is hidden in
-- battle/cutscenes/menus, so R3 there does nothing and the pad is never blocked. If a
-- battle/cutscene starts while the menu is open, it closes and unblocks at once.
--
-- On close the pad stays blocked until every button/trigger is released (a "drain"),
-- so the game never receives the residual A/R3/B press that closed the menu.
--
-- All world reads live in nav_tracker (Nav.list_targets / Nav.set_manual_target /
-- Nav.stop_tracking), behind the radar's safety gates; this file is pure input + speech.

local Input = require("input")
local Speech = require("speech")
local I18n = require("i18n")
local Nav = require("nav_tracker")
local Transition = require("transition")
local PadPoll = require("pad_poll")

local Menu = {}

-- Runs on the shared 20ms pad scheduler (pad_poll.lua): catches the R3 press within ~1
-- frame so the game barely sees it before we block (the opening click could otherwise
-- leak a frame to R3's field action).
-- Trigger raw (0..255) below which it counts as released (drain). RAISED FROM 25 TO XInput's own
-- XINPUT_GAMEPAD_TRIGGER_THRESHOLD (30) on 2026-07-31 — that mismatch WAS the original defect: a
-- worn or third-party trigger resting at 26-29 reads as released to XInput and to the game, but
-- never to this line, so the drain below could never complete and the player was left with no pad
-- and no keyboard. Matching the platform's own definition of "released" fixes it at the source; a
-- trigger the game already ignores cannot leak anything by being treated as released here.
local REL_TH = 30
local DOUBLE_TAP_TICKS = 20   -- ~400 ms at the 20ms pad tick: window for a 2nd R3 tap (= explore toggle)
-- A stick double-click is stiff and easily slower than the window above; when the 2nd tap
-- lands just after the picker already auto-opened, we still treat it as the toggle (see the
-- open branch) as long as it arrives within this grace of the FIRST tap and the player did
-- not navigate the picker in between. Without this the slow 2nd tap only cancelled the
-- accidentally-opened picker and the explore toggle never fired (never turned OFF).
local DOUBLE_RESCUE_TICKS = 34   -- ~680 ms from tap 1: the slow-double rescue window

local running = false
local open = false        -- menu currently open (and pad blocked)
local blocked = false
local draining = false    -- closed, waiting for a neutral pad before unblocking
-- STUCK-PAD RESCUE for that wait. Anti-lockout only — see the drain branch in step().
--
-- The first attempt at this (earlier on 2026-07-31) was a flat 2 s deadline that forced the
-- release regardless of pad state, and it REGRESSED into the very thing the drain exists to
-- prevent: the player confirms a target with A, keeps a button down a moment longer than the
-- deadline, and the forced unblock hands the game a held A — Goku jumps. Same for L1 (ki sense).
-- The user's "sometimes it interferes with the game" was exactly that.
--
-- The honest discriminator is not TIME, it is CHANGE: a stuck button reports a CONSTANT bitmask,
-- while a pad someone is actually using keeps changing. So the deadline RESTARTS on every change
-- of the digital button state, and only a pad that has not changed at all for DRAIN_STUCK_S is
-- treated as stuck. A player holding, mashing or fumbling can never trip it; a genuinely stuck
-- button trips it in three seconds. Triggers are deliberately NOT part of the change signal —
-- they are analog and jitter constantly, which would reset the deadline forever on precisely the
-- drifting trigger this is meant to rescue; the REL_TH fix above handles them instead.
local DRAIN_STUCK_S = 3.0
local drain_until = nil
local drain_btn = nil     -- digital button state the stuck-detector last observed
local cats = {}           -- [{ key, name, items={{actor,key,dist,noun},...} }]
local ci, ii = 1, 1       -- current category / item index
local prev_btn = 0        -- previous button bitmask (edge detection)
local tk = 0              -- step counter (double-tap timing)
local last_r3_tk = nil    -- tick of the previous R3 press
local pending_open_tk = nil   -- R3 pressed; holding to see if a 2nd tap makes it a toggle
-- Opened from the KEYBOARD. Decides one thing only: whether "no pad snapshot" means the
-- picker must be torn down (pad-opened: the pad went away mid-menu) or is the normal state
-- (keyboard-opened on a machine with no pad at all).
local kb_open = false

local B = Input.BTN

-- ---- announcements -----------------------------------------------------------------

local function announce_item(with_cat)
    local cat = cats[ci]
    if not cat then Speech.say(I18n.t("radar_nothing"), true) return end
    local it = cat.items[ii]
    if not it then Speech.say(cat.name .. ", " .. I18n.t("radar_empty_cat"), true) return end
    local parts = {}
    if with_cat then parts[#parts + 1] = cat.name end
    parts[#parts + 1] = Nav.item_phrase(it)
    parts[#parts + 1] = string.format(I18n.t("pos"), ii, #cat.items)
    Speech.say(table.concat(parts, ", "), true)
end

-- ---- open / close ------------------------------------------------------------------

local function do_open()
    -- Build the target list BEFORE blocking. If a world read faults (an uncatchable C++
    -- abort on this game — e.g. a bad property access), it must NOT strand the pad in a
    -- blocked state and freeze the game. The one-tick R3 leak before we block is
    -- harmless (R3 = ki-sense). Safety over the micro-leak.
    -- Shared TTL snapshot, not a fresh sweep (crash audit RANK 3, 2026-07-31): this runs
    -- SYNCHRONOUSLY on the 20 ms pad dispatch, and the sweep behind it is ~1.2 s of unbudgeted
    -- FindAllOf — so in a dense area the game visibly locked up on the R3 press. If explore mode
    -- swept recently the list is already there; otherwise this pays for it and explore goes free.
    local list = Nav.targets_cached()
    Input.block(true)
    blocked = true
    open = true
    _G.__KakarotPadModal = "radar"   -- claim the pad (mutex with config_menu)
    cats = list
    ci, ii = 1, 1
    if #cats == 0 then
        Speech.say(I18n.t("radar_nothing"), true)
    else
        announce_item(true)
    end
end

-- How long each keyboard-block lease lasts. The step runs every 20 ms, so this is ~15
-- ticks of slack: long enough to ride out a frame hitch, short enough that if the mod
-- stops renewing it (a crash, a reload, an error) the player has their keyboard back
-- almost immediately. See Input.kb_block.
local KB_BLOCK_MS = 300

-- Close the menu. mode:
--   "select" — commit the focused item to the radar
--   "stop"   — stop tracking the current target (B / Circle)
--   "cancel" — leave current tracking unchanged (R2 again, or lost field/pad)
-- The pad stays blocked (draining) until the player releases everything, so the
-- residual press that closed the menu never reaches the game.
local function do_close(mode)
    open = false
    kb_open = false
    -- Give the keyboard back at once rather than waiting for the lease to lapse.
    Input.kb_block(0)
    if mode == "select" then
        local cat = cats[ci]
        local it = cat and cat.items[ii]
        if it and it.actor then
            -- grp/stateful drive the arrival chaining for pickup categories.
            Nav.set_manual_target(it.actor, it.key, Nav.item_label(it),
                it.grp, it.stateful)
        end
    elseif mode == "stop" then
        Nav.stop_tracking()
    end
    cats = {}
    -- keep blocked; drain until neutral (see step). If the pad is already gone we
    -- unblock immediately (handled by the caller for the pad-lost path).
    draining = true
    -- Seed the stuck-detector from the CLOSING press itself, so its first observation is the
    -- state the player is actually holding and the deadline starts counting from a real sample.
    drain_btn, drain_until = nil, os.clock() + DRAIN_STUCK_S
end

-- ---- keyboard control (the picker without a pad) ------------------------------------
--
-- The picker is a pad modal: everything below lives in step(), which bails out the moment
-- Input.read() has no snapshot — so on a keyboard-only setup the menu could not be opened
-- at all, and with a pad merely idle it could not be driven. These commands drive the SAME
-- state machine, and they are QUEUED rather than executed where the keybind fires: a
-- RegisterKeyBind callback is not the game thread, and do_open() reads the world
-- (Nav.list_targets). step() already runs on the game thread every 20 ms, so consuming the
-- command there costs nothing and keeps every world read where it belongs.
local kb_cmd = nil

-- Called from main.lua's keybinds (via App). One pending command is enough — a human
-- cannot outrun a 20 ms poll, and dropping the older of two same-tick presses is the
-- right behaviour anyway.
function Menu.key(cmd) kb_cmd = cmd end

local function handle_kb(cmd)
    if cmd == "toggle" then
        if open then
            do_close("cancel")
        elseif Nav.field_ready() then
            -- Respect the pad-modal mutex exactly as the R3 path does: if the config menu
            -- owns the pad, the picker must not steal it.
            local modal = _G.__KakarotPadModal
            if not modal or modal == "radar" then
                -- Take the keyboard BEFORE building the list, so the very keypress that
                -- opened the picker doesn't also reach the game (V is the skill palette in
                -- the game's own default layout). Safe to do first, unlike the pad block
                -- below — the keyboard block is a lease that expires by itself, so a fault
                -- inside do_open's world reads cannot strand it.
                Input.kb_block(KB_BLOCK_MS)
                do_open()
                kb_open = true
            end
        end
        return
    end
    if not open then return end
    if cmd == "select" then do_close("select") return end
    if cmd == "stop" then do_close("stop") return end
    if #cats == 0 then return end
    -- A real picker session: a later R3 must read as cancel, not as the second half of an
    -- explore double-tap (same reason the pad navigation clears it).
    last_r3_tk = nil
    local n = #cats
    if cmd == "cat_next" then ci = ci % n + 1; ii = 1; announce_item(true) return end
    if cmd == "cat_prev" then ci = (ci - 2) % n + 1; ii = 1; announce_item(true) return end
    local m = #cats[ci].items
    if m == 0 then return end
    if cmd == "next" then
        ii = ii % m + 1; announce_item(false)
    elseif cmd == "prev" then
        ii = (ii - 2) % m + 1; announce_item(false)
    end
end

-- ---- per-tick step (game thread) ----------------------------------------------------

local function step()
    tk = tk + 1
    -- Transition gate FIRST (pure Lua + native pad calls only): a map switch is in
    -- progress — close the menu, drop the actor refs in `cats`, and NEVER leave the
    -- pad blocked across a level change.
    if Transition.active() then
        if blocked then Input.block(false) end
        Input.kb_block(0)
        blocked, open, draining, kb_open = false, false, false, false
        cats = {}
        prev_btn = 0
        last_r3_tk, pending_open_tk = nil, nil
        if _G.__KakarotPadModal == "radar" then _G.__KakarotPadModal = nil end
        return
    end

    -- Keyboard commands FIRST: they must work with no pad attached, and the snapshot check
    -- right below returns early in exactly that case.
    if kb_cmd then
        local cmd = kb_cmd
        kb_cmd = nil
        handle_kb(cmd)
    end

    -- RENEW the keyboard block while the menu is up — the same idea as blocking the pad,
    -- so a Page Down that moves the picker doesn't also reach the game. It has to be
    -- renewed here, every tick, and it must sit ABOVE the no-pad early return: on a
    -- keyboard-only machine that return is the only path this function ever takes.
    -- Renewal is what makes the block crash-proof (Input.kb_block).
    if open or draining then Input.kb_block(KB_BLOCK_MS) end
    -- ...and RE-ASSERT the PAD block every tick while the picker is open (user 2026-07-31: "other
    -- times it starts triggering game commands — ki sense on L1"). The pad block is a LEASE
    -- (PAD_BLOCK_LEASE_MS = 1 s) renewed only as a side effect of input_bridge's poll(), and
    -- renewal deliberately only EXTENDS A LIVE LEASE — it can never resurrect an expired one. But
    -- this menu asserted it exactly ONCE, in do_open. So any gap longer than a second in the 20 ms
    -- dispatch — and it drops a tick whenever the game thread is busy, which is precisely when a
    -- level streams — let the lease die with the picker still open, and from that moment every
    -- button reached the game with nothing to re-arm it. Re-asserting is one interlocked write per
    -- tick and makes the whole failure class impossible, instead of requiring us to prove which
    -- starvation path caused it. NOT while `draining`: that phase is trying to hand control back.
    -- `blocked` is set alongside it so the flag never lies about the hardware state: a
    -- KEYBOARD-opened picker also asserts the block here, and the Transition teardown above
    -- only releases `if blocked`.
    if open then Input.block(true); blocked = true end

    local snap = Input.read()
    if not snap then
        -- No pad snapshot. The pad side is always torn down (never leave the game blocked),
        -- but this is ALSO the steady state of a keyboard-only player, and blindly clearing
        -- `open` here would kill a keyboard-opened picker 20 ms after it opened. So only a
        -- PAD-opened menu counts as "pad lost".
        if blocked then Input.block(false) end
        Input.kb_block(0)
        blocked, draining = false, false
        prev_btn = 0
        last_r3_tk, pending_open_tk = nil, nil
        if not kb_open then open = false end
        -- The keyboard picker still needs the field safety the pad path gets below: a
        -- battle or cutscene starting while it is open must close it.
        if open and not Nav.field_ready() then do_close("cancel") end
        if not open and _G.__KakarotPadModal == "radar" then _G.__KakarotPadModal = nil end
        return
    end

    -- Another overlay (the config menu) owns the pad — stay out of its way, and drop any
    -- half-formed R3 tap so nothing fires when control returns.
    local modal = _G.__KakarotPadModal
    if modal and modal ~= "radar" then
        prev_btn = snap.buttons
        last_r3_tk, pending_open_tk = nil, nil
        return
    end

    local function pressed(mask) return (snap.buttons & mask) ~= 0 and (prev_btn & mask) == 0 end

    -- Draining: menu closed, waiting for a fully neutral pad before handing control
    -- back — so the A/R3/B that closed it isn't delivered to the game.
    if draining then
        -- The pad is neutral once no button is down and both triggers are below the platform's
        -- own released-threshold (REL_TH — see there; comparing against a LOWER number than
        -- XInput's was the original lock-out bug).
        local neutral = snap.buttons == 0 and snap.rt < REL_TH and snap.lt < REL_TH
        -- Stuck-pad rescue, restarted on every CHANGE of the button state (see DRAIN_STUCK_S).
        -- This is what stops the rescue from stealing a real press: as long as the player is
        -- doing anything at all, the deadline keeps moving and the release stays courteous.
        if snap.buttons ~= drain_btn then
            drain_btn, drain_until = snap.buttons, os.clock() + DRAIN_STUCK_S
        end
        if neutral or (drain_until and os.clock() >= drain_until) then
            Input.block(false); blocked = false; draining = false
            drain_until, drain_btn = nil, nil
            Input.kb_block(0)
            if _G.__KakarotPadModal == "radar" then _G.__KakarotPadModal = nil end
        end
        prev_btn = snap.buttons
        return
    end

    if not open then
        -- R3: a SINGLE click opens the target menu; a DOUBLE-tap toggles the passive
        -- explore radar. We hold the open ~280 ms to see if a 2nd tap arrives (the menu
        -- is the fallback now, so the small delay is fine); the R3 leak to the game in
        -- that window is harmless (R3 = ki-sense, as when opening).
        -- L3 held? this is the config-menu chord (config_menu.lua) — ignore R3 entirely
        -- so it neither opens the picker nor starts an explore double-tap.
        if pressed(B.RIGHT_THUMB) and not Input.down(snap, B.LEFT_THUMB) and Nav.field_ready() then
            if last_r3_tk and (tk - last_r3_tk) <= DOUBLE_TAP_TICKS then
                last_r3_tk, pending_open_tk = nil, nil
                Nav.toggle_explore()
            else
                last_r3_tk, pending_open_tk = tk, tk
            end
        elseif pending_open_tk and (tk - pending_open_tk) > DOUBLE_TAP_TICKS then
            pending_open_tk = nil
            if Nav.field_ready() then do_open() end
        end
        prev_btn = snap.buttons
        return
    end

    -- Safety: a battle/cutscene/menu started while open -> bail out (drain-unblock).
    if not Nav.field_ready() then
        do_close("cancel")
        prev_btn = snap.buttons
        return
    end

    if pressed(B.A) then                        -- A / Cross -> select + track
        last_r3_tk, pending_open_tk = nil, nil
        do_close("select")
    elseif pressed(B.RIGHT_THUMB) and last_r3_tk and (tk - last_r3_tk) <= DOUBLE_RESCUE_TICKS then
        -- Slow explore double-tap: tap 1 auto-opened the picker; this tap 2 completes the
        -- toggle instead of merely cancelling the (unwanted) picker. Only when the player
        -- did NOT navigate the picker in between (navigation clears last_r3_tk below).
        last_r3_tk, pending_open_tk = nil, nil
        do_close("cancel")
        Nav.toggle_explore()
    elseif pressed(B.RIGHT_THUMB) or pressed(B.B) then   -- R3 again / B -> close
        last_r3_tk, pending_open_tk = nil, nil
        do_close(pressed(B.B) and "stop" or "cancel")
    elseif #cats > 0 then
        local n = #cats
        if pressed(B.RIGHT_SHOULDER) then      -- R1 -> next category
            last_r3_tk = nil                   -- real picker session: a later R3 = cancel
            ci = ci % n + 1; ii = 1; announce_item(true)
        elseif pressed(B.LEFT_SHOULDER) then   -- L1 -> previous category
            last_r3_tk = nil
            ci = (ci - 2) % n + 1; ii = 1; announce_item(true)
        else
            local m = #cats[ci].items
            if m > 0 then
                if pressed(B.DPAD_DOWN) then
                    last_r3_tk = nil
                    ii = ii % m + 1; announce_item(false)
                elseif pressed(B.DPAD_UP) then
                    last_r3_tk = nil
                    ii = (ii - 2) % m + 1; announce_item(false)
                end
            end
        end
    end

    prev_btn = snap.buttons
end

-- ---- loop management (same generation/busy guard as nav_tracker) --------------------

function Menu.start()
    if running then return end
    if not Input.is_loaded() then
        print("[KakarotAccess] radar_menu: input bridge not loaded, disabled\n")
        return
    end
    running = true
    -- Shared 20ms scheduler (pad_poll.lua) — one dispatch serves every pad stepper.
    PadPoll.register("radar_menu", step, function()
        -- never strand the pad in a blocked state on an error
        if blocked then Input.block(false) end
        Input.kb_block(0)
        blocked, open, draining, kb_open = false, false, false, false
        if _G.__KakarotPadModal == "radar" then _G.__KakarotPadModal = nil end
    end)
end

function Menu.stop()
    running = false
    PadPoll.unregister("radar_menu")
    if blocked then Input.block(false) end
    Input.kb_block(0)
    blocked, open, draining, kb_open = false, false, false, false
    cats = {}
    if _G.__KakarotPadModal == "radar" then _G.__KakarotPadModal = nil end
end

return Menu
