--[[
    Screen Reader Service
    =====================
    Handles Tolk initialization and TTS output.

    SETUP:
    1. Compile tolk_lua.dll (C wrapper for Tolk) and place in scripts/ folder
    2. Place Tolk.dll and screen reader client DLLs in the game's main folder

    If tolk_lua.dll is not available, falls back to print-only mode.
]]

local ScreenReader = {}

local tolk = nil
local isLoaded = false
local fallbackMode = false

function ScreenReader.Init()
    local success, result = pcall(function()
        return require("tolk_lua")
    end)

    if success and result then
        tolk = result
        tolk.load()
        isLoaded = true
        print("[ScreenReader] Tolk loaded successfully\n")
        return true
    else
        fallbackMode = true
        print("[ScreenReader] Tolk not available, using fallback (print only)\n")
        print("[ScreenReader] Error: " .. tostring(result) .. "\n")
        return false
    end
end

function ScreenReader.IsLoaded()
    return isLoaded
end

function ScreenReader.Output(text, interrupt)
    if not text or text == "" then return end

    if isLoaded and tolk then
        tolk.speak(text, interrupt or false)
    end

    -- Always log to console for debugging
    if fallbackMode then
        print("[TTS] " .. text .. "\n")
    end
end

function ScreenReader.Silence()
    if isLoaded and tolk then
        tolk.silence()
    end
end

function ScreenReader.Shutdown()
    if isLoaded and tolk then
        tolk.unload()
        isLoaded = false
    end
end

return ScreenReader
