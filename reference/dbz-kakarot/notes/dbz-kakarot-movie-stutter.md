# Movie and cutscene audio slowing down and stuttering (2026-10-15)

> **Status: CODED, never played by us.** Lint and the offline suite are clean (13 test files).
> The fix merges our code audit with a player-side measurement by Matthew (below), who ran his
> own version of three of these changes for one evening. The checklist at the end is the
> verification.

## Report

Several players: the audio of the game's videos "gets slow" and stutters; one blamed the menu
reader. Same family as the July "audio cutting every ~1.5 s in cutscenes", which the A/B test
(`KakarotAccess : 0` = smooth) pinned on this mod and the scan stats pinned on full object scans:
every `FindAllOf` is ~65 ms of game thread (worst 455 ms), and a stall that long is audible. See
the 2026-07-16 entries in the [status history](dbz-kakarot-status-history.md).

## Field measurement (Matthew, 2026-10-15, v0.1.6 with NVDA)

He logged every scan from `timed_findall` to a file, because the Ctrl+F5 dump refuses to run
while a cutscene owns the screen. In the 48 s real-time scene before the first Vegeta fight
(minimap off), there were **125 scans at ~117 ms each, about 30% of the scene frozen**, and
`quiet=false` for every one of them.

| Class | Scans in 48 s | Why it ran |
| --- | --- | --- |
| `ATTitleLevelScriptActor` | 43 | The `tt` root of the save/load and options chains. It cannot exist in gameplay, so `find_root` retried every ~2 s forever. Root scans bypassed `timed_findall`, so Ctrl+F5 never showed them. |
| `CompZ_*` (6 classes) | 28 | Scan-path; the adapter gates only on `Core.free_roam`. |
| `Map_World_Icon_C` | 20 | Quiet-exempt, probed in every non-free-roam state. |
| Community trio | 12 | Scan-path; gated only on `Core.free_roam`. |
| 30 s alive-pool refreshes | 22 | Deferred only in quiet. |

Quiet never engaged for two reasons:

- the battle HUD stays "on screen" through battle cinematics, and `not battle` vetoed quiet;
- every fresh pad press reopened `hot_until`, which also lifts quiet.

His player had the stutter **before v0.1.6**, so the description clock is a contributor, not
the origin. Speech (49 calls, 8 ms worst) and braille were ruled out.

## Our audit (code reading, same day)

1. **The cutscene-description clock (v0.1.6).** It polled `PreRenderDemoActor` and
   `LevelSequenceActor` every 100 ms whenever the minimap was hidden. Both are quiet-exempt and
   never escalate, so whichever pool was absent re-scanned every ~4–5.5 s *through quiet mode*.
   A movie has no sequence actor (226 of the 236 movies have no cues and fell through to the
   sequence probe), a GDM scene has no movie actor, and neither exists in a menu or a fight. It
   also ran with the option **off**, and it called `GetFullName` on every sequence actor every
   poll. In Matthew's GDM scene both pools were alive, so it only shows in the refresh row.
2. `Map_World_Icon_C`, as in the table.
3. A movie in a world where `mm` does not resolve would turn quiet off entirely. Unknown.
4. Presses during a scene lift quiet. This matches his finding.

## Fix (the merged version)

- **`cinema.lua`.** A scan-free movie probe through the game's own demo manager:
  `UAT_BlueprintFunctionLibrary::GetDemoManageComponent` → `GetPreRenderDemoCtrl` →
  `.DemoActor` (+0x48) → `.ManaPlayer` (+0x358) → `IsPlaying()`. The headers are `AT.hpp:29950`,
  `28163`, `29221` and `16125`.
  - It is gated on the cached `mm` root (`Directory.root_cached`, which never scans).
  - The controller is cached per world, and the result is memoized for 50 ms.
  - The off edge must last 0.5 s, and both flips are logged as `cinema -> movie` / `cinema -> none`.
- **Movie mode in `ui_core`** (`Core.set_cinema`, set by `ui_registry`). Stricter than quiet:
  - Boost, watch and the narrative exemptions all wait; only an absent `Xcmn_Subtitles_C` pool
    may still be scanned. Root re-finds wait too, except `gi` (the transition gate).
  - `Core.free_roam` answers from a scan-free minimap peek.
  - The registry drops movie mode while a real (non-dialogue) adapter owns the screen or the
    minimap is visible, so a skip prompt reads exactly as before.
- **No title-actor scans in gameplay** (Matthew's). `getters.tt` answers from the cache while the
  `mm` or `hud` root is live. Root scans now go through `Core.findall`, so Ctrl+F5 counts them.
- **A null owner reads absent in gameplay** (Matthew's, with a valve). When a chain's base root
  is live (`mm`, `hud`, `gi`; `fm/bm/cm` count as `hud`) but the owner hop is null, `resolve`
  answers `{}` instead of falling back to a scan. A title-actor chain counts as absent too.
  - **Valve:** inside the registry's hot window (`Core.hot()`, ~1 s after a press or a screen
    commit) the old scan fallback still runs, so a screen opened from a context the MAP does not
    know is still found.
  - **Log:** if that fallback finds a live instance, `directory: <cls> found by scan with a null
    owner` is printed once, naming the missing chain. The 2026-07-15/16 history (save/load at
    the title, the story-flow community board) is exactly that shape.
- **Quiet in battle cinematics** (Matthew's, softened). The battle HUD vetoes quiet only while
  the player is pressing (any fresh press in the last ~3 s, `FIGHT_TICKS`). A fight keeps the
  old cadence, so event-less battle popups behave as before; a scene the player watches goes
  quiet. The battle-end window still lifts quiet for 3 s, which Matthew's version lost (the
  toasts, level-up banner and quest reward sheet are scan-path and appear right then).
- **Face presses inside quiet** (Matthew's, softened). `quiet_lift_until` follows `hot_until`
  except for A/B/X/Y pressed while quiet already holds (a line advance or an attack). Start,
  Back, shoulders, d-pad and thumbs still lift it, and so does any press made outside quiet.
  Matthew's version let no press lift quiet, which risks the map-d-pad deadlock (CLAUDE.md §8)
  for any scan-path screen opened by a press from a quiet state. Dialogue already behaved like
  this while a subtitle line was up (the `scan_quiet` branch).
  - **The one screen a face press can reveal in a scene is a dialogue choice**, and its rows
    and window are scan-path. So the exception applies only once the choice-row pool
    (`Choice_Cmd_C` / `AT_UIChoiceCmd`) is cached alive (`Core.pool_alive`), when a new choice
    reads without any scan. In a fresh session or after a map load, face presses lift quiet
    exactly as before until that pool is found again.
  - `screen_choice` also finds the quest Yes/No window through the mapped QuestSub
    (`UAT_UIQuestSub.UIChoiceWin` @0x430, `AT.hpp:35478`), a pointer read that quiet cannot
    delay. It is tried first and only adds a candidate.
- **Quiet flips are logged**: `quiet -> on|off (battle=…)`.
- **`audio_description`:** turned off, it probes nothing; the movie clock comes from
  `Cinema.movie()` (the class scan is only the title fallback); a playing movie without cues
  keeps the sequence probe away; no sequence probe under the live battle HUD; sequence actor
  names are cached per handle.
- **`screen_map`** probes its icon pool only while the `Map_World_C` host (directory pointer) is
  on screen. Matthew gated it on a press or free roam in the last 2 s; the host test is exact.
- **Considered and reverted:** keeping the GameInstance root across map loads. It keeps the
  world-epoch poll reading through the gate, which changes the play-verified gate timing.

## Decompiled facts (Ghidra, `code/decompiled/_democtrl_*.c`, `manual_140d2*.c`)

- `GetDemoManageComponent` has its exec at `0x141755be0`; the implementation chain is
  `0x140da89f0` → `0x140d271e0`. It reads a global singleton (`DAT_14592f0e8`) and looks the
  component up in that object's by-class map at `+0x550`. Every hop is null-checked, with an
  inlined `IsChildOf`, and the failure path returns 0. The singleton's type and writer were not
  identified, which is why the call is gated on a live gameplay `MenuManager`.
- `GetPreRenderDemoCtrl` has its exec at `0x141841620`; the implementation is `0x141040df0`, an
  inlined `TMap::Find` on `DemoCtrlMap` (`+0x120`) for key 4 (PreRender), then `IsChildOf`. It
  returns null and does not fault.
- **Unknown:**
  - Whether `DemoActor` is cleared after playback. `IsPlaying()` is the test, and `Core.valid`
    (with `Mem.alive`) guards a freed actor.
  - A scan-free "sequencer demo active" signal. `Register/UnregisterSequenceActor` are Blueprint
    functions, and `SequenceActorMap` is a TMap of weak pointers, which we must not read.
  - **Next lead for GDM:** `UATDemoCtrlBase.ResourceList` (`+0x38`, a real
    `TArray<AATDemoResource*>`, readable with `Core.array_of`), on the ctrl that drives GDM
    scenes. Which getter returns that ctrl is not confirmed (`GetCinemaDemoCtrl` is key 1).
    Nobody knows yet whether it is filled while a GDM scene plays and emptied afterwards; no
    writer was decompiled. A live read of its count during a GDM scene and in free roam would
    answer it.

## Known limits

- Real-time (GDM) cutscenes get ordinary quiet, not movie mode. Anything quiet exempts still
  scans there while its pool is absent.
- `CompZ_*` and the community trio still gate only on `Core.free_roam`, so they scan in menus
  and whenever quiet is lifted. A tighter precondition (a host pointer to test first) was not
  attempted in this batch.
- Movies not driven by the demo controller's `DemoActor` are not detected: the Dragon Ball wish
  movie (`UDragonBallManager.DragonBallMovie`), the Z-Encyclopedia (`UCompZMenu.DemoActor`) and
  the title opening (which keeps the scan fallback).
- Matthew's per-scan file log was not merged. Ctrl+F5 now counts root scans; a cutscene-time
  log can come back as a debug-only tool if the checklist below is not conclusive.

## Live checklist

1. Play a story movie with descriptions **on**, then **off**. No slowdown or stutter; the log
   shows `cinema -> movie` at the start and `cinema -> none` at the end.
2. Play the scene before the first Vegeta fight without pressing anything. The log should show
   `quiet -> on (battle=true)` and the scene should be smooth. Then mash A through it: still
   smooth.
3. Ctrl+F5 after an hour of play: `ATTitleLevelScriptActor` should have no scans after the save
   loaded, and `PreRenderDemoActor`, `LevelSequenceActor` and `Map_World_Icon_C` no growth
   during movies.
4. Check that the Raditz arrival movie still speaks its descriptions in time, and that the title
   opening still does (fallback path).
5. Press Start during a movie: the skip prompt reads its question **and** Yes/No.
6. A dialogue choice inside a cutscene still reads, and so does a side quest's accept/decline
   (the face-press rule's main risk, mitigated as above). Try one right after loading a save.
7. Battle end: results, level-up banner, item toasts and a quest reward sheet still read.
8. Open save/load and options from the title AND from the pause menu (the `tt` change).
9. Grep the log for `found by scan with a null owner`. Any hit names a MAP chain to add.
10. Play a GDM scene with cues: the `audio description source: …_GDM` line still appears.
