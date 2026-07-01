-- Main title menu reader (Gametitle_C).
--
-- Design goals (deliberately the opposite of the SparkingZero approach, which
-- crashed / read slowly / slowed loads): NO global NotifyOnNewObject, NO global
-- SetText hooks, NO per-tick FindAllOf sweeps. We navigate directly by property
-- from a single cached widget and only announce on real changes (diff-gated).
--
-- Widget model (verified on DBZ Kakarot, UE 4.21):
--   Gametitle_C_0
--     .Gametitle_List00 .. .Gametitle_List05   (Gametitle_List_C, one per option)
--        .Txt_Select / .Txt_List               (Xcmn_MultiLineText_C)
--           .mainTxt.Text                        (CFUIMultiLineTextBox, FText label)
--   The SELECTED option is the one whose .Txt_Select is visible.

local Speech = require("speech")

local Menu = {}

local POLL_MS = 100          -- selection poll while the title menu is on screen
local REACQUIRE_EVERY = 10   -- when we have no widget, only search every Nth tick
local OPTION_COUNT = 6       -- Gametitle_List00..05

local enabled = false
local gametitle = nil        -- cached Gametitle_C widget
local tick = 0
local isOpen = false         -- are we currently in the title menu?
local lastIndex = -1         -- last announced selection index (diff-gate)

-- Safe helpers: every UObject deref is guarded, because calling a member on a
-- null UObject is an uncatchable C++ abort in UE4SS (it crashes the game).
local function valid(o) return o ~= nil and o:IsValid() == true end

local function option_selected(item)
    if not valid(item) then return false end
    local sel = item.Txt_Select
    if not valid(sel) then return false end
    local ok, vis = pcall(function() return sel:IsVisible() end)
    return ok and vis == true
end

local function option_label(item)
    if not valid(item) then return nil end
    -- Txt_List always holds the label (both nodes carry the same string).
    local t = item.Txt_List
    if not valid(t) then t = item.Txt_Select end
    if not valid(t) then return nil end
    local m = t.mainTxt
    if not valid(m) then return nil end
    local ok, s = pcall(function() return m.Text:ToString() end)
    if ok and s and s ~= "" then return s end
    return nil
end

-- Re-acquire the live Gametitle widget only when needed (rate-limited).
-- NOTE: FindFirstOf can return the archetype/CDO (never visible). Live runtime
-- instances live under the "/Engine/Transient" package; archetypes live under
-- "/Game/Art/UI". Matching the package (not a fixed "_0" suffix) survives the
-- widget being recreated as Gametitle_C_1, _2, ... when re-entering the menu.
local function acquire()
    gametitle = nil
    local all = FindAllOf("Gametitle_C") or {}
    for _, g in pairs(all) do
        if valid(g) and g:GetFullName():find("/Engine/Transient", 1, true) then
            gametitle = g
            return
        end
    end
end

-- One poll step, always on the game thread.
local function step()
    if not valid(gametitle) then
        gametitle = nil
        if tick % REACQUIRE_EVERY == 0 then acquire() end
        if not gametitle then
            if isOpen then isOpen = false; lastIndex = -1 end
            return
        end
    end

    -- Is the title menu actually on screen right now?
    local okv, vis = pcall(function() return gametitle:IsVisible() end)
    if not (okv and vis) then
        if isOpen then isOpen = false; lastIndex = -1 end
        return
    end

    -- Find the selected option by direct property navigation.
    local idx, label = -1, nil
    for i = 0, OPTION_COUNT - 1 do
        local item = gametitle["Gametitle_List0" .. i]
        if option_selected(item) then
            idx = i
            label = option_label(item)
            break
        end
    end

    if not isOpen then
        -- Entering the title menu: announce the focused option (context change).
        isOpen = true
        lastIndex = idx
        if label then Speech.say(label, true) end
    elseif idx ~= -1 and idx ~= lastIndex then
        -- Selection moved: read only the new option.
        lastIndex = idx
        if label then Speech.say(label, true) end
    end
end

-- Diagnostic: read the current selection once and SPEAK what we find (or why we
-- found nothing). Lets us debug by ear, no console/file needed.
function Menu.read_current()
    ExecuteInGameThread(function()
        acquire()
        if not valid(gametitle) then Speech.say("no live gametitle found", true) return end
        local okv, vis = pcall(function() return gametitle:IsVisible() end)
        if not okv then Speech.say("visible check failed", true) return end
        if not vis then Speech.say("gametitle not visible", true) return end
        for i = 0, OPTION_COUNT - 1 do
            local item = gametitle["Gametitle_List0" .. i]
            if not valid(item) then
                Speech.say("option " .. i .. " missing", true)
                return
            end
            if option_selected(item) then
                local l = option_label(item)
                Speech.say("selected " .. i .. ": " .. (l or "no label"), true)
                return
            end
        end
        Speech.say("no option selected", true)
    end)
end

function Menu.start()
    if enabled then return end
    enabled = true
    -- Generation guard: any previously running loop (e.g. left over from a
    -- reload) sees a newer generation and stops itself, so we never end up with
    -- two loops racing on a stale Lua state (a common hot-reload crash cause).
    _G.__KakarotMenuGen = (_G.__KakarotMenuGen or 0) + 1
    local myGen = _G.__KakarotMenuGen
    LoopAsync(POLL_MS, function()
        if not enabled or _G.__KakarotMenuGen ~= myGen then return true end -- stop
        tick = tick + 1
        ExecuteInGameThread(step)
        return false
    end)
    print("[KakarotAccess] Title menu reader started\n")
end

function Menu.stop()
    enabled = false
end

function Menu.is_enabled() return enabled end

return Menu
