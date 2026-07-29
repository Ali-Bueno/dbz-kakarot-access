# dbz-kakarot-ue4ss-perf-audit-2026-07-28

> Review of the external UE4SS performance audit (`auditoria-ue4ss-mods-2026-07-28.txt`, Part 0 + Part A).
> Verified 2026-07-29 against the real code and against the RE-UE4SS source at tag **v3.0.1** — the exact
> version this game runs. **Four of its five code recommendations are rejected**, with reasons, so nobody
> re-opens them. The one finding worth having was in the section the audit itself marked *unverified*.

## Why this note exists

The audit is well-written and its description of the mod is largely accurate — which is exactly what makes
it dangerous to apply verbatim. Its recommendations are **profiler-shaped, not player-shaped**: every one
of them proposes a new cross-tick cache on pooled widgets, which is the single failure class this mod has
been bitten by most (Options re-entry, `screen_palette`, `ft_host`, `RegisterCustomProperty`). A stale
cache here does not cost frames; it costs a blind player **silence**, which is strictly worse than the
microseconds being chased.

## The premise, corrected

The audit's Part 0 is the load-bearing claim for its whole "chain hoisting" section:

- **TRUE:** every Lua property access does a **fresh `FindProperty(name)`** with no memoisation in the
  UE4SS binding layer. `__index`/`__newindex` (`LuaUObject.hpp:370-381`), `GetPropertyValue`/
  `SetPropertyValue` (`:487-497`) and `Reflection():GetProperty` (`:481`) all funnel into
  `prepare_to_handle` (`:644-681`), which builds an `FName` and calls `FindProperty` every time.
- **FABRICATED:** the audit's evidence. `LuaUObject.hpp:817-833` is the `push_integer` template, and the
  quoted comment *"this path is hit in `__index` for hot code paths"* **exists nowhere in the repo**.
  The claim is right; the citation was invented. Check citations before acting on them.
- **THE PART THAT MATTERS — the cost.** One property access ≈ **a microsecond**. A `FindAllOf` object-array
  walk is **~65 ms** on this game, and that number is already measured and already the mod's cost centre
  (`_G.__KakarotScanStats`). The audit is optimising **4-5 orders of magnitude below** the thing that
  actually costs frames. Hoisting a chain into a local inside one function body is free and fine; adding a
  cache lifetime for it is paying a real risk for an unmeasurable gain.
- Also refuted **for v3.0.1 specifically** (the audit describes the `main` branch): `UEHelpers` at 3.0.1 is
  103 lines, `GetPlayerController` **is** cached and `GetWorld` rides that cache, and `GetPlayer` /
  `GetPersistentLevel` / `GetGameModeBase` / `GetGameStateBase` / `GetAllPlayers` / `GetEngine` /
  `GetGameInstance` **do not exist at all**. `--disable-ue4ss` also does not exist at 3.0.1. And the claim
  that the official docs have the `FindObject` flag order reversed is **false** — code, shipped `Types.lua`
  and docs all agree on `(ClassName, ObjectShortName, RequiredFlags, BannedFlags)`. (Our own
  `ue4ss-api-reference.md` copy *did* have it reversed; fixed 2026-07-29.)

## Rejected, with the reason (do NOT re-open without a measurement)

| Audit item | Site | Why it is rejected |
|---|---|---|
| **A-1** hoist the navi-icon switcher chain | `nav_tracker.lua:468-484` | The proposed invalidation set is **incomplete**: `navi_icons` is REPLACED at `:579-582` every ~10 s in ordinary free roam, a path that never passes the clear points the fix named — an index-parallel cache would bind old pointers to new icons. The dangerous case is a **stale-but-valid** hit, and `:568-570` states outright that a recycled address passes both `Mem.alive` and `IsValid`, so the proposed "fall back on miss" net cannot catch it. Failure mode is a wrong main/sub classification, i.e. **steering a blind player to the wrong objective**. |
| **A-2** cache the quest row array | `quest_objective.lua:102-111` | This module is deliberately **not** a Registry adapter (`:4-6`), so it has **no `reset()`** — the standard "cleared on every screen change" backstop does not exist. `Quest_Navi_C` is directory-mapped, so the host is already a pointer walk, not a scan; and the file's own comment (`:50-56`) says the game pools several instances whose reads flicker, so host-address identity **is known to flap here**. Saves the cheapest hop on the mod's slowest loop (300 ms). |
| **A-3** cache `area_poi`'s detail-text chain | `screen_map.lua` | **Describes code that no longer exists.** `area_poi` is at `:558-585`, not `:551-552`, and the raw `pcall` hops it flags were already migrated to `Core.member` in commit `114b980` (HEAD). It also already short-circuits on `area_focus_key`, and it runs on the 100 ms registry, not the 20 ms pad loop. |
| **A.3** memoise the screen directory between ticks | `ui_directory.lua` | The mental model is wrong: `resolve()` is **not** called for every MAP entry per tick — only per class, by the adapter asking, and the sticky scheduler (`SWEEP_EVERY=3`) means most ticks probe one adapter. Residual is tens of microseconds. Worse, this is **shared substrate**: `Directory.peek`/`root_ok` bypass the `Transition.active()` short-circuit, so a memo seeded with a dying-world manager makes `resolve` answer "absent, no scan" for **every** `fm`-mapped screen at once — the exact fail-closed shape that has silenced this mod twice. It would also corrupt `debug_lines`, which deliberately lifts throttles so a MISS means a true miss. |

The standing rule these all reduce to: **measure with `_G.__KakarotScanStats` / Ctrl+F5 before optimising**,
and expect the answer to be `FindAllOf` on absent classes, never property reads.

## Accepted and applied (2026-07-29)

1. **`keyhelp.lua:63-73` — predicate reorder.** `Keyhelp.bar` called `GetFullName()` on *every* cached
   instance before testing visibility. On the auto-reader path (`visible_only`, polled ~2 Hz from
   `keyhelp_watch` in nearly every menu) the hidden fallback is unreachable, so only a visible instance can
   be returned: test visibility first and pay for the name once. **Zero new state** — this is why it was
   taken and a memo was not. (An earlier proposal to memoise the bar was rejected: the memo would expire
   before almost every call at 500 ms, `keyhelp.lua` is a shared module with **no `reset()` to hook**, and
   one memo would let `helpmsg()`'s hidden-fallback contract leak a previous screen's bar into the
   auto-reader — announcing actions that do not exist on this screen.)
2. **`screen_results.lua` — short-circuit on the existing `spoken` set.** `lines()` re-resolved every rank
   and digit texture on every 100 ms poll for lines already spoken, and `Results.update` then threw them
   away. Now already-spoken keys skip the texture work. Again **no new lifetime**: it reuses `spoken`,
   which is already cleared when the screen closes. **Careful detail:** `name` is still read
   unconditionally — the first detail row is prefixed with it even after the bar's own line is spoken.
3. **`screen_map.lua:424` — `cached_all` → `peek_all`.** Not from the audit; found by adversarial review.
   `ft_build` is reachable from the 20 ms pad loop (`ft_step → ft_guidance → ft_build`) and called a
   **scanning** helper there, which is precisely the rule `ft_host`'s own comment (`:397-402`) spells out.
   In practice it was harmless — when `ft_guidance` runs, the class cannot be `due`, so the call took the
   cached path — but it was one backoff expiry away from a 65 ms scan on the input loop. The pool is
   guaranteed populated by the registry-side scan in `world_icons_on_screen`, which must pass before
   `state.world` is set and `ft_guidance` is reachable at all.
   **This is NOT the fix for the still-open "map d-pad is slow to respond after the map opens."** Better
   suspects, all unmeasured: the budget-gated first `Map_World_Icon_C` scan after `Core.refresh_all`
   (`:604`), the ~2 s icon-wake window (`:76`), and the 250 ms `FT_BUILD_S` retry granularity.
4. **`package.ps1` — the release `.ini` profile is now correct BY CONSTRUCTION.** This was the audit's one
   solid finding (its item A-4). Packaging copied `UE4SS-settings.ini` verbatim from whatever the packaging
   machine had; the only thing keeping the debug console out of releases was a **comment in the ini asking a
   human to remember**. Packaging now forces `ConsoleEnabled` / `GuiConsoleEnabled` / `GuiConsoleVisible`
   to 0 in the staged copy and reports what it changed. Note these three **default to 1 when ABSENT**, so a
   missing key is not "off" and is written in — inside `[Debug]`, not appended at EOF where a later section
   would claim it.

## Not acted on, deliberately

- **A-5** — `Nav.dump` / `Nav.dump_levels` (`nav_tracker.lua:3131-4048`, 915 lines, ~52 KB) ship as dead
  source because `nav_tracker.lua` is not in the packaging strip list. Confirmed true. Their keybinds *are*
  compiled out (`Build.debug`), so nothing runs. Stripping them would mean a **source transform** on a
  4,000-line file during packaging — new machinery, new failure mode, to save 52 KB. Not worth it.
- **The busy-guard.** The audit correctly declined to "fix" it and said the reference doc should change
  instead. Done: the entry-clear rule and its reasoning are now in `accessibility-patterns.md`.
- **A separate `UE4SS-settings.dev.ini`.** Still the pending dev-workflow item in `STATUS.md`; unchanged.
  The safety half (the release profile) no longer depends on it.

## Method note worth keeping

The audit's own **"limits of this audit"** section — the ~15 `GetFullName()` sites it admitted it had not
traced — is where its two genuinely hot findings were hiding (`keyhelp.lua:66`, `screen_results.lua:134`).
Everything it *had* traced was already protected. **When a report tells you which part of it is unverified,
start there.**
