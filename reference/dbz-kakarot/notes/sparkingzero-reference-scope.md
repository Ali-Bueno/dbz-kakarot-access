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
