-- Pure playback-clock scheduler for cinematic audio descriptions.
--
-- The UE4SS adapter supplies a normalized movie/sequence source and its exact
-- playback time. This module deliberately knows nothing about UObjects so the
-- seek, rewind, dialogue-priority and one-shot rules can be regression tested
-- without launching the game.

local Engine = {}
local Player = {}
Player.__index = Player

-- A poll can land just after a cue. Keep the grace window smaller than a normal
-- spoken phrase: it absorbs frame/tick jitter but never resurrects old action.
local LATE_GRACE_S = 0.35
-- A clock advance this large is a seek/skip, not a late tick. Old cues are
-- discarded so skipping a cinematic can never produce a burst of narration.
local SEEK_STEP_S = 1.00
local REWIND_S = 0.50

function Engine.normalize_source(source)
    if source == nil then return nil end
    local s = tostring(source):upper()
    local stem = s:match("(C%d%d_%d%d%d_S%d%d%d)_MOV")
    if stem then return stem .. "_MOV" end
    stem = s:match("(C%d%d_%d%d%d_S%d%d%d)_GDM")
    if stem then return stem .. "_GDM" end
    return nil
end

local function first_timely(cues, now)
    local i = 1
    while cues[i] and cues[i].at < now - LATE_GRACE_S do i = i + 1 end
    return i
end

function Engine.new(catalog, say_fn, dialogue_active_fn)
    assert(type(catalog) == "table", "audio-description catalog must be a table")
    assert(type(say_fn) == "function", "audio-description speech callback is required")
    assert(type(dialogue_active_fn) == "function", "audio-description dialogue callback is required")
    return setmetatable({
        catalog = catalog,
        say = say_fn,
        dialogue_active = dialogue_active_fn,
        source = nil,
        cues = nil,
        next_cue = 1,
        last_time = nil,
    }, Player)
end

function Player:reset()
    self.source = nil
    self.cues = nil
    self.next_cue = 1
    self.last_time = nil
end

function Player:update(raw_source, seconds, playing, enabled)
    local source = Engine.normalize_source(raw_source)
    local now = tonumber(seconds)
    local cues = source and self.catalog[source] or nil
    if not source or not cues or not now or now < 0 then
        self:reset()
        return false
    end

    if source ~= self.source then
        self.source = source
        self.cues = cues
        self.next_cue = first_timely(cues, now)
        self.last_time = now
    elseif self.last_time then
        local step = now - self.last_time
        if step < -REWIND_S then
            -- A restart/rewind is a new performance of this scene.
            self.next_cue = first_timely(cues, now)
        elseif step > SEEK_STEP_S then
            -- A skip may cross many cues; retain only one that is timely now.
            self.next_cue = first_timely(cues, now)
        end
        self.last_time = now
    else
        self.last_time = now
    end

    -- While stopped/paused, remember the clock but do not consume future cues.
    if playing ~= true then return false end

    -- Turning the feature off consumes anything that elapsed while it was off.
    -- Enabling it mid-scene therefore starts with future action, never a backlog.
    if enabled == false then
        while cues[self.next_cue] and cues[self.next_cue].at <= now do
            self.next_cue = self.next_cue + 1
        end
        return false
    end

    -- Drop stale cues even on an ordinary tick. This also bounds recovery after
    -- a delayed game-thread dispatch that fell just short of SEEK_STEP_S.
    while cues[self.next_cue] and cues[self.next_cue].at < now - LATE_GRACE_S do
        self.next_cue = self.next_cue + 1
    end

    local cue = cues[self.next_cue]
    if not cue or cue.at > now then return false end

    -- Consume before asking about dialogue. If dialogue wins, the visual moment
    -- is gone; replaying the description later would be misleading.
    self.next_cue = self.next_cue + 1
    if self.dialogue_active() then return false end

    -- Queue behind any speech already reaching the reader. no_requeue=true makes
    -- an immediately following subtitle discard the description instead of
    -- reviving it after the scene has moved on.
    self.say(cue.text, false, true, cue.key)
    return true
end

return Engine
