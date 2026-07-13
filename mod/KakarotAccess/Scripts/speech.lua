-- Speech sink: the single place that routes text to the screen reader.
--
-- Transport: an in-process Lua C module, prism_bridge.dll, which statically links
-- Lua 5.4.4 (UE4SS's exact version, so the lua_State ABI matches) and dynamically
-- loads prism.dll. We just require it and call its functions. The rest of the mod
-- only ever calls Speech.say(...). (Pattern validated by AccessForge/SparkingZeroAccess.)
--
-- prism_bridge.dll + prism.dll + tolk.dll must sit in this Scripts/ folder.

local Speech = {}

local prism = nil
local loaded = false

function Speech.init()
    local ok, mod = pcall(require, "prism_bridge")
    if ok and mod then
        prism = mod
        loaded = prism.is_ready and prism.is_ready() or false
        print("[KakarotAccess] prism_bridge loaded, screen reader: " .. tostring(prism.detect and prism.detect()) .. "\n")
    else
        print("[KakarotAccess] prism_bridge FAILED to load: " .. tostring(mod) .. "\n")
    end
    return loaded
end

function Speech.is_loaded()
    return loaded
end

-- Diagnostic: log EVERY utterance (with the interrupt flag) to dumps/dump_speech.txt,
-- to see what reaches PRISM and what may be stomping what. Costs one synchronous file
-- write per utterance on the game thread — keep OFF outside a diagnosis session.
local DEBUG_LOG = false
local function log_say(text, interrupt)
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_speech.txt", "a")
        if f then
            f:write(string.format("[%d|%.2f] %s%s\n", os.time(), os.clock(),
                interrupt and "!" or " ", text))
            f:close()
        end
    end)
end

-- Speak text. interrupt=true (default) cuts off current speech (use on context changes).
function Speech.say(text, interrupt)
    if not loaded or not text or text == "" then return end
    if DEBUG_LOG then log_say(tostring(text), interrupt ~= false) end
    prism.say(tostring(text), interrupt ~= false)
end

function Speech.stop()
    if loaded then prism.stop() end
end

function Speech.backend_name()
    return loaded and prism.detect() or "none"
end

function Speech.set_rate(v) if loaded then prism.set_rate(v) end end
function Speech.set_volume(v) if loaded then prism.set_volume(v) end end
function Speech.set_pitch(v) if loaded then prism.set_pitch(v) end end

return Speech
