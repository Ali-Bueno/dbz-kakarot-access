# Accessibility Patterns for UE4SS Mods

Patterns and strategies for making Unreal Engine games accessible to blind players using UE4SS Lua mods.

---

## Architecture Overview

Each accessibility mod should follow this modular structure:

```
Mods/
  GameAccessibility/
    scripts/
      main.lua                -- Entry point, initializes everything
      screen_reader.lua       -- Tolk/TTS integration
      game_state_tracker.lua  -- Monitors game state changes
      ui_reader.lua           -- Reads UI widget text
      audio_cue_service.lua   -- Spatial audio / contextual sounds
      keybind_manager.lua     -- Accessibility hotkeys
      utils.lua               -- Shared utilities
```

---

## 1. Screen Reader Integration (Tolk)

### Challenge
Tolk is a C/C++ library. Lua mods can load C libraries (.dll) from the `scripts/` folder.

### Approach: C Wrapper DLL
Create a small C DLL that wraps Tolk and exposes Lua-callable functions:

```c
// tolk_lua.c - Compiled as tolk_lua.dll, placed in scripts/
#include <lua.h>
#include <Tolk.h>

static int l_tolk_load(lua_State *L) { Tolk_Load(); return 0; }
static int l_tolk_unload(lua_State *L) { Tolk_Unload(); return 0; }
static int l_tolk_output(lua_State *L) {
    const char *text = luaL_checkstring(L, 1);
    // Convert to wide string and call Tolk_Output
    Tolk_Output(wtext, FALSE);
    return 0;
}
static int l_tolk_speak(lua_State *L) {
    const char *text = luaL_checkstring(L, 1);
    int interrupt = lua_toboolean(L, 2);
    Tolk_Output(wtext, interrupt);
    return 0;
}
static int l_tolk_silence(lua_State *L) { Tolk_Silence(); return 0; }
static int l_tolk_is_loaded(lua_State *L) {
    lua_pushboolean(L, Tolk_IsLoaded());
    return 1;
}

static const luaL_Reg tolklib[] = {
    {"load", l_tolk_load},
    {"unload", l_tolk_unload},
    {"output", l_tolk_output},
    {"speak", l_tolk_speak},
    {"silence", l_tolk_silence},
    {"is_loaded", l_tolk_is_loaded},
    {NULL, NULL}
};

int luaopen_tolk_lua(lua_State *L) {
    luaL_newlib(L, tolklib);
    return 1;
}
```

### Usage in Lua
```lua
local tolk = require("tolk_lua")
tolk.load()
tolk.speak("Game loaded", true)  -- true = interrupt previous speech
```

### Alternative: SAPI via COM
If Tolk wrapper is too complex, use Windows SAPI directly via a similar C wrapper.

---

## 2. UI Text Reading

### Discovering Widget Classes
First, dump all objects (Ctrl+J) and search for UI-related classes:
```lua
-- Enumerate all UserWidget subclasses
local Widgets = FindAllOf("UserWidget")
if Widgets then
    for _, w in pairs(Widgets) do
        print(w:GetFullName() .. "\n")
    end
end

-- Look for TextBlock widgets specifically
local TextBlocks = FindAllOf("TextBlock")
if TextBlocks then
    for _, tb in pairs(TextBlocks) do
        local text = tb.Text  -- FText property
        if text then
            print("Text: " .. text:ToString() .. "\n")
        end
    end
end
```

### Watching for UI Changes
```lua
-- Detect when new widgets appear
NotifyOnNewObject("/Script/UMG.UserWidget", function(Widget)
    ExecuteInGameThread(function()
        -- Inspect the widget, read its text content
        local className = Widget:GetClass():GetFName():ToString()
        print("New widget: " .. className .. "\n")
    end)
end)
```

### Hooking Widget Update Functions
```lua
-- Hook specific widget functions to catch text changes
RegisterHook("/Script/UMG.TextBlock:SetText", function(self, Text)
    local newText = Text:Get():ToString()
    tolk.speak(newText, false)
end)
```

---

## 3. Game State Tracking

### State Machine Pattern
```lua
local GameState = {
    current = "unknown",
    previous = "unknown",

    STATES = {
        MAIN_MENU = "main_menu",
        GAMEPLAY = "gameplay",
        PAUSE_MENU = "pause_menu",
        INVENTORY = "inventory",
        DIALOGUE = "dialogue",
        COMBAT = "combat",
        LOADING = "loading",
    }
}

function GameState:Update(newState)
    if newState ~= self.current then
        self.previous = self.current
        self.current = newState
        self:OnStateChanged()
    end
end

function GameState:OnStateChanged()
    tolk.speak("Now in: " .. self.current, true)
end
```

### Detecting State from UE Objects
```lua
-- Hook map loading for level changes
RegisterLoadMapPostHook(function()
    GameState:Update(GameState.STATES.LOADING)
end)

-- Hook BeginPlay for when actors load
RegisterBeginPlayPostHook(function(Actor)
    local name = Actor:GetClass():GetFName():ToString()
    -- Check for menu/HUD actors to determine state
end)
```

---

## 4. Keybind System for Accessibility

```lua
-- Read current info on demand
RegisterKeyBind(Key.F1, function()
    tolk.speak("Current state: " .. GameState.current, true)
end)

RegisterKeyBind(Key.F2, function()
    -- Read player health/status
    local PC = FindFirstOf("PlayerController")
    if PC:IsValid() then
        local Pawn = PC.Pawn
        -- Read health, position, etc.
    end
end)

RegisterKeyBind(Key.F3, function()
    -- Read current menu item / focused widget
end)

-- Toggle verbosity
local verbose = false
RegisterKeyBind(Key.F4, function()
    verbose = not verbose
    tolk.speak("Verbose mode: " .. (verbose and "on" or "off"), true)
end)

-- Silence screen reader
RegisterKeyBind(Key.ESCAPE, {ModifierKey.CONTROL}, function()
    tolk.silence()
end)
```

---

## 5. Anti-Spam / Announcement Queue

```lua
local Announcer = {
    lastMessage = "",
    lastTime = 0,
    minInterval = 200,  -- ms between same messages
}

function Announcer:Speak(message, interrupt)
    local now = os.clock() * 1000
    if message == self.lastMessage and (now - self.lastTime) < self.minInterval then
        return  -- skip duplicate
    end
    self.lastMessage = message
    self.lastTime = now
    tolk.speak(message, interrupt or false)
end

function Announcer:SpeakUnique(message)
    if message ~= self.lastMessage then
        self:Speak(message, false)
    end
end
```

---

## 6. Periodic Polling (Careful Use)

```lua
local previousMenuText = ""

LoopAsync(500, function()
    ExecuteInGameThread(function()
        -- Check focused widget or menu state
        local currentText = GetCurrentFocusedText()  -- custom function
        if currentText and currentText ~= previousMenuText then
            previousMenuText = currentText
            Announcer:Speak(currentText, true)
        end
    end)
    return false  -- keep running
end)
```

**Warning:** Avoid polling every frame. 250-500ms is usually sufficient for UI changes.

---

## 7. Per-Game Discovery Workflow

1. **Install UE4SS (zDEV)** in the game
2. **Dump C++ headers** (Ctrl+H) — learn class names, properties, functions
3. **Dump all objects** (Ctrl+J) — see what's loaded at runtime
4. **Use Live Viewer** — search for UI widgets, player controllers, game state objects
5. **Identify key UFunctions** — menu navigation, text display, state changes
6. **Write hooks** — intercept relevant functions
7. **Build accessibility layer** — screen reader + audio cues
8. **Test with hot-reload** (Ctrl+R) — iterate fast

---

## Differences from BepInEx/Unity Approach

| Aspect | BepInEx (Unity) | UE4SS (Unreal) |
|--------|-----------------|----------------|
| Language | C# | Lua (or C++) |
| Patching | Harmony Prefix/Postfix | RegisterHook on UFunctions |
| Object finding | FindObjectOfType<T> | FindFirstOf("ClassName") |
| UI system | Unity UI / TextMeshPro | UMG (UserWidget, TextBlock) |
| Hot reload | Recompile DLL + restart | Ctrl+R (instant) |
| TTS | Tolk via NuGet/DLL ref | Tolk via C wrapper DLL |
| State detection | MonoBehaviour lifecycle | BeginPlay/EndPlay hooks |
| Performance | IL2CPP reflection cost | Direct property access |
