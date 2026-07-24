# Working notes — Dragon Ball Z Kakarot accessibility mod

Session knowledge that used to live in a local assistant-memory folder on one machine. It is
**versioned here instead**, so it survives a machine change and so anyone contributing through GitHub
sees the same history and the same rules. Each file is one topic, written as it was learned (dates
included) — findings, dead ends, and the reasoning behind decisions.

**Read order when picking the mod back up:** [`STATUS.md`](../../../STATUS.md) first (where the mod is
*right now*), then whichever note below covers the area you're touching. The engineering rules
themselves live in [`CLAUDE.md`](../../../CLAUDE.md) / [`PRINCIPLES.md`](../../../PRINCIPLES.md); these
notes are the evidence behind them.

> Two conventions worth knowing before you read: **nothing here is a guessed value** — every offset
> names the source that produced it (header dump, Ghidra, runtime memory diff). And a note that says
> *PENDING in-game verify* means exactly that: written, compiled, never seen working.

## Ground rules and working agreements

| Note | What it settles |
|---|---|
| [Delegate recon sweeps](feedback-delegate-recon-sweeps.md) | Chains of dump/header greps get delegated; only pinpoint reads stay inline |
| [Pane-live gate for new menus](feedback-pane-live-gate.md) | Pooled panes gate on visibility 0 + opacity — never `on_screen` + content (user directive, 2026-07-15) |
| [Dumps convention](dbz-kakarot-dumps-convention.md) | Discovery dumps are `dump_*`, always in `mod/KakarotAccess/Scripts/dumps/`, never the game folder |
| [Language preference](user-language-preference.md) | Conversation language for this project |
| [Screenshots folder](user-screenshots-folder.md) | Where the user's captures land, and how to pair them with F7 dumps |

## Engine, tooling and reverse engineering

| Note | What it settles |
|---|---|
| [Engine facts](dbz-kakarot-engine-facts.md) | UE 4.21 + UE4SS; the reflection rules that abort the game if broken |
| [Decompilation setup](dbz-kakarot-decompilation-setup.md) | SteamStub DRM (unpack before Ghidra); hidden indices live in non-UPROPERTY tail gaps; `mem_bridge.dll` stays separate |
| [Pak index (offline)](dbz-kakarot-pak-index.md) | 348k asset paths grep-able without launching the game; `LoadAllAssetsBefore*` must stay 0 |
| [UI map](dbz-kakarot-ui-map.md) | Full UI/menu/dialogue/combat/nav structure from the CXX header dump |
| [Input icons reference](dbz-kakarot-input-icons-ref.md) | The inline `<inputicon>` / KeyConfig glyph system |
| [NPC names](dbz-kakarot-npc-names.md) | Why there is no safe automatic name resolver on this game |
| [UE4SS screen-reader template](ue4ss-screenreader-template.md) | The reusable PRISM bridge, for the next UE4SS game |
| [SparkingZero reference scope](sparkingzero-reference-scope.md) | Reuse its bridge only — not its hooking/polling design |

## Solved subsystems (the offsets and the technique that found them)

| Note | What it settles |
|---|---|
| [Crash ledger](dbz-kakarot-crash-bug.md) | **Every crash this mod has had, its signature and its fix.** Read before touching any UObject access |
| [Performance architecture](dbz-kakarot-perf-architecture.md) | The screen directory, scan budgets, the watch lane, quiet mode — and the experiments that froze the game |
| [Community board](dbz-kakarot-community-board.md) | Board + emblem grid: mode machine, cursor offsets, the latency saga |
| [Status level](dbz-kakarot-status-level.md) | Enemy level/HP offsets, pinned by live vtable getter-chain decoding on a no-RTTI exe |
| [Skill tree lock](dbz-kakarot-skilltree-ghidra-next.md) | Per-node state byte via Ghidra — and the origin of the dumps-vs-decompiler rule |
| [Skill palette](dbz-kakarot-skillpalette-done.md) | Slot-button reader; the plate-border cursor pattern |
| [Save/Load + Items](dbz-kakarot-saveload-items-done.md) | Native selection indices; why stale-UI menus need native flags |
| [Fast-travel internals](dbz-kakarot-fasttravel-internals.md) | World/area map internals; lazily-spawned icons and the wake-nudge |
| [Radar picker](dbz-kakarot-radar-picker.md) | The R3 modal target menu, the XInput hook, and why raycast avoidance is dead here |
| [UI cache: no notify feed](dbz-kakarot-event-driven-ui-cache.md) | The reversed rule: construction notifies fire on the loader thread and corrupt the Lua state |

## Project history and open threads

| Note | What it settles |
|---|---|
| [Accessibility plan](dbz-kakarot-accessibility-plan.md) | The original plan and the running build log — the longest note here |
| [Localization + config menu](dbz-kakarot-localization-config.md) | External TXT localization (13 languages) and the in-game config menu |
| [Map travel + pending](dbz-kakarot-map-travel-and-pending.md) | The map open/close rule and the open TODO list as of 2026-07-10 |
| [Mod installer tool](mod-installer-tool.md) | The reusable config-driven GitHub-release installer used to ship this mod |
