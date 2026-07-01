-- UI core: the reusable substrate for the hybrid menu architecture
-- (see reference/ui-accessibility/generic-strategy.md).
--
-- Provides, with no per-screen knowledge:
--   * widget helpers (valid / is_visible / text_of / first_live / phrase)
--   * an Announcer (the diff-gated speaking rules from menus.md), shared by every
--     screen adapter so the "what to say when name/value/tab/tooltip changes"
--     logic lives in ONE place
--   * the single poll engine (LoopAsync + generation guard) the registry drives
--
-- This game exposes no reliable focus events (RegisterHook is fragile, menus have
-- no selection index), so "subscription" is a shared diff-gated poll — the
-- polling-fallback path the strategy doc allows.

local Speech = require("speech")

local Core = {}

Core.POLL_MS = 100
local TOOLTIP_WINDOW = 6   -- ticks to keep polling for a late-arriving tooltip

-- ---- widget helpers --------------------------------------------------------

function Core.valid(o) return o ~= nil and o:IsValid() == true end

function Core.is_visible(o)
    if not Core.valid(o) then return false end
    local ok, v = pcall(function() return o:IsVisible() end)
    return ok and v == true
end

-- Text out of an Xcmn_MultiLineText_C node (its inner CFUIMultiLineTextBox is
-- `mainTxt`). Empty string counts as nil (recycled rows keep stale hidden text).
function Core.text_of(node)
    if not Core.valid(node) then return nil end
    local m = node.mainTxt
    if not Core.valid(m) then return nil end
    local ok, s = pcall(function() return m.Text:ToString() end)
    if ok and s and s ~= "" then return s end
    return nil
end

-- First live (runtime, not archetype/CDO) instance of a class.
function Core.first_live(cls_name)
    local all = FindAllOf(cls_name) or {}
    for _, o in pairs(all) do
        if Core.valid(o) and o:GetFullName():find("/Engine/Transient", 1, true) then
            return o
        end
    end
    return nil
end

-- Join non-empty parts with commas, skipping nils in ANY position.
function Core.phrase(...)
    local n = select("#", ...)
    local parts = {}
    for i = 1, n do
        local p = select(i, ...)
        if p and p ~= "" then parts[#parts + 1] = p end
    end
    return table.concat(parts, ", ")
end

-- ---- Announcer: the shared diff-gated speaking rules ------------------------
-- An adapter calls announcer:focus(tab, name, value, tooltip_fn) every tick with
-- the currently focused control. tooltip_fn is a lazy getter (called only when a
-- tooltip is actually needed, so screens without one pass nil and screens with a
-- costly lookup don't pay it every tick). Rules (menus.md):
--   enter / name change -> name (+ value) (+ tooltip), interrupt
--   tab change          -> tab name only
--   value change        -> only the new value
--   tooltip arriving late-> spoken on its own, queued

local Announcer = {}
Announcer.__index = Announcer

function Core.make_announcer()
    return setmetatable({ t = 0, open = false }, Announcer)
end

function Announcer:reset()
    self.open = false
    self.tab, self.name, self.value, self.tip = nil, nil, nil, nil
    self.pending = false
end

-- screen = the menu's own name, spoken ONLY when entering the screen (context
-- change), before the focused item. tab = the current tab/section, spoken on tab
-- change. Either may be nil.
function Announcer:focus(screen, tab, name, value, tooltip_fn)
    self.t = self.t + 1
    local function tip() return tooltip_fn and tooltip_fn() or nil end

    if not self.open then
        self.open = true
        self.tab, self.name, self.value = tab, name, value
        local tt = tip()
        Speech.say(Core.phrase(screen, name, value, tt), true)
        self.tip = tt
        self.pending = (tooltip_fn ~= nil and tt == nil)
        self.deadline = self.t + TOOLTIP_WINDOW
        return
    end

    if tab ~= self.tab then
        self.tab, self.name, self.value = tab, name, value
        if tab then Speech.say(tab, true) end
        self.pending = false
    elseif name ~= self.name then
        self.name, self.value = name, value
        local tt = tip()
        Speech.say(Core.phrase(name, value, tt), true)
        self.tip = tt
        self.pending = (tooltip_fn ~= nil and tt == nil)
        self.deadline = self.t + TOOLTIP_WINDOW
    elseif value ~= self.value then
        self.value = value
        if value then Speech.say(value, true) end
    elseif self.pending then
        if self.t > self.deadline then
            self.pending = false
        else
            local tt = tip()
            if tt then Speech.say(tt, false); self.pending = false end
        end
    end
end

-- ---- poll engine -----------------------------------------------------------
-- One loop for the whole UI. A generation guard stops any loop left over from a
-- prior load (so a reload never leaves two loops racing on a stale Lua state).
--
-- BUSY GUARD: LoopAsync fires every POLL_MS on a worker thread and queues `step` onto
-- the game thread WITHOUT waiting for it. When the game thread is saturated (loading,
-- a fullscreen movie, etc.), those queued steps pile up into a backlog that then runs
-- late and in bursts — felt as long delays when navigating a menu. So we only queue a
-- new step once the previous one has finished, keeping the reader on the CURRENT state.
function Core.loop(step)
    _G.__KakarotUiGen = (_G.__KakarotUiGen or 0) + 1
    local myGen = _G.__KakarotUiGen
    local busy = false
    LoopAsync(Core.POLL_MS, function()
        if _G.__KakarotUiGen ~= myGen then return true end
        if not busy then
            busy = true
            ExecuteInGameThread(function()
                local ok, err = pcall(step)
                busy = false
                if not ok then print("[KakarotAccess] UI step error: " .. tostring(err) .. "\n") end
            end)
        end
        return false
    end)
end

return Core
