# UE4SS API Reference

Complete reference for the UE4SS Lua API, organized for quick lookup when building accessibility mods.

---

## 1. Object Finding

### FindFirstOf(className)
Find first non-default instance by short class name.
```lua
local PlayerController = FindFirstOf("PlayerController")
if PlayerController:IsValid() then
    print("Found: " .. PlayerController:GetFullName() .. "\n")
end
```

### FindAllOf(className)
Find all non-default instances. Returns nil if none found.
```lua
local Widgets = FindAllOf("TextBlock")
if Widgets then
    for i, w in pairs(Widgets) do
        print(w:GetFullName() .. "\n")
    end
end
```

### StaticFindObject(fullPath)
Find any UObject by full path (best for classes, functions, non-instances).
```lua
local CharClass = StaticFindObject("/Script/Engine.Character")
```

### FindObject(className, objectName, excludeFlags, requireFlags)
Flexible search. Either param can be nil but not both.
```lua
local Obj = FindObject("PlayerController", nil, EObjectFlags.RF_NoFlags, EObjectFlags.RF_NoFlags)
```

### FindObjects(count, className, objectName, excludeFlags, requireFlags, exactClass)
Find multiple objects. count=0 for unlimited.

### ForEachUObject(callback)
Iterate entire GUObjectArray.
```lua
ForEachUObject(function(Object, ChunkIndex, ObjectIndex)
    print(Object:GetFullName() .. "\n")
end)
```

---

## 2. Hooks & Events

### RegisterHook(funcPath, preCallback, postCallback)
Hook a UFunction. Returns preId, postId for unregistration.
```lua
local preId, postId = RegisterHook("/Script/Engine.PlayerController:ClientRestart", function(self)
    print("Player restarted\n")
end)
```

**Important:** Params (except strings, bools, FOutputDevice) must use `:Get()` / `:Set()`.

### UnregisterHook(funcPath, preId, postId)
```lua
UnregisterHook("/Script/Engine.PlayerController:ClientRestart", preId, postId)
```

### NotifyOnNewObject(fullClassName, callback)
Fires when an instance (or subclass) is constructed.
```lua
NotifyOnNewObject("/Script/UMG.UserWidget", function(Widget)
    print("New widget: " .. Widget:GetFullName() .. "\n")
end)
```

### RegisterCustomEvent(eventName, callback)
Fires when a Blueprint event with that name is called.

### Lifecycle Hooks
```lua
RegisterBeginPlayPreHook(function(Actor) end)
RegisterBeginPlayPostHook(function(Actor) end)
RegisterInitGameStatePreHook(function(GameState) end)
RegisterInitGameStatePostHook(function(GameState) end)
RegisterLoadMapPreHook(function() end)
RegisterLoadMapPostHook(function() end)
RegisterProcessConsoleExecPreHook(function(Context, Cmd, Rest, Ar, Executor) end)
RegisterProcessConsoleExecPostHook(function(Context, Cmd, Rest, Ar, Executor) end)
```

---

## 3. Threading & Execution

### ExecuteInGameThread(callback)
**CRITICAL** for any UE object manipulation.
```lua
ExecuteInGameThread(function()
    local PC = FindFirstOf("PlayerController")
    -- Safe to manipulate UE objects here
end)
```

### ExecuteAsync(callback)
Run asynchronously.

### ExecuteWithDelay(ms, callback)
Run after delay in milliseconds.

### LoopAsync(ms, callback)
Repeating loop. Return `true` to stop, `false` to continue.
```lua
LoopAsync(500, function()
    -- Check state every 500ms
    return false
end)
```

---

## 4. Input / Keybinds

### RegisterKeyBind(key, callback)
```lua
RegisterKeyBind(Key.F1, function()
    print("F1 pressed\n")
end)
```

### RegisterKeyBind(key, modifiers, callback)
```lua
RegisterKeyBind(Key.R, {ModifierKey.CONTROL, ModifierKey.ALT}, function()
    print("Ctrl+Alt+R\n")
end)
```

### IsKeyBindRegistered(key [, modifiers])
Check if already bound.

### Available Keys
- Letters: `Key.A` through `Key.Z`
- Numbers: `Key.ZERO` through `Key.NINE`
- Function: `Key.F1` through `Key.F24`
- Numpad: `Key.NUM_ZERO` through `Key.NUM_NINE`
- Navigation: `Key.LEFT_ARROW`, `Key.UP_ARROW`, `Key.RIGHT_ARROW`, `Key.DOWN_ARROW`
- Special: `Key.RETURN`, `Key.SPACE`, `Key.ESCAPE`, `Key.TAB`, `Key.BACKSPACE`
- Modifiers: `ModifierKey.CONTROL`, `ModifierKey.ALT`, `ModifierKey.SHIFT`

---

## 5. UObject Methods

### Property Access
```lua
local Character = FindFirstOf("Character")
local JumpCount = Character.JumpMaxCount          -- read
Character.JumpMaxCount = 5                         -- write
local CanJump = Character:CanJump()               -- call UFunction
```

### Core Methods
| Method | Returns | Description |
|--------|---------|-------------|
| `:GetFullName()` | string | Full name with path |
| `:GetFName()` | FName | Object's FName |
| `:GetAddress()` | integer | Memory address |
| `:GetClass()` | UClass | Object's UClass |
| `:GetOuter()` | UObject | Outer UObject |
| `:GetWorld()` | UWorld | Containing world |
| `:IsValid()` | bool | Pointer validity check |
| `:IsA(class)` | bool | Type check (UClass or string) |
| `:IsClass()` | bool | Is UClass or derivative |
| `:CallFunction(UFunction, args...)` | auto | Call a UFunction |
| `:Reflection()` | UObjectReflection | Property metadata |
| `:type()` | string | UE4SS type string |

---

## 6. Other Important Classes

### UClass (inherits UStruct → UObject)
- `:GetCDO()` → ClassDefaultObject
- `:IsChildOf(class)` → bool

### UStruct (inherits UObject)
- `:GetSuperStruct()` → parent struct
- `:ForEachFunction(callback)` → iterate UFunctions
- `:ForEachProperty(callback)` → iterate Properties

### TArray
- `array[index]` → get element
- `array[index] = value` → set element
- `#array` → count
- `:ForEach(callback)` → iterate (callback gets index, param)
- `:GetArrayNum()` → current count
- `:Empty()` → clear

### FName / FString / FText
- All have `:ToString()` to get Lua string
- Constructors: `FName("name")`, `FText("text")`

### UEnum
- `:GetNameByValue(val)` → FName
- `:ForEachName(callback)` → iterate name/value pairs

### Mod (via ModRef global)
- `:SetSharedVariable(name, value)` → cross-mod storage (persists across hot-reload)
- `:GetSharedVariable(name)` → retrieve

---

## 7. Console Commands

### RegisterConsoleCommandHandler(cmd, callback)
```lua
RegisterConsoleCommandHandler("accessibility", function(FullCommand, Parameters, OutputDevice)
    -- handle command
    return true -- prevent other handlers
end)
```

### RegisterConsoleCommandGlobalHandler(cmd, callback)
Same but fires for all contexts.

---

## 8. Helpers

### UEHelpers module
```lua
local UEHelpers = require("UEHelpers")
local PC = UEHelpers:GetPlayerController()
```

### Simple structs as tables
```lua
local Location = {X=100.0, Y=200.0, Z=0.0}
```

### File system
```lua
for _, Dir in pairs(IterateGameDirectories()) do
    print(Dir.__name .. " -> " .. Dir.__absolute_path .. "\n")
end
```

---

## 9. Flags & Constants

### EObjectFlags (combinable with |)
- `RF_NoFlags`, `RF_Public`, `RF_Standalone`, `RF_Transient`
- `RF_ClassDefaultObject`, `RF_ArchetypeObject`
- `RF_BeginDestroyed`, `RF_FinishDestroyed`
- `RF_AllFlags`

### EInternalObjectFlags
- `PendingKill`, `Unreachable`, `Native`, `RootSet`

### PropertyTypes (for RegisterCustomProperty)
- `ObjectProperty`, `IntProperty`, `FloatProperty`, `StrProperty`, `BoolProperty`
- `NameProperty`, `TextProperty`, `EnumProperty`, `ArrayProperty`, `MapProperty`
- `StructProperty`, `ClassProperty`, `WeakObjectProperty`
- `Int8Property`, `Int16Property`, `Int64Property`
- `ByteProperty`, `UInt16Property`, `UIntProperty`, `UInt64Property`
