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
local deps = {}

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
        "memdiff               F4 runtime memory diff -> Scripts/dev_probe.txt",
    }) do out("  " .. l) end
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
        out("last spoken (newest first):")
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
    local ok, err = pcall(fn, table.unpack(args))
    if not ok then out("error: " .. tostring(err)) end
end

--------------------------------------------------------------------------------
-- install
--------------------------------------------------------------------------------

function Channel.install(d)
    -- ONE loop, ever. main.lua is never re-required (Ctrl+Shift+R only drops package.loaded), so this
    -- normally installs once — but the reload DOES nil package.loaded.dev_channel, so any later
    -- require would build a second module table and, without this, a second 500 ms loop polling the
    -- same file. A global flag is the only state that survives both.
    deps = d or {}
    if _G.__KakarotDevChannel then
        out("dev channel already installed")
        return false
    end
    _G.__KakarotDevChannel = true

    local last_cmd, busy = nil, false
    LoopAsync(POLL_MS, function()
        -- Backlog guard (ue4ss-api-reference.md:144-149): never queue the next step until the
        -- previous one finished. `census` and `reload` are tens of milliseconds of game-thread work;
        -- without this, a caller that writes a second command while the first is still running gets
        -- them interleaved and the sequence markers stop bracketing anything.
        if busy then return false end
        local f = io.open(CMD_PATH, "r")
        if not f then return false end
        local line = f:read("*l")
        f:close()
        if not line or line == "" or line == last_cmd then return false end
        last_cmd = line
        busy = true
        ExecuteInGameThread(function()
            -- pcall around the WHOLE body, including the markers: if anything in here raises, the
            -- END marker must still be written or the caller waits out its full timeout for an
            -- answer that is never coming, and `busy` would latch the channel dead forever.
            local seq, rest = line:match("^#(%S+)%s+(.*)$")
            pcall(function()
                if seq then out("<<<BEGIN " .. seq .. ">>>") end
                dispatch(rest or line)
            end)
            if seq then pcall(out, "<<<END " .. seq .. ">>>") end
            busy = false
        end)
        return false
    end)

    out("dev channel ready - write a command into " .. CMD_PATH .. ", read " .. OUT_PATH)
    return true
end

return Channel
