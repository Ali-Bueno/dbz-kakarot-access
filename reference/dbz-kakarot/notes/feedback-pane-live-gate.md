# feedback-pane-live-gate

> User directive 2026-07-15 — every new menu adapter must gate on the pooled pane being GENUINELY LIVE (visibility 0 + opacity), never just on_screen+content


After the Kakarot cooking-latch saga (four rounds in one evening: stale "Cocina, asado
con hueso" shadowing the NPC dialogue, then the pause ring, then the emblems menu), the
user directed: **"tienes que mirar eso al implementar nuevos menús"** — check this when
implementing every new menu.

**Why:** pooled full-screen panes on this engine stay `on_screen` with readable stale
content long after closing. An adapter gated on `on_screen` + content shadows every
adapter registered below it and re-announces stale content on each dispatcher flip.
Per-screen yields (ring, emblems, …) are whack-a-mole; the user confirmed the ROOT
approach works (pause + emblems verified in game 2026-07-15).

**How to apply:** gate on engine state, not content — `GetVisibility() == 0` (Visible;
parked pooled widgets render under another ESlateVisibility) AND `RenderOpacity > ~0`
(close anims fade to 0 while visibility lags), both pcall-guarded (unreadable = live).
Plus the spoken-key suppression for flip-flop re-announces (survives `reset()`, cleared
on genuine close, `reannounce()` hook for F1). Reference: `pane_live` in the Kakarot
mod's `screen_cooking.lua`; rule recorded in the project CLAUDE.md §8. Related:
[dbz-kakarot-perf-architecture](dbz-kakarot-perf-architecture.md), [dbz-kakarot-crash-bug](dbz-kakarot-crash-bug.md).

**CAVEAT (2026-07-17, fishing-sheet + intro-cards saga):** the `GetVisibility()==0` half
applies to INTERACTIVE pooled panes (menus). PASSIVE overlays (result sheets, notice
cards, subtitles) render as HitTestInvisible(3) as their NORMAL shown state — demanding
Visible(0) holds them permanently silent. For those, gate on opacity + a genuine show
signal; the best is the game's OWN pointer/collapse flip (e.g. the `fm.InfoName`
container flips Collapsed(1)↔rendered per card — pure pointer read, no scans). Also
learned: `Core.on_screen` can be BLIND to an overlay's display path (the pooled
`Info_Name_C` hosts never render at all — the display uses a separate fm-owned
container), so when a passive overlay reads as "never on screen", trace the fm
container pointers before trusting any widget-side check.
