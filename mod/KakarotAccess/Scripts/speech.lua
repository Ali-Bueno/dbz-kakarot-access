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
local DEBUG_LOG = false  -- utterance stream to dumps/dump_speech.txt (diagnosis only)
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

-- Spanish TTS at a normal rate is ~14 chars/s; add a 0.5 s tail so the reader doesn't
-- clip the last word. Capped so a very long notice can't lock out the board forever.
local function speak_seconds(text)
    return math.min(6.0, 0.5 + #tostring(text) / 14)
end

-- Cost telemetry (2026-07-16 night: mods.txt A/B proved the residual cinematic stutter
-- IS this mod, while the registry step measures only ~5% — something unmeasured). Every
-- prism_bridge call runs ON THE GAME THREAD, and cinematics are the densest speech
-- state: if the backend (NVDA IPC / SAPI) blocks for tens of ms per call, every
-- subtitle line is a felt hitch. This times each call into _G.__KakarotSpeechStats
-- (printed by the Ctrl+F5 dump) so the next dump confirms or clears it with data.
local function timed_say(text, interrupt)
    local t0 = os.clock()
    prism.say(text, interrupt)
    local dt = (os.clock() - t0) * 1000
    local s = _G.__KakarotSpeechStats
    if not s then s = { n = 0, ms = 0, max = 0 } _G.__KakarotSpeechStats = s end
    s.n = s.n + 1
    s.ms = s.ms + dt
    if dt > s.max then s.max = dt end
end

-- Queued (interrupt=false) lines the backend may not have FINISHED speaking yet. An
-- interrupt=true say clears the reader's whole queue — which silently killed pickup
-- toasts ("Hierba x 1", orbs) whenever a subtitle line landed at the same time (user
-- 2026-07-16: subtitle + item pickup → one of the two lost). On every interrupt the
-- unfinished queued lines are RE-APPENDED right after the interrupting line, so a
-- toast survives the cut and speaks in the next gap. Finish times are estimated with
-- the same chars/s model as the protection window; each line is re-queued at most
-- REQUEUE_MAX times so an unlucky estimate (or a busy menu) can't make it immortal.
local REQUEUE_MAX = 2
local pending = {}   -- { text, until_, requeues }

local function prune_pending()
    local now, keep = os.clock(), {}
    for _, p in ipairs(pending) do
        if p.until_ > now then keep[#keep + 1] = p end
    end
    pending = keep
end

-- Speak text. interrupt=true (default) cuts off current speech (use on context changes).
-- no_requeue=true (queued lines only): if an interrupt cuts this line, it is NOT
-- re-appended. For ephemeral overlays (the cinematic intro cards) a repeat is worse
-- than a lost tail — the requeue feature exists for pickup toasts (2026-07-17).
function Speech.say(text, interrupt, no_requeue)
    if not loaded or not text or text == "" then return end
    text = tostring(text)
    interrupt = interrupt ~= false
    if DEBUG_LOG then log_say(text, interrupt) end
    if not interrupt then
        timed_say(text, false)
        if not no_requeue then
            pending[#pending + 1] = { text = text, until_ = os.clock() + speak_seconds(text),
                                      requeues = 0 }
        end
        return
    end
    prune_pending()
    local cut = pending
    pending = {}
    timed_say(text, true)
    local base = os.clock() + speak_seconds(text)
    for _, p in ipairs(cut) do
        if p.requeues < REQUEUE_MAX and p.text ~= text then
            if DEBUG_LOG then log_say("(requeue) " .. p.text, false) end
            timed_say(p.text, false)
            p.requeues = p.requeues + 1
            p.until_ = base + speak_seconds(p.text)
            pending[#pending + 1] = p
        end
    end
end

function Speech.stop()
    if loaded then prism.stop() end
end

-- ---- speech PRIORITY / protection ------------------------------------------------
-- There is NO Lua-side queue: interrupt=true clears the backend and speaks NOW,
-- interrupt=false appends to the screen reader's own queue. So an actionable
-- INSTRUCTION ("select an empty panel, press A") or a reward NOTICE gets shredded the
-- instant a lower-priority reader (the board/grid panel readout) fires its own
-- interrupt=true (user 2026-07-16: the board "eats" the instruction). Fix: when an
-- important line is spoken, PROTECT it for roughly its spoken duration; lower-priority
-- readers check Speech.protected() and DEFER their readout until it clears, so the
-- instruction is heard in full, then the panel reads.
local protect_until = 0

-- Speak an important line (interrupt=true — win immediately) and protect it.
function Speech.say_protected(text)
    if not loaded or not text or text == "" then return end
    Speech.say(text, true)
    protect_until = os.clock() + speak_seconds(text)
end

-- Is an important line still playing (protected window not yet elapsed)?
function Speech.protected()
    return os.clock() < protect_until
end

function Speech.backend_name()
    return loaded and prism.detect() or "none"
end

function Speech.set_rate(v) if loaded then prism.set_rate(v) end end
function Speech.set_volume(v) if loaded then prism.set_volume(v) end end
function Speech.set_pitch(v) if loaded then prism.set_pitch(v) end end

return Speech
