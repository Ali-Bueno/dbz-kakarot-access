-- KakarotAccess — bootstrap (loaded once, never reloaded).
--
-- Responsibilities: init the PRISM bridge, register the keybinds, and hand all
-- accessibility logic to app.lua. Everything app.lua requires is reloadable at
-- runtime via Ctrl+Shift+R (see below), so we can iterate on the whole mod from
-- inside the game. Only the speech bridge survives a reload.
--
-- discover.lua stays in the repo as the dev tool for mapping future screens; it
-- is intentionally NOT wired here so the shipping mod stays lean.

local Speech = require("speech")
local Core = require("ui_core")
local Mem = require("mem")
local Audio = require("audio")
local Input = require("input")
local Settings = require("settings")
local Build = require("build_flags")

local MOD = "KakarotAccess"

-- Our own directory, derived from this file's own path. Used for version.txt and for the crash
-- black box below, so the two can never disagree about where the mod lives.
local MOD_DIR = (debug.getinfo(1, "S").source:sub(2):match("^(.*)[/\\]")) or "."
-- The mod's own folder, one level ABOVE Scripts (…\Mods\KakarotAccess). That is where
-- package.ps1 stages version.txt — the installer reads it from there — while MOD_DIR is the
-- Scripts folder inside it. See the version block below.
local MOD_ROOT = MOD_DIR:match("^(.*)[/\\]") or MOD_DIR

-- NAME THE BUILD IN THE FIRST LINE OF THE LOG (2026-07-29 (e)). A player's UE4SS.log did not say
-- which release produced it, so classifying a crash report meant guessing — and guessing wrong is
-- not hypothetical: an entire round of this investigation was framed against v0.1.3 when the
-- reporter was on v0.1.4, because the only version evidence available was a local git tag that
-- happened to be stale. NOT a hardcoded constant (which would drift from the tag the moment someone
-- forgot to bump it): `package.ps1` already writes `version.txt` into the staged mod as the single
-- source of truth for what was shipped, so we read that. An unpackaged working tree has no
-- version.txt and says so, which is itself the useful answer — it means "this is not a release".
--
-- LOOK IN THE MOD FOLDER, NOT IN Scripts (fixed 2026-07-31). package.ps1 writes version.txt into
-- …\Mods\KakarotAccess\version.txt (line 197 — the installer reads it from there to know what is
-- installed), and this used to open …\Mods\KakarotAccess\Scripts\version.txt, one folder too
-- deep. It therefore never found it: EVERY release printed "dev (unpackaged)", which is the exact
-- opposite of what this line was added for, and left crash reports unattributable to a build.
-- Scripts stays in the list as a second candidate so a hand-placed copy still works.
print("[" .. MOD .. "] Lua loading, build " .. (function()
    local ok, v = pcall(function()
        for _, dir in ipairs({ MOD_ROOT, MOD_DIR }) do
            local f = io.open(dir .. "\\version.txt", "r")
            if f then
                local s = f:read("*a")
                f:close()
                s = s and s:gsub("%s+$", "")
                if s and s ~= "" then return s end
            end
        end
        return nil
    end)
    if ok and v and v ~= "" then return v end
    return "dev (unpackaged, no version.txt)"
end)() .. "\n")

Speech.init()
-- Native memory reader (mem_bridge.dll) for the non-reflected menu selection indices.
-- Loaded here (before the protected snapshot) so it survives Ctrl+Shift+R like the
-- speech bridge. Kept SEPARATE from prism_bridge (screen reader only).
Mem.init()
-- CRASH BLACK BOX (2026-07-26). Opens the memory-mapped marker ring and REPORTS THE PREVIOUS
-- SESSION'S TRAIL before resetting it. If the last run ended in the uncatchable UE4SS throw,
-- these lines are the last things the mod did, and they are the evidence no crash dump and no
-- Lua traceback could give us. Printed into UE4SS.log so a player's log carries it too.
pcall(function()
    local trail = Mem.mark_open(MOD_DIR .. "\\crash_trail.bin")
    if not trail then
        print("[" .. MOD .. "] crash trail unavailable (mem_bridge too old or file locked)\n")
    elseif #trail == 0 then
        print("[" .. MOD .. "] crash trail: empty (clean previous shutdown, or first run)\n")
    else
        print("[" .. MOD .. "] ---- PREVIOUS SESSION ENDED HERE (last " .. #trail
            .. " operations, oldest first) ----\n")
        for _, line in ipairs(trail) do print("[" .. MOD .. "]   " .. line .. "\n") end
        print("[" .. MOD .. "] ---- end of crash trail ----\n")
    end
end)
-- Native audio-cue player (audio_bridge.dll, XAudio2) for the navigation radar.
-- Same lifecycle as the other bridges: loaded once, survives hot reloads.
Audio.init()
-- Native gamepad reader + game-input blocker (input_bridge.dll, XInput IAT hook) for
-- the R3 radar target picker. Installed once (the IAT hook must NOT be reinstalled
-- on a reload), so it lives before the protected snapshot like the other bridges.
Input.init()
-- User config (settings.lua → Scripts/config.txt): audio-cue master switch + volume,
-- radar auto-activation, language override. Loaded here (before the protected snapshot)
-- so the in-memory values survive a Ctrl+Shift+R reload; the config menu writes changes
-- straight to disk. app.start() re-applies the language override to the (reloaded) i18n.
Settings.init()
-- Braille displays: Speech.init() runs long before the config exists (it is the first thing the
-- mod does, so a load failure is reported through the reader itself), so the braille mode is
-- applied here, once Settings is up. The config menu re-applies it whenever the user changes it.
Speech.set_braille(Settings.braille())

-- Snapshot everything loaded so far (stdlib + speech + prism_bridge + mem_bridge). These must
-- survive a reload; anything required AFTER this point is our own logic and is
-- cleared/re-required on reload. New feature modules are picked up automatically.
local protected = {}
for name in pairs(package.loaded) do protected[name] = true end

local App = require("app")

-- === Keybinds (registered once; they delegate to App so a reload takes effect) ==
-- These are accessibility conveniences; the mod is designed to be played with a
-- CONTROLLER (see the radar picker, config menu, status stepping, etc.). The
-- developer/diagnostic keys are gathered at the bottom behind Build.debug and do
-- NOT ship in releases.

-- F8: speech test through the full pipeline (announces which screen reader PRISM picked).
-- Both wrapped (crash audit RANK 1, 2026-07-31): a RegisterKeyBind callback runs on UE4SS's
-- KEYBOARD thread, and Speech.say/stop are not flag flips — say() does a read-modify-write of
-- speech.lua's shared `pending` queue that the poll loop also mutates, on the SAME lua_State.
RegisterKeyBind(Key.F8, function()
    ExecuteInGameThread(function()
        Speech.say("Kakarot accessibility online. Screen reader: " .. Speech.backend_name(), true)
    end)
end)

-- Ctrl+F8: silence the screen reader.
RegisterKeyBind(Key.F8, { ModifierKey.CONTROL }, function()
    ExecuteInGameThread(function() Speech.stop() end)
end)

-- F1: repeat the currently focused menu item.
RegisterKeyBind(Key.F1, function() App.repeat_current() end)

-- F2: read the on-screen button prompts (the contextual keyhelp bar) in full.
RegisterKeyBind(Key.F2, function() App.read_keyhelp() end)

-- Ctrl+F2: toggle the AUTOMATIC announcement of a screen's actions ("X: assign to a slot",
-- "Y: skill tree"), spoken on entering a menu and whenever they change. F2 still works.
RegisterKeyBind(Key.F2, { ModifierKey.CONTROL }, function() App.keyhelp_auto_toggle() end)

-- F3: toggle the quest navigation radar (off = immediate silence).
RegisterKeyBind(Key.F3, function() App.nav_toggle() end)

-- Shift+F3: toggle NavMesh route guidance (beacon follows path corners vs straight line).
RegisterKeyBind(Key.F3, { ModifierKey.SHIFT }, function() App.nav_route_toggle() end)

-- === Keyboard control of the pad-driven overlays ==============================
-- Both the radar target picker and the world map's travel list were pad modals: their
-- steps bail out when there is no pad snapshot, so a keyboard-only player could not reach
-- either. These keys drive both, and each overlay ignores a command unless it is the one
-- currently up — they can never be up together (the picker needs free roam, the travel
-- list is a menu). Every key only QUEUES a command; the overlay consumes it on the game
-- thread.
--
-- V opens and closes the radar picker. While it is open the game does not see the
-- keyboard at all (input_bridge.kb_block), which is what makes plain, unmodified keys
-- usable here: the game's own default layout has the skill palette on V and mount/dismount
-- on the arrows, and none of that fires while the picker owns the keyboard.
RegisterKeyBind(Key.V, function() App.nav_key("toggle") end)
-- Arrows: up/down move through the entries, left/right change category. On the world map
-- the same up/down are the d-pad the travel list already listened for.
RegisterKeyBind(Key.DOWN_ARROW, function() App.nav_key("next") end)
RegisterKeyBind(Key.UP_ARROW, function() App.nav_key("prev") end)
RegisterKeyBind(Key.RIGHT_ARROW, function() App.nav_key("cat_next") end)
RegisterKeyBind(Key.LEFT_ARROW, function() App.nav_key("cat_prev") end)
-- Confirm: lock onto the focused target / travel to the selected point (the pad's A).
RegisterKeyBind(Key.RETURN, function() App.nav_key("select") end)
-- Escape: close the picker AND stop tracking, the pad's B. Only does anything while the
-- picker is open — and while it is, the game never sees the key, so this cannot open the
-- game's own pause menu underneath.
RegisterKeyBind(Key.ESCAPE, function() App.nav_key("stop") end)

-- F5: announce the tracked objective on demand (type, distance, clock direction).
RegisterKeyBind(Key.F5, function() App.nav_where() end)

-- Shift+F5: cycle companion tracking (nearest party member -> next -> quest objective).
RegisterKeyBind(Key.F5, { ModifierKey.SHIFT }, function() App.nav_companion() end)

-- F10: read the current quest objective ("go to Lucca Village") on demand. SHIPPED, not a dev
-- key (moved out of the Build.debug block 2026-07-31): package.ps1 sets debug = false for every
-- release, so in every build a player has ever run there was no way to re-hear the objective at
-- all, and a missed announcement was simply lost. The pad twin is L3 + Y (quest_objective.lua).
-- Nothing runs on the keyboard thread here: like F1/F2/F3/F5/F11 above, the handler only
-- delegates and the ExecuteInGameThread wrap lives one level down, in QuestObjective.read() —
-- which also keeps it hot-reloadable. Deliberately NOT wrapped a second time here: that would be
-- the only ExecuteInGameThread queued from INSIDE a game-thread callback in the whole mod, and
-- UE4SS documents nothing about re-entering its queue while it is being drained.
RegisterKeyBind(Key.F10, function() App.read_objective() end)

-- F11 / Shift+F11: on the character status page (confirm a character in Characters), step
-- forward/back through the stat blocks — HP, Ki and the five attributes — reading each as its
-- total plus the breakdown (base, state boost, food effect). Entering the page already speaks
-- the header (name, level, EXP to next, HP, Ki, BP); these keys are the detail on demand.
-- The GAMEPAD does the same with d-pad down/up (screen_status.lua) — no keyboard needed.
RegisterKeyBind(Key.F11, function() App.status_step(1) end)
RegisterKeyBind(Key.F11, { ModifierKey.SHIFT }, function() App.status_step(-1) end)

-- Ctrl+M: toggle the menu reader on/off (App.toggle announces the state, localized).
RegisterKeyBind(Key.M, { ModifierKey.CONTROL }, function() App.toggle() end)

-- Ctrl+Shift+R: reload the WHOLE mod's logic from inside the game (no UE4SS
-- console needed). We stop all features, drop every non-protected module from
-- the require cache, then re-require app.lua (which re-requires its features
-- fresh) and start again. The PRISM bridge is untouched; leftover poll loops
-- stop themselves via each feature's generation guard.
--
-- ON THE GAME THREAD (fixed 2026-07-29 — crash sweep). UE4SS runs keybind handlers on its own
-- UpdateThread, and this handler is the heaviest thing the mod can do off it: `require("app")`
-- reparses ~60 modules — string interning, proto/closure/table allocation, incremental-GC steps —
-- for tens of milliseconds on the keybind thread while ui_core, nav, battle, quest and pad_poll
-- are all executing Lua on the SAME global_State. That is the allocator + GC race documented in
-- the construction-notify episode, and it corrupts the state to crash minutes later somewhere
-- unrelated. This bind is NOT dev-only — it is above the `if Build.debug` block and the README
-- advertises it to players, so it shipped in every release. (The stop()/start() halves are pure
-- Lua state plus bridge calls and were never the hazard; the reload is.)
--
-- Extracted from the keybind body (2026-08-06) so the dev command channel can drive the same reload
-- with no keyboard and no focused window (dev_channel.lua). It is the SAME code, not a copy —
-- a second reload path that drifted from this one would be the worst kind of bug to debug.
-- CALL ONLY ON THE GAME THREAD, for the reason above: the keybind wraps it in ExecuteInGameThread,
-- and the channel is already inside one.
local function hot_reload()
    App.stop()
    for name in pairs(package.loaded) do
        if not protected[name] then package.loaded[name] = nil end
    end
    -- PCALL'd (crash audit RANK 13, 2026-07-31). app.lua's top level requires ~25 adapter
    -- modules and runs module-scope code, so an ordinary error in any of them — a half-saved
    -- file, a renamed native_offsets field — used to propagate straight out of here: the
    -- assignment never landed, `App` still pointed at the table whose loops had just been
    -- stopped two lines above, App.start() never ran, and a blind player got TOTAL SILENCE
    -- with nothing spoken and no way back but restarting the game. The old table's closures
    -- are intact, so restarting it is a valid fallback.
    local reload_ok, newApp = pcall(require, "app")
    if reload_ok then
        App = newApp
    else
        print("[" .. MOD .. "] reload FAILED, keeping the previous version: "
            .. tostring(newApp) .. "\n")
    end
    App.start()
    -- Say which of the two actually happened: a silent "Mod reloaded" over the OLD code is
    -- exactly the report that wastes a debugging session.
    Speech.say(reload_ok and "Mod reloaded" or "Reload failed, previous version restored", true)
    return reload_ok
end

RegisterKeyBind(Key.R, { ModifierKey.CONTROL, ModifierKey.SHIFT }, function()
    ExecuteInGameThread(hot_reload)
end)

-- === Developer / diagnostic keybinds — NOT shipped in releases =================
-- build_flags.debug is true in the dev tree and false in packaged releases
-- (package.ps1 rewrites build_flags.lua and drops the dev-only modules), so none
-- of these register for players.
if Build.debug then
    -- F9: read the player's world position (pipeline test for live UObject reads).
    RegisterKeyBind(Key.F9, function()
        ExecuteInGameThread(function()
            local pc = FindFirstOf("PlayerController")
            if not Core.valid(pc) then Speech.say("No player controller", true) return end
            local pawn = Core.member(pc, "Pawn")
            if not Core.valid(pawn) then Speech.say("No player pawn", true) return end
            local loc = pawn:K2_GetActorLocation()
            Speech.say(string.format("X %d, Y %d, Z %d",
                math.floor(loc.X + 0.5), math.floor(loc.Y + 0.5), math.floor(loc.Z + 0.5)), true)
        end)
    end)

    -- Ctrl+F5: dump the guidance candidates + a NavMesh probe to dumps/dump_nav_targets.txt.
    RegisterKeyBind(Key.F5, { ModifierKey.CONTROL }, function() App.nav_dump() end)

    -- Ctrl+Shift+F5: dump the level-offset hunt windows to dumps/dump_enemy_level.txt.
    RegisterKeyBind(Key.F5, { ModifierKey.CONTROL, ModifierKey.SHIFT }, function() App.nav_dump_levels() end)

    -- F6: diagnostic — SetFontType section hook status + fire count.
    RegisterKeyBind(Key.F6, function()
        local ok, s = pcall(function() return require("header_hook").status() end)
        Speech.say(ok and s or "hook status unavailable", true)
    end)

    -- F4: runtime memory diff — finds a non-reflected selection offset (fresh each press).
    RegisterKeyBind(Key.F4, function()
        package.loaded.dev_memdiff = nil
        require("dev_memdiff").run()
    end)

    -- F7: discovery — re-requires discover.lua fresh each press and runs its current step.
    RegisterKeyBind(Key.F7, function()
        package.loaded.discover = nil
        require("discover").run()
        Speech.say("Discover", true)
    end)

    -- Ctrl+G: flip the two REFLECTION GATES in ui_core (the member-existence check and the array
    -- type check) off and on. They refuse a fetch the class does not declare, and an array read on
    -- a member that is not a real TArray — both of which are otherwise uncatchable aborts. But a
    -- gate on that path fails by making a screen go QUIET with no error, so this key answers "is
    -- the gate why this screen stopped reading?" in one press instead of one code change. The
    -- state survives Ctrl+Shift+R; the `member gate:` / `array gate:` log lines name what was
    -- refused. Does NOT touch the memory pre-check (Mem.alive) — that one is the actual crash fix
    -- and has no silent-failure mode of this kind.
    RegisterKeyBind(Key.G, { ModifierKey.CONTROL }, function() App.toggle_gates() end)

    -- Ctrl+Shift+G: flip the SEH memory pre-check (Mem.alive) off and on. That check is the actual
    -- crash fix, so it stays on normally — but a guard that wrongly refuses a LIVE object silences
    -- whatever was reading it with no error at all, and a rising "N rejected" count looks identical
    -- to the guard working. This key is how that gets ruled in or out in one press: if a screen
    -- starts reading again with the pre-check off, the pre-check was the cause. Together with Ctrl+G
    -- these two keys switch off everything the 2026-07-25 batch added to the read path.
    RegisterKeyBind(Key.G, { ModifierKey.CONTROL, ModifierKey.SHIFT }, function() App.toggle_precheck() end)

    -- File-driven command channel (dev_channel.lua) — the same diagnostics as the keys above, minus
    -- the keyboard. Every bind in this block needs the game window focused and a human at it, which
    -- is precisely what stops an external tool (tools/kakarot-mcp) from asking the mod anything
    -- about itself; a census or a nav dump then costs a round-trip through the player. Adds no
    -- keybind and no work to any existing loop: it polls its own file every 500 ms.
    -- pcall'd like the other optional installs — a broken dev tool must never stop the mod loading.
    pcall(function()
        require("dev_channel").install({
            reload = hot_reload,
            -- A GETTER, not App itself: hot_reload swaps the table, and a captured reference would
            -- have the channel driving the stopped copy from the first reload onward.
            app    = function() return App end,
        })
    end)
end

-- Global transition gate: "map switch in progress" makes every loop go inert and every
-- UObject cache flush before a tick could probe a freed object of the old level (an
-- uncatchable abort — the recurring return-to-title / post-cutscene crash). Since
-- 2026-07-25 the signal is a WORLD-EPOCH POLL on the game thread, driven from
-- ui_core.begin_scan_tick — no construction notify (its callback could run mod Lua on the
-- engine's async loading thread) and no LoadMap hook (that trampoline crashes this game).
-- Nothing to register any more; install() just announces itself. See transition.lua.
pcall(function() require("transition").install() end)

-- Field-menu section reader — the mod's ONE RegisterHook, in its own file so it can be
-- disabled by simply deleting header_hook.lua (see that file). Registered ONCE here (a reload
-- must not re-register it); the whole thing is pcall'd so even a failure to install can't stop
-- the mod from loading. Needs a full game restart to take effect (main.lua isn't hot-reloaded).
pcall(function()
    if require("header_hook").install() then
        print("[" .. MOD .. "] Header section hook registered.\n")
    end
end)

-- Start all accessibility features.
App.start()

print("[" .. MOD .. "] Lua loaded. Accessibility active.\n")
