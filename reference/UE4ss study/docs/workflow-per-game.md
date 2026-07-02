# Workflow: Making a New Game Accessible with UE4SS

Step-by-step guide for each new Unreal Engine game.

---

## Phase 1: Setup & Discovery

### 1.1 Install UE4SS
1. Download the **zDEV** version (developer install)
2. Find the game's main `.exe` in `GameName/Binaries/Win64/`
3. Extract UE4SS there
4. Edit `UE4SS-settings.ini`:
   ```ini
   [Debug]
   ConsoleEnabled = 1
   GuiConsoleEnabled = 1
   GuiConsoleVisible = 1

   [General]
   EnableHotReloadSystem = 1
   ```
5. Launch game, verify UE4SS loads (check `UE4SS.log`)

### 1.2 Dump Game Data
1. Press **Ctrl+H** → C++ headers (class structures, properties, functions)
2. Press **Ctrl+J** → Object dump (everything loaded in memory)
3. Open headers in an editor, search for:
   - Widget/UI classes
   - PlayerController subclasses
   - GameState/GameMode classes
   - HUD classes
   - Menu/inventory classes

### 1.3 Explore with Live Viewer  ← do this before writing any reflection code
Full details in [ue4ss-discovery-tools](ue4ss-discovery-tools.md).
1. Open the GUI console (`GuiConsoleEnabled=1`).
2. Search (right-click the bar for filters: **Instances only**, regex, by class/property) for `Widget`,
   `HUD`, `Menu`, `Text`, or the specific container.
3. Note class names, property names, and function names.
4. **Add a watch** on a candidate property/row → open the **Watches** tab → **navigate the menu in-game
   and see which value flips.** That is your selection/state signal — no colour, no guessing. If nothing
   in reflection changes, the highlight is material/animation-driven → read the menu behaviourally.
5. Use **"Find functions"** to call a setter and confirm its effect before hooking it.
6. If reflection can answer it, prefer `Ctrl+H` (CXX headers) to read a class's real property/function
   list offline instead of dumping at runtime.

---

## Phase 2: Create the Mod

### 2.1 Copy Template
```
Copy: UE4ss study/template/scripts/
To:   GameDir/Binaries/Win64/Mods/GameAccessibility/scripts/
```

### 2.2 Enable the Mod
Add to `Mods/mods.txt`:
```
GameAccessibility : 1
```

### 2.3 Test Basic Loading
Launch game, check console for:
```
[GameAccessibility] v1.0.0 loading...
[ScreenReader] ...
[KeybindManager] Default keybinds registered
[GameAccessibility] Fully loaded
```

---

## Phase 3: Game-Specific Customization

### 3.1 Identify Key Classes
Use the discovery helpers from `utils.lua` in the UE4SS console or a test script:
```lua
local Utils = require("utils")

-- List all widget types
Utils.ListInstances("UserWidget")

-- Inspect a specific object
local PC = FindFirstOf("PlayerController")
Utils.DumpProperties(PC)
Utils.DumpFunctions(PC)
```

### 3.2 Customize game_state_tracker.lua
Add detection logic in `OnActorBeginPlay`:
```lua
function GameStateTracker.OnActorBeginPlay(className, actor)
    if className == "BP_MainMenu_C" then
        GameStateTracker.SetState(GameStateTracker.STATES.MAIN_MENU)
    elseif className == "BP_HUD_C" then
        GameStateTracker.SetState(GameStateTracker.STATES.GAMEPLAY)
    end
end
```

### 3.3 Customize ui_reader.lua
Add widget reading for the game's specific UI:
```lua
function UIReader.OnNewWidget(widget)
    local className = widget:GetClass():GetFName():ToString()

    if className == "WBP_DialogueBox_C" then
        local text = UIReader.ReadWidgetText(widget, "DialogueText")
        if text then announcer.Speak(text, false) end
    end
end
```

### 3.4 Add Game-Specific Hooks
In `main.lua`, add hooks for important game functions:
```lua
-- Example: Hook dialogue system
RegisterHook("/Script/GameName.DialogueManager:ShowDialogue", function(self, Text)
    local dialogueText = Text:Get():ToString()
    Announcer.Speak(dialogueText, true)
end)
```

### 3.5 Add Game-Specific Keybinds
In `keybind_manager.lua`:
```lua
-- Example: Read inventory
RegisterKeyBind(Key.I, {ModifierKey.CONTROL}, function()
    ExecuteInGameThread(function()
        -- Read inventory contents
    end)
end)
```

---

## Phase 4: Testing & Iteration

### 4.1 Hot Reload Cycle
1. Edit Lua files
2. Press **Ctrl+R** in game (or "Restart All Mods" in GUI)
3. Changes take effect immediately
4. Check console for errors

### 4.2 Test with Screen Reader
1. Ensure NVDA/JAWS is running
2. Ensure Tolk DLLs are in place
3. Navigate through all game screens
4. Verify all announcements are clear and timely

### 4.3 Common Issues
- **Widget not found**: Class name might differ, use `FindAllOf("UserWidget")` to discover
- **Hook not firing**: Function path might be wrong, check C++ header dump
- **Crash on property access**: Use `Utils.SafeGet()` instead of direct access
- **Text is empty**: Property might be FText not FString, try different property names

---

## Phase 5: Polish

1. Adjust announcement timing (anti-spam intervals)
2. Add audio cues for spatial/contextual info
3. Ensure all menus are navigable
4. Test full game flow: main menu → gameplay → all features → end
5. Document which features are accessible
