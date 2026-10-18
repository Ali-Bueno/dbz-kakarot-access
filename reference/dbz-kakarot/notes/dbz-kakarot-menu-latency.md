# dbz-kakarot-menu-latency

Menu entry latency, measured unattended on 2026-10-16 with the dev channel's virtual pad (see
the end of this note). Time = virtual press to the first spoken line, from the speech ring's
`os.clock` stamps (`kak_dev screen`) and the `screen -> <adapter> t=` commit lines in the log.

## Measured (Namek save, Gohan Lv 22)

| Screen | Before | After | Where the time goes |
|---|---|---|---|
| Title menu after the boot dialogs | ~4 s (1.5 s confirm + 2.5 s settle on every entry) | ~2.8 s after the last boot dialog; ~1 s back from Options/Load (game anim) | Settle gate now guards the boot sequence only |
| Pause ring (Start) | <0.5 s | same | — |
| Comunidad / Sistema submenus | ~0.5 s | same; no "Main menu, System" flash between a submenu row and its screen | ring shows the bare row for ~0.4-0.6 s around a submenu screen |
| Options | ~0.9 s | same | game |
| Story (Historia) | 1.5-2.4 s, read twice, objective instead of episode | ~0.3 s less, episode + level + summary, read once | ~0.8 s ring close + 0.7-1.9 s until the detail title exists (game) |
| Items | 1.9-2.6 s, "Recovery, empty list", first item never read | same speed, reads the first item | game opening; the empty hint is the tab INDEX on entry |
| Party / Characters | 0.9-2.0 s | same | game |
| DLC | ~2.3 s | same | game |
| Community board | ~1 s | same | — |
| Status page | — | labelled "HP"/"Ki" gauges; d-pad walks 7 blocks | — |

The full-screen menus behind the ring are dominated by the game: ~0.8-1 s for the ring to
close, then the new screen's own opening. The mod's own overhead there is the commit confirm
(100-200 ms) and per-adapter settles.

## Later in the same session

- **Story read the objective instead of the episode.** On opening, BOTH `All_Win_Story` and
  `All_Win_Task` are `Visible(0)`; after the first X the hidden one is `Collapsed(1)` and the
  shown one `HitTestInvisible(3)`. `task_mode()` now needs the task pane visible AND the story
  pane not. The X toggle re-reads once (it used to say the new entry, then its context).
  Dev probe: `kak_dev story`.
- **Quiet in the ring -> screen gap.** With an ambient conversation nearby, quiet engaged in the
  second between the ring closing and Options appearing, and Options took ~5 s. Closing the
  ring now keeps quiet off for 3 s (`quiet_lift_until`, ring only, so cutscene prompts keep it).
- **Title repeats.** Ending the boot gate at the first settle let the later boot gaps say "Main
  menu, Continue" three times. The gate now ends on a real cursor move or a 5 s hold.
- **An objective right after `kak_reload` is not a bug.** `quest_objective`'s diff gate is
  module state, so every reload re-announces the current objective on the next HUD reading.
- **The dev channel froze the mod.** After ~30 minutes of one scripted command a second, every
  KakarotAccess loop stopped (no log, process responsive, the Inspector still answering; last
  breadcrumb `ui.update screen_characters`). The channel read its file on the LoopAsync worker
  and queued an `ExecuteInGameThread` per command, the race pad_poll.lua documents. It is now a
  pad_poll stepper on the game thread, re-attached by `App.start`; the freeze did not recur.

## Fixes that were about speech, not detection

- **Requeue replays stale lines.** `speech.lua` re-appends unfinished queued lines after every
  interrupt. That is right for a pickup toast and wrong for anything that describes the screen
  the player is navigating: a late tooltip came back after the NEXT item, the keyhelp line and
  the board summary came back after every d-pad move, and the world map's whole travel list
  would replay after each d-pad step. Those are now `no_requeue`. And when the player opens a
  menu from the field (Start/Back), the registry drops queued requeues — not protection (the radar's "Tracking main
  quest" was spoken after "Main menu, Items").
- **Announcer tab branch** spoke the tab label alone and swallowed the item; it now speaks
  "tab, item".

## World map (code only, not verifiable on this save)

The world map is locked in the Ginyu arc ("you can't go to the world map right now"). The
icon class is on the scan path with a ~4-5 s backoff after an empty scan, so a scan that ran a
moment before the lazily created icons existed held the map silent. While the wake window is
open the class is re-scanned every 2 polls; `Map.confirm_ticks = 1`; and a d-pad press made
before the map commits is replayed. Untested live.

## The virtual pad (dev channel)

`press <BTN[+BTN]> [ms]`, `stick <x> <y> [ms]`, `unplug`, `commu`. `input_bridge` reports pad
0 as connected for a 10-minute lease (posting `WM_DEVICECHANGE` so UE4 re-polls an empty slot)
and ORs the buttons in before the snapshot is cached, so the mod's own pad readers see the
press too. Both are leases; nothing can stay pressed. With nobody at the PC this is enough to
boot, load a save and walk every menu. Two helpers live only in the session scratchpad: a
press-sequence writer for `kakarot_cmd.txt`, and a `screen` reader.

Added 2026-10-18 for the race test:

- `trigger <lt> <rt> [ms]` is a third lease in the bridge; RT drives the car.
- `where`, `keys` and `objective` echo F5, F2 and F10.
- `tp`, `enemyhp` and `drive` are test shortcuts. One live session got through a story fight and
  drove a whole race unattended.

Table and caveats in [race minigame](dbz-kakarot-race-minigame.md).
