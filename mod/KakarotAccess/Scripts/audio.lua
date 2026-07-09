-- Audio cue sink: the single place that routes navigation cues to the speakers.
--
-- Transport: an in-process Lua C module, audio_bridge.dll (XAudio2), which plays
-- the two radar sounds shipped in Scripts/sounds/ (beacon.wav / arrived.wav —
-- reused from the validated XV2 accessibility mod). The rest of the mod only
-- ever calls Audio.ping/arrival/stop; all the distance->cadence and behind->pitch
-- logic lives in nav_tracker.lua.
--
-- Loaded once from main.lua (before the protected snapshot) so, like the speech
-- and memory bridges, it survives a Ctrl+Shift+R hot reload.

local Audio = {}

local bridge = nil
local loaded = false

function Audio.init()
    local ok, mod = pcall(require, "audio_bridge")
    if ok and mod then
        bridge = mod
        local inited, err = bridge.init()
        loaded = inited == true
        if loaded then
            print("[KakarotAccess] audio_bridge loaded (XAudio2 cues ready)\n")
        else
            print("[KakarotAccess] audio_bridge init FAILED: " .. tostring(err) .. "\n")
        end
    else
        print("[KakarotAccess] audio_bridge FAILED to load: " .. tostring(mod) .. "\n")
    end
    return loaded
end

function Audio.is_loaded()
    return loaded
end

-- One radar ping. pan -1..1 (left..right), volume 0..1, pitch 0.5..2 (1 = normal).
-- The CALLER controls the cadence (how often pings fire); each call retriggers.
function Audio.ping(pan, volume, pitch)
    if loaded then bridge.ping(pan, volume, pitch) end
end

-- Objective reached: cuts the beacon and plays the arrival cue centered.
function Audio.arrival()
    if loaded then bridge.arrival() end
end

-- Play a named category cue (explore radar): "item", "enemy", ... pan -1..1,
-- volume 0..1, pitch 0.5..2. Unknown/unloaded names are a silent no-op.
function Audio.cue(name, pan, volume, pitch)
    if loaded and bridge.cue then bridge.cue(name, pan, volume, pitch) end
end

-- Continuous soft sine tone (seamless loop): call repeatedly to reshape volume 0..1
-- and pitch 0.5..2 (110..440 Hz) — no retrigger, gentle on the ears. Stop when done.
function Audio.tone(volume, pitch)
    if loaded and bridge.tone then bridge.tone(volume, pitch) end
end

function Audio.tone_stop()
    if loaded and bridge.tone_stop then bridge.tone_stop() end
end

-- Silence all cues immediately.
function Audio.stop()
    if loaded then bridge.stop() end
end

return Audio
