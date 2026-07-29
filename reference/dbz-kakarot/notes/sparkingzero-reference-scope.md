# sparkingzero-reference-scope

> Only reuse SparkingZeroAccess's screen-reader bridge; avoid its heavy UE4SS patterns


For the DBZ Kakarot accessibility mod, reuse from SparkingZeroAccess **only** the screen-reader
bridge (the `prism_bridge` Lua C module pattern). Do NOT copy its runtime approach.

**Why:** the user tested SparkingZeroAccess and it crashed a lot, read text slowly, and made
game files/levels load slower. The heavy UE4SS patterns are the cause: global
`NotifyOnNewObject("/Script/UMG.UserWidget", ...)`, global `RegisterHook` on
`TextBlock:SetText`, broad `FindAllOf` sweeps every tick, and aggressive polling loops.
(Iterating the game's huge widget lists also hard-crashes Kakarot — see [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md).)

**How to apply:** prefer targeted, event-driven or low-rate reads scoped to the specific widget
already located (direct property navigation on a cached node, e.g. `Gametitle_C_0.Gametitle_List0N`),
not global scans/hooks. Diff-gate announcements. Keep the bridge; rewrite the reading layer.

---

## Its menu architecture cannot be ported here — settled 2026-07-28, do not re-open

SparkingZeroAccess ended up with **no per-screen adapter files at all**: it polls two owner roots
(`PlayerController` and the base class `ASSMenuSceneController`) and reads the game's own
`USSMenuManager.LastFocusedWidget` property, so one reader plus a table of manager/property names
covers every screen. Kakarot's 43 hand-written `screen_*.lua` adapters look like avoidable work next
to that. They are not. **The difference is the games' class layouts, not the mods' designs.**

Measured against the CXX dump (`AT\Binaries\Win64\CXXHeaderDump\`, 1580 `.hpp`, `AT.hpp` 44 143 lines):

| Fact | Evidence |
|---|---|
| `UCFUIUserWidget` has **428 subclasses** and declares exactly one member, `PathAnimMng` (an animation manager) | `CFramework.hpp:1017` |
| `UATUIUserWidget` has **131 subclasses** and declares exactly one function, `SetForceTick(bool)` | `AT.hpp:29683` |
| **No class anywhere in the dump holds a "current/focused/selected widget" pointer.** The only `FocusWidget` is the parameter of UMG's own `UWidgetBlueprintLibrary::SetUserFocus` — engine, not game | grep for `class U*(UI\|Widget)* \*(Current\|Active\|Focus\|Selected)…` over all 1580 headers: 6 hits, all `UWidgetAnimation*` or group-title text |
| Selection state is genuinely ad-hoc: the member spelling `cursorIndex` occurs **twice** in the whole dump; the rest of the `Index` members are engine noise (`LODIndex`, `SrcLightmapIndex`, `MaterialIndex`) | census of `int32 *Cursor\|Select\|Index\|Idx*;` across `*.hpp` |
| `GetSelectValue()` — the one real game convention — is declared by **4 classes**, not by a list base | `AT.hpp:34353, 34388, 34421, 34444` |

So the UI base classes here are empty shells. A generic focus reader would have nothing to read, and
per-screen selection logic is not technical debt — it is the direct consequence of this game shipping
559 widget classes that inherit no state. Kakarot's own equivalent of Sparking Zero's real insight
("resolve owners, don't scan") already exists as `ui_directory.lua`; that part was arrived at
independently and is the piece worth keeping.

**The one thing still worth borrowing:** SparkingZero's bounded generic subtree text search
(`Engine.FindTextInSubtree` — depth 8, breadth 24, placeholder/rich-tag filtering, invoked only for
the single winning reading per tick). It needs **no** focus property, so the finding above does not
block it: it walks down from whatever host is already on screen and reads the text it finds. Kakarot
has no generic fallback at all today, so an unmapped screen is total silence; this would give it a
voice. It belongs at the END of the priority chain, behind a strict member gate
(`Core.member(o, n, true)`) — a multi-candidate probe must fail closed, per CLAUDE.md.

**Do not port** its `RegisterBeginPlayPostHook` owner-acquisition feed. Construction/spawn notifies
are banned on this game for reasons paid for twice — see
[UI cache: no notify feed](dbz-kakarot-event-driven-ui-cache.md).

**Method note:** two of the greps above were first answered by a subagent that never located the dump
and silently substituted this repo's own notes, inventing subclass counts ("≈30", "≈10"). Verify the
dump path exists before trusting any census taken from it.
