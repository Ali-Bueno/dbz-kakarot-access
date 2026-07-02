# UE4SS Lua API reference

Quick-lookup reference for the UE4SS Lua API when building mods. Companion docs:
[discovery-tools](ue4ss-discovery-tools.md) (dumpers + Live View),
[settings-reference](ue4ss-settings-reference.md), [accessibility-patterns](accessibility-patterns.md),
[compatibility-and-crashes](ue4ss-compatibility-and-crashes.md).

> **Doc caveat:** the official API list is "mostly complete" but not fully updated since 2.5.2 — verify
> against your build. Lua runtime is **PUC Lua 5.4** (real Lua), so `require` of a native Lua C module
> works (the PRISM/Tolk bridge path).

---

## 1. Object finding & iteration

| Function | Notes |
|---|---|
| `FindFirstOf(shortClassName)` | first non-default instance; returns an invalid object if none (check `:IsValid()`). |
| `FindAllOf(shortClassName)` | table of all non-default instances, or `nil`. **O(all UObjects)** — see Performance. |
| `StaticFindObject(fullPath)` | any UObject by full path — best for classes/functions/assets, not instances. |
| `FindObject(class, name, excludeFlags, requireFlags)` | flexible single find; either class or name may be nil (not both). |
| `FindObjects(count, class, name, excludeFlags, requireFlags, exactClass)` | multiple; `count=0` = unlimited. |
| `ForEachUObject(cb(obj, chunkIdx, objIdx))` | iterate the entire GUObjectArray. |

```lua
local pc = FindFirstOf("PlayerController")
if pc:IsValid() then print(pc:GetFullName().."\n") end
```

### Performance (READ THIS)
- **`FindAllOf`/`FindObjects`/`ForEachUObject` scan the whole GUObjectArray every call** (tens of
  thousands of objects). Calling one every tick per adapter stalls the game thread → input lag. **Cache**:
  find a container once, keep the ref (pooled UI widgets persist for the session — closing only Collapses
  them), and re-scan only when the cached ref goes invalid. Cache list results too, refreshing every few
  seconds. See the caching pattern in [accessibility-patterns](accessibility-patterns.md).
- `bUseUObjectArrayCache=true` makes UE4SS keep its own fast object cache (backs these finders) — but the
  Lua call still materializes/filters a table each time; caching in your mod is still needed.
- Prefer **event-driven** acquisition (`NotifyOnNewObject`) over polling `FindAllOf` where possible.

---

## 2. Hooks & event-driven detection

### RegisterHook(funcPath, callback) → preId, postId
Hook a **UFunction by path** — fires for **ALL instances** of that class and subclasses (inheritance
respected). `self` in the callback is the specific instance.
```lua
local pre, post = RegisterHook("/Script/Engine.Actor:BeginPlay", function(self, ...)
    print("BeginPlay: "..self:GetFullName().."\n")
end)
```
- **Callback params:** `self` first, then the UFunction's params. **All params except strings, bools, and
  FOutputDevice are WRAPPED** — read with `param:Get()`, write with `param:Set(v)`. (This `:Get()/:Set()`
  rule is for HOOK PARAMS only — normal `obj.Prop` access on a UObject does not need it.)
- **Pre vs post:** the same call registers both; return a value to override, `nil` to pass through.
- `RegisterULocalPlayerExecPreHook/PostHook` callbacks return **two** values:
  `(overrideReturn, shouldCallOriginal)`.
- `UnregisterHook(funcPath, preId, postId)` — pass both ids.
- **Gotcha:** on some games UE4SS can't safely install the dispatch detour → calling RegisterHook can
  crash at boot. This is downstream of address-resolution failures — fix the engine-version override
  first (see compatibility doc). A mod calling RegisterHook installs the ProcessInternal detour **on
  demand** regardless of the `[Hooks]` flags.

### NotifyOnNewObject(fullClassName, cb(newObj))
Fires when an instance of the class (or a subclass) is **constructed**. Great for reacting to a widget/HUD
appearing instead of polling. No "on destroyed" callback exists.
```lua
NotifyOnNewObject("/Script/UMG.UserWidget", function(w)
    print("new widget "..w:GetFullName().."\n")
end)
```

### RegisterCustomEvent(name, cb)
Fires when a Blueprint function/event with that name is called.

### Lifecycle hooks
```lua
RegisterBeginPlayPreHook / PostHook(cb(Actor))
RegisterInitGameStatePreHook / PostHook(cb(GameState))
RegisterLoadMapPreHook / PostHook(cb(...))
RegisterProcessConsoleExecPreHook / PostHook(cb(Context, Cmd, Rest, Ar, Executor))
```
Use LoadMap/BeginPlay to detect level/state changes cheaply (no polling).

---

## 3. Threading & execution
- `ExecuteInGameThread(cb)` — **required** for any UObject read/write (runs on the game thread; dispatched
  via `EngineTick` by default, see `DefaultExecuteInGameThreadMethod`).
- `ExecuteAsync(cb)` — off-thread.
- `ExecuteWithDelay(ms, cb)` — once, after a delay.
- `LoopAsync(ms, cb)` — repeating; `cb` returns `true` to stop, `false` to continue. Runs on a worker
  thread — do the actual UObject work inside an `ExecuteInGameThread` and **guard against backlog** (only
  queue the next step once the previous finished) so a busy game thread doesn't pile up late steps.

---

## 4. Input, console, files
- `RegisterKeyBind(Key.X, cb)` / `RegisterKeyBind(Key.X, {ModifierKey.CONTROL, ...}, cb)`;
  `IsKeyBindRegistered(Key.X [, mods])`.
  - Keys: `Key.A..Z`, `Key.ZERO..NINE`, `Key.F1..F24`, `Key.NUM_ZERO..`, arrows, `RETURN/SPACE/ESCAPE/TAB/
    BACKSPACE`; `ModifierKey.CONTROL/ALT/SHIFT`.
- `RegisterConsoleCommandHandler(cmd, cb(full, params, ar))` / `...GlobalHandler(...)` — return `true` to
  consume.
- `IterateGameDirectories()` → tree with `.__name` / `.__absolute_path`.
- `RegisterCustomProperty{...}` — expose an unreflected property to Lua (advanced).

---

## 5. Classes & methods

**Base wrappers:** `RemoteObject` (wraps a C++ pointer; `:IsValid()`), `LocalObject` (Lua-owned inline
value, e.g. FName/FText).

### UObject (RemoteObject)
```lua
obj[Prop]            obj[Prop] = v          -- property read/write (__index/__newindex)
obj:GetPropertyValue(name)  obj:SetPropertyValue(name, v)
obj:GetFullName()  obj:GetFName()  obj:GetAddress()  obj:GetClass()  obj:GetOuter()  obj:GetWorld()
obj:IsValid()  obj:IsA(UClass|string)  obj:IsClass()/IsAnyClass()
obj:HasAllFlags(f)  obj:HasAnyFlags(f)  obj:HasAnyInternalFlags(f)
obj:CallFunction(UFunction, ...)   obj:Reflection() -> UObjectReflection
obj:ProcessConsoleExec(cmd, nil, executor)
obj:type() -> "ObjectRef" | specific
```
**Every deref must be guarded** — calling a method on a null/dangling UObject is an **uncatchable C++
abort that pcall can't catch**. Check `:IsValid()` at each level before `.Prop`/method calls.

### UStruct (UObject) / UClass / UScriptStruct / UFunction
```lua
struct:GetSuperStruct()
struct:ForEachFunction(cb(UFunction) -> stopBool)   -- reflect functions
struct:ForEachProperty(cb(FProperty) -> stopBool)   -- reflect properties (custom dumping)
class:GetCDO()      class:IsChildOf(UClass)
```

### AActor / UWorld
`actor:GetWorld()`, `actor:GetLevel()`; UWorld inherits UObject.

### TArray (RemoteObject)
`arr[i]` / `arr[i]=v`, `arr:GetArrayNum()` (count), `arr:GetArrayMax()` (capacity),
`arr:GetArrayAddress()`; `#arr` and `:ForEach(cb(i, elem))` where available.

### FName / FString / FText (LocalObject)
`:ToString()` on all; `name:GetComparisonIndex()`. Constructors: `FName("x")` / `FName(index)`,
`FString("x")`, `FText("x")`.

### UEnum
`:GetNameByValue(v)`, `:ForEachName(cb)`.

### FOutputDevice
Hook-param type (console output sink); passed directly (no `:Get()`).

### Mod (via `ModRef`)
`Mod:SetSharedVariable(name, value)` / `Mod:GetSharedVariable(name)` — cross-mod, **survives hot reload**
(good for keeping a native-bridge handle alive across reloads). `Mod:type()` → `"ModRef"`.

### UEHelpers
`local UEHelpers = require("UEHelpers")` → `:GetPlayerController()`, world/engine helpers.

---

## 6. Flags & constants
- `EObjectFlags`: `RF_NoFlags/Public/Standalone/Transient/ClassDefaultObject/ArchetypeObject/…/RF_AllFlags`
  (combine with `|`).
- `EInternalObjectFlags`: `PendingKill/Unreachable/Native/RootSet`.
- `PropertyTypes` (for `RegisterCustomProperty`): `ObjectProperty/IntProperty/FloatProperty/StrProperty/
  BoolProperty/NameProperty/TextProperty/EnumProperty/ArrayProperty/StructProperty/ClassProperty/…`.
