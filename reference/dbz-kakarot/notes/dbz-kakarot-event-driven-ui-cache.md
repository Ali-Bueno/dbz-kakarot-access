# dbz-kakarot-event-driven-ui-cache

> REVERSED — never feed UI caches from NotifyOnNewObject in Lua; UE4SS delivers it on the async LOADER thread and it corrupts the lua_State (crashed Kakarot twice, 2026-07-14). Poll cached refs instead.


**This REVERSES the "screen detection must be event-driven" rule I wrote earlier the same day** (commits
14ac601 / b118aeb). The event feed was REMOVED on 2026-07-14: it was the cause of both crashes, not a
victim of them. CLAUDE.md §8 now carries the corrected rule.

**Why:** UE4SS delivers `NotifyOnNewObject("/Script/UMG.Widget")` on the engine's **async LOADING thread**
as well as the game thread — MEASURED, not assumed, with a new `mem_bridge.thread_id()` export:
`widget notify thread(s): 5744 (FOREIGN!), 38620 (game thread)`. The callback's Lua then runs on the SAME
`lua_State` as the poll loop, concurrently, and even a two-line stash (`t[#t+1] = w`) races the allocator
and the incremental GC — freeing userdata the game thread still holds. Symptom: a cached widget passes
`IsValid()` and reports a NULL UObject on the very next member call ("Tried calling a member function but
the UObject instance is nullptr", `GetArrayNum` → `screen_skillcustom.lua:92`), once per tick forever (the
poisoned entry was inserted at the HEAD of the cached list, shadowing the real screen), until UE4SS's
**uncatchable** C++ throw (0xe06d7363) killed the process. The earlier AV at a garbage address
(0x8e6fe4ac) was the same corruption. `pcall` catches neither.

**How to apply:**
- **Dead ends — never retry:** `ExecuteInGameThread` INSIDE the callback (the wrapper is itself Lua,
  already running on the foreign thread), and moving the stash into a mutex-protected C module (reaching C
  still executes Lua bytecode). A real event feed must be armed from a NATIVE plugin, outside the VM.
- **Measure, don't assume:** compare `GetCurrentThreadId()` against the game thread before trusting ANY
  engine callback — UE4SS documents none of this. `Core.begin_scan_tick` stamps `_G.__KakarotGameTid`;
  actor-spawn notifies (GameModeBase → the transition gate, the mod's only remaining notify) are
  game-thread and safe, and now log which thread they ran on.
- **Detect screens by polling cached refs, made fast the honest way** (free in the steady state): pooled
  widgets stay valid while a screen is merely CLOSED, so a class only earns fast re-scans (`CHURN_FAST`
  ~1 s, `CHURN_WINDOW` ~15 s) once its cached instances all go INVALID — destroyed, about to be recreated.
  Never scan per tick and never shorten the global refresh: that is what lags navigation. Replaces the old
  churn-force ([dbz-kakarot-saveload-items-done](dbz-kakarot-saveload-items-done.md)), now deleted.
  **The empty-vs-dead distinction is load-bearing** — I got it wrong first and half the menus (main menu
  included) went SILENT: an EMPTY cached list means "never opened", the normal state of nearly every menu.
  If empty also forces a re-scan, all ~25 idle classes ask EVERY tick, the 3-scans-per-tick budget goes to
  whoever probes first, and the adapters at the BOTTOM of the registry never get scanned at all. Empty →
  slow `DEAD_BACKOFF` (~4 s). Dead-but-not-empty → fast lane.
- **Every TArray read goes through `Core.array_of(owner, name)`:** `owner[prop]` returns an INVALID
  RemoteObject rather than nil, so `arr ~= nil` is NOT a validity check and a raw `GetArrayNum` on it is
  the uncatchable throw. Migrated: skillcustom, skilltree, tutorials, status, ui_archetypes.

Related: [dbz-kakarot-crash-bug](dbz-kakarot-crash-bug.md), [dbz-kakarot-engine-facts](dbz-kakarot-engine-facts.md), [ue4ss-screenreader-template](ue4ss-screenreader-template.md).
