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
        "autodrive [rt|stick|both|off] race autopilot on the guide's own pan, every tick (testing)",
        "survey x0 y0 x1 y1 step name [ch] [ztop]  downward traces -> dumps/survey_<name>.txt (race paths)",
        "msurvey x0 y0 x1 y1 step name [layers]  multi-layer surfaces + headroom -> dumps/msurvey_<name>.txt",
        "racelevel [filter]   race level actors, race-only meshes, splines -> dumps/dump_race_level.txt",
        "course | volgeom [cls] [part] | views | rmove  race course, volume hulls, race cameras, robot limits",
        "robot x y z [yaw] | rtrack secs [sx sy jump] | pos | walkto x y [ms]  robot/pawn tests (metres)",
        "enemyhp [value]       list enemy HP; with a value, set it (testing: get past a story fight)",
        "tp <Class>|@<group>  put the player 1.5 m in front of a class instance / radar target (testing)",
        "tpxyz <x> <y> <z>     put the player at world coordinates in cm (testing)",
        "bounds <Class> [name] world bounds of one actor's colliding components (testing)",
        "memdiff               F4 runtime memory diff -> Scripts/dev_probe.txt",
        "charnames             character name-field probe -> Scripts/dumps/dump_char_names.txt",
        "questdump             live quest phases + find-list ids -> Scripts/dumps/dump_quest_state.txt",
        "items <text>|<id>     drop slugs whose localized item name contains <text> (or one id's name)",
        "drops <slug substr>   live pickups/animals whose drop slug matches, with distance",
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
    -- Prefer what the guide is playing right now (booster or checkpoint, its own heading source),
    -- so the driver tests exactly what the player hears.
    local aim = require("race_guide")._aim
    local src = "own"
    if aim and os.clock() - aim.at < 1 then pan, fb, src = aim.pan, aim.fb, aim.ring and ("ring " .. aim.ring) or "cp" end
    local steer = math.max(-1, math.min(1, pan * 1.6))
    if fb < 0 then steer = pan >= 0 and 1 or -1 end
    Input.vpad_plug(VPAD_PLUG_MS)
    Input.vpad_stick(steer, 0, ms)
    Input.vpad_trigger(0, 1, ms)
    out(string.format("cp=%s aim=%s dh=%.0fm dz=%.0fm pan=%.2f fb=%.2f steer=%.2f yaw=%.0f pos=%.0f,%.0f,%.0f",
        tostring(Core.read_text(Core.member(host, "WL_Txt_Num_Cp"))), src, math.sqrt(dx * dx + dy * dy) / 100,
        dz / 100, pan, fb, steer, yaw, p.X, p.Y, p.Z))
end

-- Boost rings of the current race (positions in m, used flag) and the racer's position.
function C.rings()
    local rings, used = require("race_rings")._dump()
    if not rings then return out("no rings") end
    for i, r in ipairs(rings) do
        out(string.format("ring %d: %.0f %.0f %.0f%s", i, r[1] / 100, r[2] / 100, r[3] / 100,
            used and used[i] and " used" or ""))
    end
    local aim = require("race_guide")._aim
    if aim then out(string.format("aim pan=%.2f fb=%.2f ring=%s", aim.pan, aim.fb, tostring(aim.ring))) end
end

-- Race heading probe: robot actor yaw, player pawn yaw, camera yaw and the racer's HUD position.
function C.racedbg()
    local Core = require("ui_core")
    local function yaw_of(o)
        local y
        pcall(function() y = o:K2_GetActorRotation().Yaw end)
        return y
    end
    local robot
    for _, a in ipairs(FindAllOf("TwinFootBP_C") or {}) do
        if Core.valid(a) and not tostring(a:GetFullName()):find("Default__", 1, true) then robot = a end
    end
    local pc = require("ui_directory").peek("hud", "PlayerOwner")
    local pawn = Core.valid(pc) and Core.member(pc, "Pawn")
    local cam = Core.valid(pc) and Core.member(pc, "PlayerCameraManager")
    local cyaw
    pcall(function() cyaw = cam:GetCameraRotation().Yaw end)
    local host
    for _, w in ipairs(Core.peek_all("AT_UIMiniGameRace")) do if Core.valid(w) then host = w end end
    local p
    pcall(function() p = host:GetPlayerLocation() end)
    local rl
    pcall(function() rl = robot:K2_GetActorLocation() end)
    out(string.format("robot yaw=%s at %s | pawn %s yaw=%s | cam yaw=%s | racer %s",
        tostring(robot and yaw_of(robot)), rl and string.format("%.0f,%.0f", rl.X, rl.Y) or "-",
        pawn and tostring(pawn:GetClass():GetFName():ToString()) or "-", tostring(pawn and yaw_of(pawn)),
        tostring(cyaw), p and string.format("%.0f,%.0f,%.0f", p.X, p.Y, p.Z) or "-"))
end

-- Race course dump: every course arrow (StaticMeshActor STM_mg_racing_Arrow*), checkpoint, boost
-- point, restart point and water volume, with position (cm) and yaw, to dumps/dump_race_course.txt.
function C.course()
    local Core = require("ui_core")
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    local f = io.open(dir .. "\\dumps\\dump_race_course.txt", "w")
    if not f then return out("cannot write") end
    local n = 0
    local function put(kind, a)
        local loc, rot, name
        pcall(function() loc = a:K2_GetActorLocation() end)
        pcall(function() rot = a:K2_GetActorRotation() end)
        pcall(function() name = a:GetFName():ToString() end)
        if loc then
            f:write(string.format("%s %s %.0f %.0f %.0f %.1f\n", kind, tostring(name), loc.X, loc.Y, loc.Z,
                rot and rot.Yaw or 0))
            n = n + 1
        end
    end
    for _, a in ipairs(FindAllOf("StaticMeshActor") or {}) do
        local name = ""
        pcall(function() name = a:GetFName():ToString() end)
        if name:find("racing_Arrow", 1, true) then put("arrow", a) end
    end
    for _, cls in ipairs({ "BP_CheckPoint_C", "BP_BoostPoint_C", "TargetPoint", "UnderWaterRestartVolume" }) do
        for _, a in ipairs(FindAllOf(cls) or {}) do
            if Core.valid(a) then
                local full = ""
                pcall(function() full = a:GetFullName() end)
                if full:find("RaceGame", 1, true) then put(cls, a) end
            end
        end
    end
    f:close()
    out("course dump: " .. n .. " actors")
end

-- Everything the race level places (full name containing `filter`, default "RaceGame"): class
-- counts, then every non-mesh actor with its location, then every spline component in the world
-- with its owner and point count -> dumps/dump_race_level.txt. Looks for the course's own route
-- data (a guide spline, AI path, restart points). Written line by line: a breadcrumb trail.
function C.racelevel(filter)
    filter = filter or "RaceGame"
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    local path = dir .. "\\dumps\\dump_race_level.txt"
    local f = io.open(path, "w")
    if not f then return out("cannot write") end
    local function w(s) f:write(s, "\n"); f:flush() end
    local counts, n = {}, 0
    w("step: actors")
    for _, a in ipairs(FindAllOf("Actor") or {}) do
        local full = ""
        pcall(function() full = a:GetFullName() end)
        if full:find(filter, 1, true) then
            local cls = full:match("^(%S+)") or "?"
            counts[cls] = (counts[cls] or 0) + 1
            n = n + 1
            local loc
            pcall(function() loc = a:K2_GetActorLocation() end)
            local where = loc and string.format("%.0f %.0f %.0f", loc.X, loc.Y, loc.Z) or "?"
            if cls ~= "StaticMeshActor" then
                w(string.format("actor %s %s", full, where))
            elseif not full:find("racing_Arrow", 1, true) then
                local mesh, rot, o, e = "?", nil, {}, {}
                pcall(function() mesh = a.StaticMeshComponent.StaticMesh:GetFName():ToString() end)
                pcall(function() rot = a:K2_GetActorRotation() end)
                pcall(function() a:GetActorBounds(false, o, e) end)
                w(string.format("mesh %s %s at %s yaw %.0f bounds %.0f %.0f %.0f ext %.0f %.0f %.0f",
                    full:match("([^%.:]+)$") or full, mesh, where, rot and rot.Yaw or 0,
                    o.X or 0, o.Y or 0, o.Z or 0, e.X or 0, e.Y or 0, e.Z or 0))
            end
        end
    end
    w("step: counts")
    for cls, c in pairs(counts) do w(string.format("count %s %d", cls, c)) end
    w("step: splines")
    for _, s in ipairs(FindAllOf("SplineComponent") or {}) do
        local full, pts = "", -1
        pcall(function() full = s:GetFullName() end)
        if not full:find("Default__", 1, true) then
            pcall(function() pts = s:GetNumberOfSplinePoints() end)
            local l0, l1
            pcall(function() l0 = s:GetLocationAtSplinePoint(0, 1) end)
            pcall(function() l1 = s:GetLocationAtSplinePoint(pts - 1, 1) end)
            w(string.format("spline %s pts %d from %s to %s", full, pts,
                l0 and string.format("%.0f,%.0f,%.0f", l0.X, l0.Y, l0.Z) or "?",
                l1 and string.format("%.0f,%.0f,%.0f", l1.X, l1.Y, l1.Z) or "?"))
        end
    end
    w("step: done")
    f:close()
    out("race level: " .. n .. " actors -> " .. path)
end

-- Race NavMesh probe: register the robot as a navigation invoker (radius gen_m), then ask the
-- Human mesh for a path from the robot to the next checkpoint (or to x y z in metres). Prints
-- every corner. Run it twice: tiles take a few frames to build after the first registration.
function C.racepath(gen_m, x, y, z, pawn_ctx)
    local Core = require("ui_core")
    local robot
    for _, a in ipairs(FindAllOf("TwinFootBP_C") or {}) do
        if Core.valid(a) and not tostring(a:GetFullName()):find("Default__", 1, true) then robot = a end
    end
    if not robot then return out("no robot") end
    local p = robot:K2_GetActorLocation()
    local tx, ty, tz
    if x then
        tx, ty, tz = tonumber(x) * 100, tonumber(y) * 100, tonumber(z) * 100
    else
        for _, w in ipairs(Core.peek_all("AT_UIMiniGameRace")) do
            if Core.valid(w) then
                pcall(function() local v = w:GetNextCheckPointLocation() tx, ty, tz = v.X, v.Y, v.Z end)
            end
        end
    end
    if not tx then return out("no target") end
    local cdo = StaticFindObject("/Script/NavigationSystem.Default__NavigationSystemV1")
    local nav
    pcall(function() nav = cdo:GetNavigationSystem(robot) end)
    if not Core.valid(nav) then return out("no nav system") end
    local gen = (tonumber(gen_m) or 150) * 100
    pcall(function() nav:RegisterNavigationInvoker(robot, gen, gen + 2000) end)
    local meshes = {}
    for _, a in pairs(Core.findall("RecastNavMesh")) do
        if Core.valid(a) then meshes[#meshes + 1] = a end
    end
    out(string.format("robot %.0f,%.0f,%.0f -> target %.0f,%.0f,%.0f (%d meshes)", p.X / 100, p.Y / 100,
        p.Z / 100, tx / 100, ty / 100, tz / 100, #meshes))
    for _, mesh in ipairs(meshes) do
        local mname = mesh:GetFName():ToString()
        local function proj(px, py, pz)
            local r
            pcall(function()
                local q = nav:ProjectPointToNavigation(robot, { X = px, Y = py, Z = pz }, mesh, nil,
                    { X = 500, Y = 500, Z = 2000 })
                if q and not (q.X == 0 and q.Y == 0 and q.Z == 0) then r = q end
            end)
            return r
        end
        local s, e = proj(p.X, p.Y, p.Z), proj(tx, ty, tz)
        out(string.format("%s: start %s end %s", mname, s and "on mesh" or "off", e and "on mesh" or "off"))
        local path
        local pc = require("ui_directory").peek("hud", "PlayerOwner")
        local querier = (pawn_ctx == "pawn" and Core.valid(pc)) and Core.member(pc, "Pawn") or robot
        local okc, err = pcall(function()
            path = nav:FindPathToLocationSynchronously(querier, s and { X = s.X, Y = s.Y, Z = s.Z } or p,
                e and { X = e.X, Y = e.Y, Z = e.Z } or { X = tx, Y = ty, Z = tz }, mesh, nil)
        end)
        if not okc then out("  findpath error: " .. tostring(err)) end
        out("  path object valid=" .. tostring(Core.valid(path)))
        local arr, n = Core.array_of(path, "PathPoints")
        local partial
        pcall(function() partial = path:IsPartial() end)
        out(string.format("  path %d points, partial=%s", n or 0, tostring(partial)))
        for i = 1, math.min(n or 0, 60) do
            local q = arr[i]
            if Core.valid_ref(q) then
                out(string.format("  %d: %.0f %.0f %.0f", i, q.X / 100, q.Y / 100, q.Z / 100))
            end
        end
    end
end

-- Terrain survey for race paths: a downward Visibility trace every `step` metres over the box
-- (x0,y0)-(x1,y1) in metres, from 250 m down to -50 m. Appends "x y z nz water" lines (cm, normal Z,
-- 1 when the ground point lies inside the race's UnderWaterRestartVolume) to dumps/race_survey.txt.
-- Run it while the race map is loaded (a race paused under the pause menu).
-- Multi-layer terrain survey (metres in, cm out): per cell, every surface a downward trace meets
-- from the sky, then from 0.5 m under each surface found (a trace that starts inside a solid
-- passes through its back faces, so it meets the next surface down), up to `layers`; and for each
-- lower surface, the headroom up to whatever is above it (an upward trace). Line format:
-- "x y z normalZ layer headroom" (headroom -1 = open sky). Built after a single top-down survey
-- read dinosaur bones and rock-arch decks as the only ground (live 2026-11-20).
function C.msurvey(x0, y0, x1, y1, step, name, layers)
    local Core = require("ui_core")
    local Ray = require("raycast")
    x0, y0, x1, y1 = tonumber(x0) * 100, tonumber(y0) * 100, tonumber(x1) * 100, tonumber(y1) * 100
    step = (tonumber(step) or 2) * 100
    layers = tonumber(layers) or 3
    local ctx
    for _, a in ipairs(FindAllOf("TwinFootBP_C") or {}) do
        if Core.valid(a) and not tostring(a:GetFullName()):find("Default__", 1, true) then ctx = a end
    end
    if not ctx then
        local pc = FindFirstOf("PlayerController")
        ctx = Core.valid(pc) and Core.member(pc, "Pawn") or nil
    end
    if not Core.valid(ctx) then return out("no world context") end
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    local f = io.open(dir .. "\\dumps\\msurvey_" .. (name or "race") .. ".txt", "a")
    if not f then return out("cannot write") end
    local t0, n, surf = os.clock(), 0, 0
    local y = y0
    while y <= y1 do
        local x = x0
        while x <= x1 do
            n = n + 1
            local start, above = 25000, nil
            for layer = 1, layers do
                local hit, dist, nz = Ray.probe_surface(ctx, x, y, start, x, y, -5000, 0)
                if not hit or not dist then break end
                local z = start - dist
                local head = -1
                if above then
                    local uh, ud = Ray.probe_surface(ctx, x, y, z + 20, x, y, above + 100, 0)
                    head = (uh and ud) and (ud + 20) or (above - z)
                end
                f:write(string.format("%.0f %.0f %.0f %.2f %d %.0f\n", x, y, z, nz or -1, layer, head))
                surf = surf + 1
                above, start = z, z - 50
                if start < -4000 then break end
            end
            x = x + step
        end
        y = y + step
    end
    f:close()
    out(string.format("msurvey: %d cells, %d surfaces, %.2f s", n, surf, os.clock() - t0))
end

function C.survey(x0, y0, x1, y1, step, name, ch, ztop)
    local Core = require("ui_core")
    -- ztop (metres): start the traces there instead of the sky, just above the racer, so what
    -- arches over the ground (rib cages, rock arches) is passed and only what is at body height
    -- is hit (live 2026-11-20: the top-down survey read dinosaur bones as ground).
    local zt = tonumber(ztop) and tonumber(ztop) * 100 or 25000
    local Ray = require("raycast")
    x0, y0, x1, y1 = tonumber(x0) * 100, tonumber(y0) * 100, tonumber(x1) * 100, tonumber(y1) * 100
    step = (tonumber(step) or 3) * 100
    local ctx
    for _, a in ipairs(FindAllOf("TwinFootBP_C") or {}) do
        if Core.valid(a) and not tostring(a:GetFullName()):find("Default__", 1, true) then ctx = a end
    end
    if not ctx then
        local pc = FindFirstOf("PlayerController")
        ctx = Core.valid(pc) and Core.member(pc, "Pawn") or nil
    end
    if not Core.valid(ctx) then return out("no world context") end
    local vol = nil
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    local f = io.open(dir .. "\\dumps\\survey_" .. (name or "race") .. ".txt", "a")
    if not f then return out("cannot write") end
    local t0, n, hits, wet = os.clock(), 0, 0, 0
    local y = y0
    while y <= y1 do
        local x = x0
        while x <= x1 do
            n = n + 1
            local hit, dist, nz = Ray.probe_surface(ctx, x, y, zt, x, y, -5000, tonumber(ch) or 0)
            if hit then
                hits = hits + 1
                local gz = zt - (dist or 0)
                -- AVolume.EncompassesPoint aborts through pcall here (its float& out-param), so
                -- water is judged offline from the heights instead.
                local w = 0
                f:write(string.format("%.0f %.0f %.0f %.2f %d\n", x, y, gz, nz or -1, w))
            end
            x = x + step
        end
        y = y + step
    end
    f:close()
    out(string.format("survey: %d traces, %d hits, %d in water, %.2f s, volume %s", n, hits, wet,
        os.clock() - t0, vol and "found" or "missing"))
end

-- Race experiments with the walking robot: `robot x y z [yaw]` (metres) teleports it; `rtrack secs
-- [fwd] [jump]` logs its position every guide tick for `secs`, pushing the stick forward when fwd=1
-- and pressing A once at the start when jump=1 (to learn jump height and what a booster does).
local function race_robot()
    local Core = require("ui_core")
    for _, a in ipairs(FindAllOf("TwinFootBP_C") or {}) do
        if Core.valid(a) and not tostring(a:GetFullName()):find("Default__", 1, true) then return a end
    end
end

function C.pos()
    local Core = require("ui_core")
    local mm = Core.peek_all("AT_UIMiniMapRadar")[1]
    local pawn = Core.valid(mm) and Core.member(mm, "PlayerIns")
    if not Core.valid(pawn) then return out("no player pawn") end
    local l = pawn:K2_GetActorLocation()
    out(string.format("pawn %.1f %.1f %.1f", l.X / 100, l.Y / 100, l.Z / 100))
end

-- The race's water restart volume: transform and bounds, to solve its true box offline.
function C.vol()
    local Core = require("ui_core")
    for _, a in ipairs(FindAllOf("UnderWaterRestartVolume") or {}) do
        if Core.valid(a) and tostring(a:GetFullName()):find("TwinFoot", 1, true) then
            local l, r, s = a:K2_GetActorLocation(), a:K2_GetActorRotation(), a:GetActorScale3D()
            local o, e = {}, {}
            a:GetActorBounds(false, o, e)
            out(string.format("loc %.0f %.0f %.0f rot P%.2f Y%.2f R%.2f scale %.3f %.3f %.3f",
                l.X, l.Y, l.Z, r.Pitch, r.Yaw, r.Roll, s.X, s.Y, s.Z))
            out(string.format("bounds origin %.0f %.0f %.0f extent %.0f %.0f %.0f",
                o.X or 0, o.Y or 0, o.Z or 0, e.X or 0, e.Y or 0, e.Z or 0))
            return
        end
    end
    out("no race volume")
end

-- The volume's collision hull: BrushComponent.BrushBodySetup.AggGeom.ConvexElems[].VertexData
-- (local space, cm), written to dumps/race_volume.txt one "elem x y z" line per vertex.
function C.volgeom(cls, part)
    local Core = require("ui_core")
    cls, part = cls or "UnderWaterRestartVolume", part or "TwinFoot"
    local src = debug.getinfo(1, "S").source:sub(2)
    local f = io.open((src:match("^(.*)[/\\]") or ".") .. "\\dumps\\volumes_" .. cls .. ".txt", "w")
    local nvol = 0
    for _, vol in ipairs(FindAllOf(cls) or {}) do
        local full = ""
        pcall(function() full = vol:GetFullName() end)
        if Core.valid(vol) and full:find(part, 1, true) and not full:find("Default__", 1, true) then
            local l, r, sc = vol:K2_GetActorLocation(), vol:K2_GetActorRotation(), vol:GetActorScale3D()
            f:write(string.format("vol %s loc %.0f %.0f %.0f rot %.2f %.2f %.2f scale %.3f %.3f %.3f\n",
                full:match("([^%.]+)$") or full, l.X, l.Y, l.Z, r.Pitch, r.Yaw, r.Roll, sc.X, sc.Y, sc.Z))
            nvol = nvol + 1
            local bc = Core.member(vol, "BrushComponent")
            local bs = Core.valid(bc) and Core.member(bc, "BrushBodySetup")
            local agg = Core.valid(bs) and bs.AggGeom
            if agg and Core.valid_ref(agg) then
                local elems, n = Core.array_of(agg, "ConvexElems")
                for i = 1, n or 0 do
                    local e = elems[i]
                    if Core.valid_ref(e) then
                        local verts, vn = Core.array_of(e, "VertexData")
                        for k = 1, vn or 0 do
                            local v = verts[k]
                            if Core.valid_ref(v) then
                                f:write(string.format("v %d %.0f %.0f %.0f\n", i, v.X, v.Y, v.Z))
                            end
                        end
                    end
                end
            end
        end
    end
    f:close()
    out("volumes: " .. nvol)
end

-- The robot's movement limits: step height, walkable slope, jump speed, gravity, speeds, air control.
function C.rmove()
    local Core = require("ui_core")
    local rb = race_robot()
    if not rb then return out("no robot") end
    local cm = Core.member(rb, "CharacterMovement")
    if not Core.valid(cm) then return out("no movement component") end
    for _, k in ipairs({ "MaxStepHeight", "WalkableFloorAngle", "WalkableFloorZ", "JumpZVelocity",
        "GravityScale", "MaxWalkSpeed", "MaxAcceleration", "AirControl", "RotationRate" }) do
        local v
        pcall(function() v = cm[k] end)
        if type(v) == "userdata" or type(v) == "table" then
            pcall(function() v = string.format("P%.0f Y%.0f R%.0f", v.Pitch, v.Yaw, v.Roll) end)
        end
        out(k .. " = " .. tostring(v))
    end
    local cap = Core.member(rb, "CapsuleComponent")
    if Core.valid(cap) then
        pcall(function() out(string.format("capsule r=%.0f half=%.0f", cap:GetScaledCapsuleRadius(),
            cap:GetScaledCapsuleHalfHeight())) end)
    end
    pcall(function() out("JumpMaxHoldTime = " .. tostring(rb.JumpMaxHoldTime) .. " JumpMaxCount = " .. tostring(rb.JumpMaxCount)) end)
end

-- Walk the player towards (x, y) in metres for ms: the stick is camera-relative, so the direction
-- is turned into the camera's frame first (talk zones only open on a walk-in, not a teleport).
function C.walkto(x, y, ms)
    local Core = require("ui_core")
    local mm = Core.peek_all("AT_UIMiniMapRadar")[1]
    local pawn = Core.valid(mm) and Core.member(mm, "PlayerIns")
    local ctrl = Core.valid(pawn) and Core.member(pawn, "Controller")
    if not (Core.valid(pawn) and Core.valid(ctrl)) then return out("no pawn or controller") end
    local l = pawn:K2_GetActorLocation()
    local yaw = math.rad(ctrl:GetControlRotation().Yaw)
    local dx, dy = tonumber(x) * 100 - l.X, tonumber(y) * 100 - l.Y
    local d = math.sqrt(dx * dx + dy * dy)
    if d < 1 then return out("already there") end
    dx, dy = dx / d, dy / d
    local fwd = dx * math.cos(yaw) + dy * math.sin(yaw)
    local right = -dx * math.sin(yaw) + dy * math.cos(yaw)
    C.stick(string.format("%.2f", right), string.format("%.2f", fwd), ms or 500)
    out(string.format("walkto: %.1f m away, stick %.2f %.2f", d / 100, right, fwd))
end

-- The race's placed cameras (ViewActorRaceGame_N): location and camera yaw, to find the live one.
function C.views()
    local Core = require("ui_core")
    local n = 0
    for _, a in ipairs(FindAllOf("CameraComponent") or {}) do
        local full = ""
        pcall(function() full = a:GetFullName() end)
        if Core.valid(a) and full:find("ViewActorRaceGame", 1, true) then
            local l, r
            pcall(function() l = a:K2_GetComponentLocation() r = a:K2_GetComponentRotation() end)
            local active = "?"
            pcall(function() active = tostring(a:IsActive()) end)
            if l then
                n = n + 1
                out(string.format("%s at %.0f,%.0f,%.0f yaw %.1f pitch %.1f active %s",
                    full:match("(ViewActorRaceGame_%d+)") or "?", l.X / 100, l.Y / 100, l.Z / 100, r.Yaw, r.Pitch, active))
            end
        end
    end
    out(n .. " race cameras")
end

function C.robot(x, y, z, yaw)
    local rb = race_robot()
    if not rb then return out("no robot") end
    rb:K2_SetActorLocation({ X = tonumber(x) * 100, Y = tonumber(y) * 100, Z = tonumber(z) * 100 }, false, {}, true)
    if yaw then rb:K2_SetActorRotation({ Pitch = 0, Yaw = tonumber(yaw), Roll = 0 }, true) end
    local l, r = rb:K2_GetActorLocation(), rb:K2_GetActorRotation()
    out(string.format("robot -> %.1f %.1f %.1f yaw %.0f", l.X / 100, l.Y / 100, l.Z / 100, r.Yaw))
end

function C.rtrack(secs, sx, sy, jump)
    local Race, Input = require("race_guide"), require("input")
    local Core = require("ui_core")
    local until_t = os.clock() + (tonumber(secs) or 4)
    local t0 = os.clock()
    sx, sy = tonumber(sx) or 0, tonumber(sy) or 0
    if jump == "1" then C.press("A", 250) end
    Race._autodrive = function()
        if os.clock() > until_t then
            Race._autodrive = nil
            print("[KakarotDev] rtrack done\n")
            return
        end
        local rb = race_robot()
        if not rb then return end
        local l, r = rb:K2_GetActorLocation(), rb:K2_GetActorRotation()
        local cy = "?"
        pcall(function() cy = string.format("%.0f", Core.member(rb, "Controller"):GetControlRotation().Yaw) end)
        pcall(function()
            local pc = require("ui_directory").peek("hud", "PlayerOwner")
            cy = cy .. string.format(" cam %.0f", Core.member(pc, "PlayerCameraManager"):GetCameraRotation().Yaw)
        end)
        pcall(function()
            local gs = StaticFindObject("/Script/Engine.Default__GameplayStatics")
            local cm = gs:GetPlayerCameraManager(rb, 0)
            cy = cy .. string.format(" cam0 %.0f", cm:GetCameraRotation().Yaw)
        end)
        print(string.format("[KakarotDev] rtrack t=%.1f %.1f %.1f %.1f yaw %.0f view %s\n", os.clock() - t0,
            l.X / 100, l.Y / 100, l.Z / 100, r.Yaw, cy))
        if sx ~= 0 or sy ~= 0 then
            Input.vpad_plug(VPAD_PLUG_MS)
            Input.vpad_stick(sx, sy, 300)
        end
    end
    out("tracking")
end

-- Race autopilot for testing: on every race-guide tick, steer by the pan the guide is playing
-- (booster or checkpoint) and push forward. Mode "rt" holds RT (car), "stick" pushes the left
-- stick forward (walking robot), "both" does both. `autodrive off` stops it.
function C.autodrive(mode)
    local Race, Input = require("race_guide"), require("input")
    if mode == "off" then
        Race._autodrive = nil
        return out("autodrive off")
    end
    mode = mode or "both"
    local last_log = 0
    Race._cam, Race._stick_angle = nil, nil
    Race._autodrive = function(pan, fb)
        local a = Race._aim
        if a and a.px and os.clock() - last_log >= 2 then
            last_log = os.clock()
            print(string.format("[KakarotDev] autodrive racer %.0f,%.0f z=%.0f | ring %s%s | wp %s | pan %.2f fb %.2f\n",
                a.px / 100, a.py / 100, a.pz / 100, tostring(a.ring),
                a.bx and string.format(" at %.0f,%.0f z=%.0f (%.0f m)", a.bx / 100, a.by / 100, a.bz / 100,
                    math.sqrt((a.bx - a.px) ^ 2 + (a.by - a.py) ^ 2) / 100) or "",
                tostring(a.wp), pan, fb))
        end
        Input.vpad_plug(VPAD_PLUG_MS)
        if mode == "stick" then
            -- Walking robot: the stick is read against a follow camera nobody can read. The body
            -- turns to (camera + stick angle) within ~0.2 s, so the camera is estimated as
            -- body yaw minus the stick angle applied last, smoothed; then the stick is pushed at
            -- the aim's world bearing in that frame.
            local rb = race_robot()
            local ryaw
            pcall(function() ryaw = rb:K2_GetActorRotation().Yaw end)
            if a and a.stop then
                Input.vpad_stick(0, 0, 300)   -- "stop" at a launch point, then the jump
            elseif ryaw and a and a.dx then
                local function wrap(d) return (d + 540) % 360 - 180 end
                local est = ryaw - (Race._stick_angle or 0)
                -- In the air the body cannot turn to the stick, so the estimate would drift and
                -- the robot landed steering off the ridge (live 2026-11-20): hold it while airborne.
                local vz = 0
                pcall(function() vz = rb:GetVelocity().Z end)
                if not Race._cam or math.abs(tonumber(vz) or 0) < 200 then
                    Race._cam = Race._cam and Race._cam + 0.35 * wrap(est - Race._cam) or est
                end
                local bearing = math.deg(math.atan(a.dy, a.dx))
                local rel = math.rad(wrap(bearing - Race._cam))
                Race._stick_angle = math.deg(rel)
                Input.vpad_stick(math.sin(rel), math.cos(rel), 300)
            else
                Input.vpad_stick(pan, 1, 300)
            end
        else
            local steer = math.max(-1, math.min(1, pan * 1.6))
            if fb < 0 then steer = pan >= 0 and 1 or -1 end
            Input.vpad_stick(steer, (mode == "rt") and 0 or 1, 300)
        end
        if a and a.jump then
            print("[KakarotDev] autodrive jump\n")
            C.press("A", 250)
        end
        if mode ~= "stick" then Input.vpad_trigger(0, 1, 300) end
    end
    out("autodrive on (" .. mode .. ")")
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

-- World bounds of one actor (colliding components only), plus its collision component's class:
-- "how far does this trigger reach, and is its edge reachable?".
function C.bounds(cls, name_part)
    if not cls then return out("usage: bounds <Class> [name substring]") end
    local Core = require("ui_core")
    local target
    for _, o in ipairs(FindAllOf(cls) or {}) do
        if Core.valid(o) and (not name_part or tostring(o:GetFullName()):find(name_part, 1, true)) then
            target = o break
        end
    end
    if not Core.valid(target) then return out("no live " .. cls) end
    out("target: " .. tostring(target:GetFullName()))
    local origin, extent = {}, {}
    local ok, err = pcall(function() target:GetActorBounds(true, origin, extent) end)
    out(string.format("bounds ok=%s origin=%.0f,%.0f,%.0f extent=%.0f,%.0f,%.0f %s", tostring(ok),
        origin.X or 0, origin.Y or 0, origin.Z or 0, extent.X or 0, extent.Y or 0, extent.Z or 0,
        ok and "" or tostring(err)))
    local comp = Core.member(target, "CollisionComponent", true)
    if Core.valid(comp) then
        local cn = "?"
        pcall(function() cn = comp:GetClass():GetFName():ToString() end)
        out("collision component: " .. cn)
    end
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

-- Reverse lookup over the game's OWN item names: which drop slugs (item_drop_ids.lua) resolve to
-- a localized name containing <text>? Answers "what in the world gives this quest item" without
-- leaving the game. A bare number prints Item_<n>_Name instead. Spaces are allowed in <text>.
function C.items(...)
    local q = table.concat({ ... }, " ")
    if q == "" then return out("usage: items <name substring> | items <numeric id>") end
    local Nav = require("nav_tracker")
    if q:match("^%d+$") then
        return out(string.format("Item_%s_Name = %s", q, tostring(Nav._message_text("Item_" .. q .. "_Name"))))
    end
    local ok, ids = pcall(require, "item_drop_ids")
    if not ok or type(ids) ~= "table" then return out("item_drop_ids unavailable") end
    local want, by_id = q:lower(), {}
    for slug, num in pairs(ids) do
        local nm = Nav._message_text("Item_" .. num .. "_Name")
        if nm and nm:lower():find(want, 1, true) then
            by_id[num] = by_id[num] or { name = nm, slugs = {} }
            table.insert(by_id[num].slugs, slug)
        end
    end
    local n = 0
    for num, e in pairs(by_id) do
        table.sort(e.slugs)
        out(string.format("%d %s <- %s", num, e.name, table.concat(e.slugs, ", ")))
        n = n + 1
    end
    if n == 0 then out("no drop slug resolves to a name containing '" .. q .. "'") end
end

-- Live actors whose DROP SLUG contains <substr>: field pickups (ItemTableComponent) and wild
-- animals (AnimalMob_Pawn_C.FieldItemTable), with distance and hidden flag. Answers "is the quest
-- item anywhere in the loaded world, and is the radar's naming path able to see it?".
function C.drops(sub)
    if not sub then return out("usage: drops <slug substring>   e.g. drops Snail") end
    local Core = require("ui_core")
    local mm = Core.peek_all("AT_UIMiniMapRadar")[1]
    local pawn = Core.valid(mm) and Core.member(mm, "PlayerIns")
    local p = Core.valid(pawn) and pawn:K2_GetActorLocation() or nil
    local want, n = sub:lower(), 0
    for _, c in ipairs({
        { cls = "AccessPointItemBase", comp = "ItemTableComponent", strict = true },
        { cls = "PlacementObjectInfo", comp = "ItemTableComponent" },
        { cls = "AnimalMob_Pawn_C",    comp = "FieldItemTable",     strict = true },
    }) do
        for _, a in ipairs(FindAllOf(c.cls) or {}) do
            if Core.valid(a) then
                local slug
                pcall(function()
                    local comp = Core.member(a, c.comp, c.strict)
                    if not Core.valid(comp) then return end
                    local d = Core.member(comp, "FieldItemDropData", true)
                    if not Core.valid_ref(d) then return end
                    local s = {}
                    for _, f in ipairs({ "FixedId", "NormalId" }) do
                        local v = Core.name_str(d[f])
                        if v and v ~= "None" then s[#s + 1] = f .. "=" .. v end
                    end
                    slug = table.concat(s, " ")
                end)
                if slug and slug:lower():find(want, 1, true) then
                    local l = a:K2_GetActorLocation()
                    local dist = p and math.sqrt((l.X - p.X) ^ 2 + (l.Y - p.Y) ^ 2 + (l.Z - p.Z) ^ 2) / 100 or -1
                    local hid
                    pcall(function() hid = Core.member(a, "bHidden") end)
                    local cn = "?"
                    pcall(function() cn = a:GetClass():GetFName():ToString() end)
                    out(string.format("%-28s %s  d=%.0fm hidden=%s @ %.0f %.0f %.0f",
                        cn, slug, dist, tostring(hid), l.X, l.Y, l.Z))
                    n = n + 1
                    if n >= 60 then return out("(capped at 60)") end
                end
            end
        end
    end
    if n == 0 then out("no live actor carries a drop slug containing '" .. sub .. "'") end
end

-- Wild-animal SPAWN VOLUMES in the loaded world: the class each spawns, that class's DEFAULT drop
-- slug (read off the class default object's FieldItemTable), distance. Animals only exist while
-- the player is inside their volume, so the volume is the only thing a radar can point at before
-- that. `spawners [substr]` filters on class name or slug.
function C.spawners(sub)
    local Core = require("ui_core")
    local mm = Core.peek_all("AT_UIMiniMapRadar")[1]
    local pawn = Core.valid(mm) and Core.member(mm, "PlayerIns")
    local p = Core.valid(pawn) and pawn:K2_GetActorLocation() or nil
    local want = sub and sub:lower()
    local n = 0
    local function cdo_slug(cls)
        local slug
        pcall(function()
            local cdo = cls:GetCDO()
            if not Core.valid(cdo) then return end
            local comp = Core.member(cdo, "FieldItemTable", true)
            if not Core.valid(comp) then return end
            local d = Core.member(comp, "FieldItemDropData", true)
            if not Core.valid_ref(d) then return end
            local s = {}
            for _, f in ipairs({ "FixedId", "NormalId" }) do
                local v = Core.name_str(d[f])
                if v and v ~= "None" then s[#s + 1] = v end
            end
            slug = table.concat(s, " ")
        end)
        return slug or "-"
    end
    for _, vc in ipairs({ "AnimalSpawnerVolumeGround", "AnimalSpawnerVolumeSky" }) do
        for _, v in ipairs(FindAllOf(vc) or {}) do
            if Core.valid(v) then
                local arr, cnt = Core.array_of(v, "AnimalSpawnData", true)
                local l = v:K2_GetActorLocation()
                local dist = p and math.sqrt((l.X - p.X) ^ 2 + (l.Y - p.Y) ^ 2 + (l.Z - p.Z) ^ 2) / 100 or -1
                for i = 1, (cnt or 0) do
                    local cname, slug, max = "?", "-", "?"
                    pcall(function()
                        local e = arr[i]
                        local cls = e.ClassToSpawn
                        if Core.valid(cls) then
                            cname = cls:GetFName():ToString()
                            slug = cdo_slug(cls)
                        end
                        max = tostring(e.MaxAnimalNum)
                    end)
                    local line = string.format("%s[%d] %-24s max=%s slug=%s d=%.0fm @ %.0f %.0f %.0f",
                        vc:sub(20), i, cname, max, slug, dist, l.X, l.Y, l.Z)
                    if not want or line:lower():find(want, 1, true) then
                        out(line)
                        n = n + 1
                        if n >= 80 then return out("(capped at 80)") end
                    end
                end
            end
        end
    end
    if n == 0 then out("no animal spawn volume matches") end
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
