-- dev_channel.lua — DEV-ONLY command channel for the mod's OWN diagnostics.
--
-- WHY THIS IS SEPARATE FROM tools/ue4ss-inspector. The Inspector is a second UE4SS mod. Whether
-- UE4SS hands every mod its own lua_State is not documented anywhere in this repo (only "the shared
-- lua_State" in passing, ue4ss-mod-architecture.md:162, and about a different subject), and the cost
-- of assuming wrong is a cross-VM call — so we do not assume: anything that needs THIS mod's
-- internals (the adapter registry, App, discover.lua) is driven from inside this mod. The Inspector
-- keeps what it is good at, the generic engine reflection (findall/dumpclass/probe/…), which needs
-- no mod state at all.
--
-- WHY A FILE AND NOT A SOCKET. A poll on the UE4SS worker thread that queues the real work through
-- ExecuteInGameThread is the sanctioned pattern (ue4ss-api-reference.md:144-149), and it is the only
-- shape that keeps mod Lua off a foreign thread — the exact mistake behind the construction-notify
-- crash that cost two sessions. A socket or a named pipe would put an alien thread next to the
-- lua_State for a latency win we do not need: commands are typed by a human-scale tool, not a game
-- loop. Same reason the Inspector does it this way.
--
-- PROTOCOL. One line in, tagged with a sequence number so the caller can find ITS answer in an
-- append-only log and so two identical commands in a row are still two distinct commands (the
-- Inspector's `last_cmd` dedup made the second one a no-op):
--     in   kakarot_cmd.txt   #<seq> <command> [args...]
--     out  kakarot_out.txt   <<<BEGIN seq>>> … lines … <<<END seq>>>
-- Both live next to UE4SS.log (the game's working directory), like the Inspector's pair.
--
-- Gated on build_flags.debug: package.ps1 rewrites that to false for a release AND drops this file
-- from the zip, so it never exists for a player.

local Channel = {}

local CMD_PATH = "kakarot_cmd.txt"
local OUT_PATH = "kakarot_out.txt"
local POLL_MS  = 500

--------------------------------------------------------------------------------
-- output
--------------------------------------------------------------------------------

-- Opened and closed per line, like the Inspector's: the reader on the other side is polling a file
-- that must never sit half-written in a buffer we are not going to flush for another minute.
local function out(line)
    line = tostring(line)
    print("[KakarotDev] " .. line .. "\n")
    local f = io.open(OUT_PATH, "a")
    if f then f:write(line, "\n"); f:close() end
end

--------------------------------------------------------------------------------
-- commands
--------------------------------------------------------------------------------

-- Dependencies injected by main.lua rather than reached for. `reload` is inline code in main.lua's
-- keybind (it reassigns main.lua's own `App` upvalue, which nothing else can do), and `app` is a
-- GETTER, not the table: a reload replaces App wholesale, so a captured reference would go stale on
-- the first reload and every later nav dump would run against the stopped copy.
local deps = _G.__KakarotDevDeps or {}   -- survives a reload (see Channel.install)

-- Everything else is required LAZILY, inside the command. Ctrl+Shift+R drops every non-protected
-- module from package.loaded, so a module-level `local Registry = require(...)` would pin the
-- pre-reload registry forever and report the state of a table nobody is driving any more.
local C = {}

function C.help()
    out("commands:")
    for _, l in ipairs({
        "ping                  liveness + which build is loaded",
        "screen                active adapter, registry state, last spoken lines",
        "say <text>            speak a line (pipeline test / marks a moment in the log)",
        "reload                Ctrl+Shift+R without the keyboard",
        "census                F7 discovery census -> Scripts/dumps/dump_<epoch>_<n>.txt",
        "navdump               Ctrl+F5   -> Scripts/dumps/dump_nav_targets.txt",
        "navlevels             Ctrl+Shift+F5 -> Scripts/dumps/dump_enemy_level.txt",
        "dir [substring]       screen-directory roots and chain traces (works with a menu open)",
        "where                 F5: tracked target distance + clock direction (spoken and echoed)",
        "keys                  F2: on-screen button prompts (spoken and echoed)",
        "objective             F10: objective read, the race status while racing (spoken and echoed)",
        "ray [m] [ch] [dz_m]   one LineTraceSingle ahead of the pawn (testing: is a trace safe?)",
        "drive [ms]            race test driver: steer by the beacon pan, hold RT for ms (testing)",
        "enemyhp [value]       list enemy HP; with a value, set it (testing: get past a story fight)",
        "tp <Class>|@<group>  put the player 1.5 m in front of a class instance / radar target (testing)",
        "tpxyz <x> <y> <z>     put the player at world coordinates in cm (testing)",
        "memdiff               F4 runtime memory diff -> Scripts/dev_probe.txt",
        "charnames             character name-field probe -> Scripts/dumps/dump_char_names.txt",
        "questdump             live quest phases + find-list ids -> Scripts/dumps/dump_quest_state.txt",
        "press <BTN[+BTN]> [ms] virtual pad press, e.g. press A / press DPAD_DOWN 150",
        "stick <x> <y> [ms]    virtual left stick, -1..1 each axis (y up = +)",
        "trigger <lt> <rt> [ms] virtual triggers, 0..1 each (RT accelerates the race car)",
        "commu                 community board geometry (reticle, sockets)",
        "unplug                drop the virtual pad",
    }) do out("  " .. l) end
end

-- Virtual pad: lets the assistant drive menus with nobody at the controller. The plug lease is
-- renewed by every press, so an idle session unplugs by itself.
local VPAD_PLUG_MS = 10 * 60 * 1000
local last_press = nil
function C.press(btns, ms)
    local Input = require("input")
    local mask = 0
    for nm in tostring(btns or ""):gmatch("[^+]+") do
        local b = Input.BTN[nm:upper()]
        if not b then return out("unknown button: " .. nm) end
        mask = mask | b
    end
    if mask == 0 then return out("usage: press <BTN[+BTN]> [ms]") end
    Input.vpad_plug(VPAD_PLUG_MS)
    local ok = Input.vpad_press(mask, tonumber(ms) or 120)
    last_press = os.clock()
    out(string.format("press 0x%04X hooked=%s", mask, tostring(ok)))
end

function C.stick(x, y, ms)
    local Input = require("input")
    Input.vpad_plug(VPAD_PLUG_MS)
    local ok = Input.vpad_stick(tonumber(x) or 0, tonumber(y) or 0, tonumber(ms) or 200)
    last_press = os.clock()
    out(string.format("stick %s,%s for %s ms hooked=%s", tostring(x), tostring(y), tostring(ms), tostring(ok)))
end

function C.trigger(lt, rt, ms)
    local Input = require("input")
    Input.vpad_plug(VPAD_PLUG_MS)
    local ok = Input.vpad_trigger(tonumber(lt) or 0, tonumber(rt) or 0, tonumber(ms) or 200)
    last_press = os.clock()
    out(string.format("trigger %s,%s for %s ms hooked=%s", tostring(lt), tostring(rt), tostring(ms), tostring(ok)))
end

function C.story()
    for _, l in ipairs(require("screen_story").dev_state()) do out(l) end
end

function C.commu()
    for _, l in ipairs(require("screen_community").dev_board()) do out(l) end
end

function C.unplug()
    require("input").vpad_plug(0)
    out("unplugged")
end

function C.ping()
    out("pong")
    local ok, Build = pcall(require, "build_flags")
    out("debug build : " .. tostring(ok and Build and Build.debug))
end

-- The single most-asked diagnostic in this codebase: "which adapter owns the screen, and what did it
-- say?" The playbook's own shortcut for a silent screen is to grep the log for `screen -> <adapter>`;
-- this answers it directly, and pairs it with the speech tail so "the adapter claims the tick and
-- then says nothing" — a failure this mod has hit repeatedly — is one command instead of two.
function C.screen()
    local ok, Registry = pcall(require, "ui_registry")
    if not ok or not Registry then return out("ui_registry unavailable: " .. tostring(Registry)) end
    -- Every accessor is feature-tested: after a failed reload the registry in package.loaded can be
    -- an older build than this file expects, and a nil call here would take the channel down with it.
    local name = Registry.active_name and Registry.active_name()
    out("adapter : " .. (name or "(none - free roam, or nothing committed)"))
    out("index   : " .. tostring(Registry.active_index and Registry.active_index()))
    out("enabled : " .. tostring(Registry.is_enabled and Registry.is_enabled()))
    out("hot     : " .. tostring(Registry.hot and Registry.hot()))

    local sok, Speech = pcall(require, "speech")
    if sok and Speech and Speech.recent then
        out(string.format("last spoken (newest first; [clock, ! = interrupt]) now=%.2f last press=%s:",
            os.clock(), last_press and string.format("%.2f", last_press) or "-"))
        local lines = Speech.recent(8)
        if not lines or #lines == 0 then
            out("   (nothing yet)")
        else
            for i, l in ipairs(lines) do out(("   %d. %s"):format(i, l)) end
        end
    end
end

function C.say(...)
    local text = table.concat({ ... }, " ")
    if text == "" then return out("usage: say <text>") end
    local ok, Speech = pcall(require, "speech")
    if not ok or not Speech then return out("speech unavailable") end
    Speech.say(text, true)
    out("said: " .. text)
end

function C.reload()
    if not deps.reload then return out("reload unavailable (no dep injected)") end
    -- Already on the game thread here — the poll loop queued us through ExecuteInGameThread — which
    -- is the whole reason the keybind wraps this and we do not: re-requiring ~60 modules off the
    -- game thread is the allocator/GC race documented at main.lua:205-213.
    local ok, err = pcall(deps.reload)
    out(ok and "reloaded" or ("reload raised: " .. tostring(err)))
end

function C.census()
    package.loaded.discover = nil
    local ok, mod = pcall(require, "discover")
    if not ok or not mod then return out("discover.lua unavailable: " .. tostring(mod)) end
    -- run() wraps its work in its own ExecuteInGameThread, so the file appears a tick or two after
    -- this returns. It reports no path, so the caller finds the newest file in the dumps folder.
    local rok, err = pcall(mod.run)
    out(rok and "census queued -> newest file in Scripts/dumps/" or ("census raised: " .. tostring(err)))
end

function C.navdump()
    local App = deps.app and deps.app()
    if not App or not App.nav_dump then return out("App.nav_dump unavailable") end
    local ok, err = pcall(App.nav_dump)
    out(ok and "nav dump -> Scripts/dumps/dump_nav_targets.txt" or ("raised: " .. tostring(err)))
end

-- F5 without the keyboard: speak the tracked target's distance and clock direction, and echo it.
function C.where()
    local App = deps.app and deps.app()
    if not App or not App.nav_where then return out("App.nav_where unavailable") end
    local ok, err = pcall(App.nav_where)
    if not ok then return out("raised: " .. tostring(err)) end
    local sok, Speech = pcall(require, "speech")
    local last = sok and Speech.recent and Speech.recent(1)[1]
    out(last or "(nothing spoken)")
end

-- Test shortcut: list enemy HP and optionally set it (`enemyhp 1`) to get a test save past a story
-- fight. HP is the native f32 at StatusInstance+0x394, enemy instances only (STATUS derived facts).
function C.enemyhp(value, name_part)
    local Core, Mem, Nav = require("ui_core"), require("mem"), require("nav_tracker")
    local HP_OFF = 0x394
    for _, c in ipairs(FindAllOf("AT_Character") or {}) do
        if Core.valid(c) and Nav._char_side(c) == "enemy"
            and (not name_part or tostring(c:GetFullName()):find(name_part, 1, true)) then
            local a = Core.member(c, "AttributeComponent")
            local si = Core.valid(a) and Core.member(a, "StatusInstance")
            if Core.valid(si) then
                local hp = Mem.float(si, HP_OFF)
                local line = string.format("%s hp=%s", tostring(c:GetFullName()), tostring(hp))
                if tonumber(value) and hp and hp > 0 then
                    line = line .. " set=" .. tostring(Mem.write_float(si, HP_OFF, tonumber(value)))
                end
                out(line)
            end
        end
    end
end

-- Race test driver: steer toward the next checkpoint with the SAME heading-relative pan the race
-- beacon plays, holding RT for `ms`. Echoes what the beacon is being fed (testing only).
function C.drive(ms)
    local Core, Beacon, Input = require("ui_core"), require("beacon"), require("input")
    ms = tonumber(ms) or 400
    local host
    for _, w in ipairs(Core.peek_all("AT_UIMiniGameRace")) do
        if Core.valid(w) and Core.on_screen(w) then host = w end
    end
    if not host then return out("no race HUD") end
    local n, p = host:GetNextCheckPointLocation(), host:GetPlayerLocation()
    local pc = require("ui_directory").peek("hud", "PlayerOwner")
    local pawn = Core.valid(pc) and Core.member(pc, "Pawn")
    local yaw = Core.valid(pawn) and pawn:K2_GetActorRotation().Yaw or 0
    local dx, dy, dz = n.X - p.X, n.Y - p.Y, n.Z - p.Z
    local pan, fb = Beacon.cam_rel(dx, dy, math.cos(math.rad(yaw)), math.sin(math.rad(yaw)))
    local steer = math.max(-1, math.min(1, pan * 1.6))
    if fb < 0 then steer = pan >= 0 and 1 or -1 end
    Input.vpad_plug(VPAD_PLUG_MS)
    Input.vpad_stick(steer, 0, ms)
    Input.vpad_trigger(0, 1, ms)
    out(string.format("cp=%s dh=%.0fm dz=%.0fm pan=%.2f fb=%.2f steer=%.2f yaw=%.0f pos=%.0f,%.0f,%.0f",
        tostring(Core.read_text(Core.member(host, "WL_Txt_Num_Cp"))), math.sqrt(dx * dx + dy * dy) / 100,
        dz / 100, pan, fb, steer, yaw, p.X, p.Y, p.Z))
end

-- One LineTraceSingle (channel overload) straight ahead of the pawn, to settle whether a
-- reflected trace survives on this build (engine-facts: they aborted in July). The breadcrumb
-- file is written BEFORE the call, so a dead process leaves "calling" behind.
function C.ray(dist_m, ch, dz_m)
    local Core = require("ui_core")
    local pc = require("ui_directory").peek("hud", "PlayerOwner")
    local pawn = Core.valid(pc) and Core.member(pc, "Pawn")
    if not Core.valid(pawn) then return out("no player pawn") end
    local loc, rot = pawn:K2_GetActorLocation(), pawn:K2_GetActorRotation()
    local d = (tonumber(dist_m) or 20) * 100
    local yaw = math.rad(rot.Yaw)
    local sx, sy, sz = loc.X + math.cos(yaw) * 150, loc.Y + math.sin(yaw) * 150, loc.Z + 50
    local f = io.open("kakarot_ray_crumb.txt", "w")
    if f then f:write("calling channel=" .. tostring(ch or 0) .. "\n") f:close() end
    local k = StaticFindObject("/Script/Engine.Default__KismetSystemLibrary")
    local hitres = {}
    local ok, hit = pcall(function()
        return k:LineTraceSingle(pawn, { X = sx, Y = sy, Z = sz },
            { X = sx + math.cos(yaw) * d, Y = sy + math.sin(yaw) * d, Z = sz + (tonumber(dz_m) or 0) * 100 },
            tonumber(ch) or 0, false, {}, 0, hitres, true,
            { R = 0, G = 0, B = 0, A = 0 }, { R = 0, G = 0, B = 0, A = 0 }, 0.0)
    end)
    f = io.open("kakarot_ray_crumb.txt", "w")
    if f then f:write("returned ok=" .. tostring(ok) .. "\n") f:close() end
    out(string.format("ok=%s hit=%s (%s) from %.0f,%.0f,%.0f yaw %.0f", tostring(ok), tostring(hit), type(hit), sx, sy, sz, rot.Yaw))
    for k2, v in pairs(hitres) do
        local s = tostring(v)
        if type(v) == "table" or type(v) == "userdata" then
            pcall(function() s = string.format("%.1f,%.1f,%.1f", v.X, v.Y, v.Z) end)
        end
        out("  " .. tostring(k2) .. " = " .. s)
    end
end

-- F10 without the keyboard: the objective read (the race status while racing), echoed.
function C.objective()
    local App = deps.app and deps.app()
    if not App or not App.read_objective then return out("App.read_objective unavailable") end
    local ok, err = pcall(App.read_objective)
    if not ok then return out("raised: " .. tostring(err)) end
    local sok, Speech = pcall(require, "speech")
    out((sok and Speech.recent and Speech.recent(1)[1]) or "(nothing spoken)")
end

-- F2 without the keyboard: read the on-screen button prompts, and echo them.
function C.keys()
    local App = deps.app and deps.app()
    if not App or not App.read_keyhelp then return out("App.read_keyhelp unavailable") end
    local ok, err = pcall(App.read_keyhelp)
    if not ok then return out("raised: " .. tostring(err)) end
    local sok, Speech = pcall(require, "speech")
    out((sok and Speech.recent and Speech.recent(1)[1]) or "(nothing spoken)")
end

-- Test shortcut: put the player 1.5 m in front of, and facing, the first live instance of a class, or of the
-- first radar target in a group (`tp @minigames`).
function C.tp(cls, name_part)
    if not cls then return out("usage: tp <Class> [name substring] | tp @<radar group>") end
    local Core = require("ui_core")
    local target
    if cls:sub(1, 1) == "@" then
        for _, g in ipairs(require("nav_tracker").targets_cached() or {}) do
            if g.key == cls:sub(2) then
                for _, it in ipairs(g.items) do
                    if Core.valid(it.actor) then target = it.actor break end
                end
            end
        end
    elseif name_part then
        for _, o in ipairs(FindAllOf(cls) or {}) do
            if Core.valid(o) and tostring(o:GetFullName()):find(name_part, 1, true) then target = o break end
        end
    else
        target = FindFirstOf(cls)
    end
    if not Core.valid(target) then return out("no live " .. cls) end
    out("target: " .. tostring(target:GetFullName()))
    local mm = Core.peek_all("AT_UIMiniMapRadar")[1]
    local pawn = Core.valid(mm) and Core.member(mm, "PlayerIns")
    if not Core.valid(pawn) then return out("no player pawn (minimap down?)") end
    local loc, rot = target:K2_GetActorLocation(), target:K2_GetActorRotation()
    local yaw = math.rad(rot.Yaw or 0)
    local dest = { X = loc.X + math.cos(yaw) * 150, Y = loc.Y + math.sin(yaw) * 150, Z = loc.Z + 100 }
    pawn:K2_SetActorLocation(dest, false, {}, true)
    -- Face the target, so an interaction prompt is in front of the player.
    pawn:K2_SetActorRotation({ Pitch = 0, Yaw = math.deg(yaw) + 180, Roll = 0 }, true)
    out(string.format("tp -> %.0f %.0f %.0f (target %.0f %.0f %.0f)", dest.X, dest.Y, dest.Z,
        loc.X, loc.Y, loc.Z))
end

-- Put the player at raw world coordinates (cm), e.g. a door's playerStart from the nav dump.
function C.tpxyz(x, y, z)
    x, y, z = tonumber(x), tonumber(y), tonumber(z)
    if not (x and y and z) then return out("usage: tpxyz <x> <y> <z>") end
    local Core = require("ui_core")
    local mm = Core.peek_all("AT_UIMiniMapRadar")[1]
    local pawn = Core.valid(mm) and Core.member(mm, "PlayerIns")
    if not Core.valid(pawn) then return out("no player pawn (minimap down?)") end
    pawn:K2_SetActorLocation({ X = x, Y = y, Z = z }, false, {}, true)
    local l = pawn:K2_GetActorLocation()
    out(string.format("tpxyz -> %.0f %.0f %.0f", l.X, l.Y, l.Z))
end

-- Liveness of every cached instance of a class: on_screen (ancestor walk), own visibility enum,
-- render opacity, pane_live and pane_rendered. For "why does this host still claim the tick?".
function C.vis(cls, member)
    if not cls then return out("usage: vis <Class> [member]") end
    local Core = require("ui_core")
    local list = Core.cached_all(cls)
    if member then
        local w = {}
        for _, o in ipairs(list or {}) do w[#w + 1] = Core.member(o, member) end
        list = w
    end
    out(string.format("%s: %d cached", cls, #(list or {})))
    for i, o in ipairs(list or {}) do
        if Core.valid(o) then
            local v, op
            pcall(function() v = o:GetVisibility() end)
            pcall(function() op = o:GetRenderOpacity() end)
            local n = "?"
            pcall(function() n = o:GetFName():ToString() end)
            out(string.format("  [%d] %s on_screen=%s vis=%s op=%s live=%s rendered=%s", i, n,
                tostring(Core.on_screen(o)), tostring(v), tostring(op),
                tostring(Core.pane_live(o)), tostring(Core.pane_rendered(o))))
        else
            out(string.format("  [%d] invalid", i))
        end
    end
end

-- The directory part of the nav dump, which the dump skips whenever no world is "alive" (menus).
function C.dir(filter)
    local ok, lines = pcall(function() return require("ui_directory").debug_lines() end)
    if not ok then return out("raised: " .. tostring(lines)) end
    local Core = require("ui_core")
    for _, l in ipairs(lines) do
        if not filter or l:find("root ", 1, true) or l:find(filter, 1, true) then out(l) end
        -- What the adapters actually get back: resolve() gates its hops, the trace above does not.
        local cls = filter and l:match("^%s+([%w_]+_C)%s") or nil
        if cls and l:find(filter, 1, true) then
            local list = Core.cached_all(cls)
            local flags = {}
            for _, o in ipairs(list or {}) do flags[#flags + 1] = Core.on_screen(o) and "ON" or "off" end
            out(string.format("    cached_all -> %d [%s]", #(list or {}), table.concat(flags, " ")))
        end
    end
end

function C.navlevels()
    local App = deps.app and deps.app()
    if not App or not App.nav_dump_levels then return out("App.nav_dump_levels unavailable") end
    local ok, err = pcall(App.nav_dump_levels)
    out(ok and "level dump -> Scripts/dumps/dump_enemy_level.txt" or ("raised: " .. tostring(err)))
end

function C.memdiff()
    package.loaded.dev_memdiff = nil
    local ok, mod = pcall(require, "dev_memdiff")
    if not ok or not mod then return out("dev_memdiff.lua unavailable: " .. tostring(mod)) end
    local rok, err = pcall(mod.run)
    out(rok and "memdiff -> Scripts/dev_probe.txt" or ("memdiff raised: " .. tostring(err)))
end

function C.charnames()
    package.loaded.dev_charnames = nil
    local ok, mod = pcall(require, "dev_charnames")
    if not ok or not mod then return out("dev_charnames.lua unavailable: " .. tostring(mod)) end
    -- run() queues its own ExecuteInGameThread, so the file lands a tick or two after this returns.
    local rok, err = pcall(mod.run)
    out(rok and "char name probe -> Scripts/dumps/dump_char_names.txt"
             or ("charnames raised: " .. tostring(err)))
end

function C.questdump()
    package.loaded.dev_questdump = nil
    local ok, mod = pcall(require, "dev_questdump")
    if not ok or not mod then return out("dev_questdump.lua unavailable: " .. tostring(mod)) end
    -- run() queues its own ExecuteInGameThread, so the file lands a tick or two after this returns.
    local rok, err = pcall(mod.run)
    out(rok and "quest dump -> Scripts/dumps/dump_quest_state.txt"
             or ("questdump raised: " .. tostring(err)))
end

--------------------------------------------------------------------------------
-- dispatch
--------------------------------------------------------------------------------

local function dispatch(line)
    local args = {}
    for w in tostring(line):gmatch("%S+") do args[#args + 1] = w end
    local cmd = table.remove(args, 1)
    if not cmd then return end
    local fn = C[cmd:lower()]
    if not fn then return out("unknown command: " .. cmd .. " (try 'help')") end
    -- NEVER NEST ExecuteInGameThread FROM HERE (2026-09-08; it froze the game twice: `navdump`
    -- on 2026-08-18 and `questdump` today, same signature both times — the channel answered, the
    -- dump file was never even CREATED, both channels went silent, process still up). Dispatch
    -- already runs inside a game-thread action, and every dump module wraps its body in its own
    -- ExecuteInGameThread for the KEYBIND path. RE-UE4SS 3.0.1 drains the action vector inside a
    -- `std::remove_if` and runs each callback from within it (LuaMod.cpp:2919-2948,
    -- process_event_hook); a nested call does `emplace_back` on that same vector under the
    -- iterators, which reallocates whenever capacity is exhausted — UB that shows up as a hang.
    -- It works most of the time (spare capacity), which is why `census` never seemed to mind.
    -- So while a command runs, the global is shadowed with "call it now": we ARE the game thread.
    -- The shim reaches every module at once because none captures the global at load time
    -- (grep `= ExecuteInGameThread` before adding one that does).
    local real_eigt = ExecuteInGameThread
    ExecuteInGameThread = function(cb) return cb() end
    local ok, err = pcall(fn, table.unpack(args))
    ExecuteInGameThread = real_eigt
    if not ok then out("error: " .. tostring(err)) end
end

--------------------------------------------------------------------------------
-- install
--------------------------------------------------------------------------------

-- The poll runs as a pad_poll stepper, i.e. ON THE GAME THREAD (2026-10-16). It used to be its own
-- LoopAsync that read the file on the worker thread and queued each command with
-- ExecuteInGameThread: worker-side Lua on the shared state plus a queue push per command, the two
-- access classes pad_poll.lua documents as the session-ending race. Thirty minutes of scripted
-- presses (one command a second) froze the whole mod silently, with the process still up.
-- App.start calls attach() so the stepper lands in the current pad_poll after every reload.
local next_poll = 0
local function poll_step()
    local now = os.clock()
    if now < next_poll then return end
    next_poll = now + POLL_MS / 1000
    local f = io.open(CMD_PATH, "r")
    if not f then return end
    local line = f:read("*l")
    f:close()
    if not line or line == "" or line == _G.__KakarotDevLastCmd then return end
    _G.__KakarotDevLastCmd = line
    local seq, rest = line:match("^#(%S+)%s+(.*)$")
    -- pcall around the WHOLE body, including the markers: if anything in here raises, the END
    -- marker must still be written or the caller waits out its full timeout.
    pcall(function()
        if seq then out("<<<BEGIN " .. seq .. ">>>") end
        dispatch(rest or line)
    end)
    if seq then pcall(out, "<<<END " .. seq .. ">>>") end
end

function Channel.attach()
    if not _G.__KakarotDevChannel then return end
    require("pad_poll").register("dev_channel", poll_step)
end

function Channel.install(d)
    -- ONE install, ever: main.lua runs this once at boot, and the global flag survives a reload
    -- (which nils package.loaded.dev_channel). The poll itself is re-attached by App.start.
    deps = d or {}
    _G.__KakarotDevDeps = deps
    if _G.__KakarotDevChannel then
        out("dev channel already installed")
        return false
    end
    _G.__KakarotDevChannel = true

    -- PRIMED WITH WHATEVER IS ALREADY IN THE FILE (2026-08-15). Starting this at nil meant any
    -- command left over from the previous session looked NEW to the very first poll, so the last
    -- thing typed before a restart re-ran ~1 second into boot — on a half-loaded world, which is
    -- the single most fragile moment there is, and with no human expecting it. That is exactly how
    -- a stale `navdump` fired at 13:32:33 against a game that had started at 13:32:32. A command
    -- file is a MAILBOX, not a queue: at startup its contents are history, not instructions.
    local f0 = io.open(CMD_PATH, "r")
    local last_cmd = f0 and f0:read("*l") or nil
    if f0 then f0:close() end
    if last_cmd and last_cmd ~= "" then
        out("ignoring stale command from a previous session: " .. last_cmd)
    end
    _G.__KakarotDevLastCmd = last_cmd
    out("dev channel ready - write a command into " .. CMD_PATH .. ", read " .. OUT_PATH)
    return true
end

return Channel
