# dbz-kakarot-skillpalette-done

> Skill Palette reader SHIPPED 2026-07-13; reusable plate-border cursor pattern; GetSelectValue can be dead per-screen


2026-07-13 SHIPPED (commits 0b6e623 + 39c0667, verified in-game): Skill Palette / Super Attack equip
reader (`screen_skillcustom.lua`) — slot buttons ("botón Y, Masenko"), empty "---" slots, level/Ki/desc,
group-title on crossing, and the ASSIGN submenu (A on a slot → pick the acquired skill).

Reusable RE lessons (beyond what STATUS.md records):
- A `MenuListBase00:GetSelectValue()` can be **DEAD on a specific screen** (frozen 0, rows never move)
  even though the class family is the game's standard selection mechanism elsewhere. Never trust it
  without a live capture; the same screen may drive selection through per-plate widgets instead.
- **Per-plate cursor pattern**: selected plate = `SelectActiveBorder` visible AND `BaseBlinkImage`
  hidden. Containers can hold STRUCTURAL plates with highlight widgets permanently ON (here ordinals
  4/7: nil name, border+blink always on) that poison a naive "the one visible border" scan.
- Detail panes lag the cursor ~1 tick and go STALE (keep the last item) on empty slots — gate
  detail-pane data on pane-name == announced-name, never use the pane as the selection source.
- Announcer gotcha: a one-tick prefix (e.g. group title) on the spoken name makes the diff-gate
  re-announce the row without it next tick — sticky-prefix until the cursor leaves that row.
- **Movement state machine for two coexisting cursors**: when a screen has an embedded sub-list with
  NO static focus signal (list panel permanent, plate border persists, AllCurs renders in both modes),
  the mode = whichever cursor MOVED last (plate slot vs list GetSelectValue — the latter only comes
  alive when the list has focus). Cost: the A/B press itself is silent until the first move.
- **Entry announcements can be eaten silently**: pooled widgets hold stale state for ~0.5 s during the
  opening anim → the settle re-announce CUTS the entry phrase (with synth start latency ≈ perceived
  total silence). Fix: debounce the FIRST announcement after reset (SETTLE_TICKS on the read value).
- **Speech-sink tap** (`speech.lua` DEBUG_LOG → dumps/dump_speech.txt, off by default): logs every
  utterance + interrupt flag. THE tool for "it didn't speak" reports — decides in one capture whether
  the phrase was never sent, sent-then-stomped, or lost below PRISM.
Related: [dbz-kakarot-saveload-items-done](dbz-kakarot-saveload-items-done.md), [dbz-kakarot-engine-facts](dbz-kakarot-engine-facts.md), [dbz-kakarot-crash-bug](dbz-kakarot-crash-bug.md).
