# dbz-kakarot-saveload-items-done

> DBZ Kakarot — Save/Load + Items menu SHIPPED (2026-07-11, commits 29ba8c1/e158030/ecb71f9); two reusable techniques (churn-force opt-in, native flag for stale-UI empty detection)


Session 2026-07-11 on the DBZ Kakarot accessibility mod. Builds on [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md),
[dbz-kakarot-ui-map](dbz-kakarot-ui-map.md), [dbz-kakarot-engine-facts](dbz-kakarot-engine-facts.md). All verified in-game and pushed to `main`.

## Shipped
- **Save/Load slots** (`screen_saveload.lua`, `AT_UIStartSaveLoad`): `UISaveLoadBar_List` is a VIRTUALIZED
  3-bar window (pool-position ≠ ordinal). Ordinal from native index `saveLoad.selectedIndex = 0x410` (+1),
  cursor bar = `windowPos = 0x418` (F4-diff confirmed; `0x414` = scroll offset). Reads FILLED and EMPTY
  slots (check `Canvas_None` FIRST — pooled `Canvas_Title` stays rendered on empty bars). SETTLE_TICKS
  debounce drops mid-scroll title-only/flashing-empty frames.
- **Item "use" character-select** (`screen_itemuse.lua`, NEW): A on a usable item → pick who uses it. Only
  the SELECTED char's `AT_UIItemMenu.WL_Start_Party_Bars` bar (UAT_UIStartPartyBarEx) animates on-screen, so
  the on-screen bar IS the selection. Reads `Txt_Name01` + `Txt_Lv01→Txt_Lv02` level-up preview + prompt.
  Registered BEFORE the item list reader.
- **Item empty categories** (`screen_list.lua` factory): item list is a fixed pool whose whole UI goes STALE
  on an empty category. Read emptiness from native flag `itemMenu.hasItems = 0x620` (0 = empty). New factory
  param `empty_off`.

## Reusable techniques (both in `ui_core`/patterns — reuse on other menus)
- **Churn-force is OPT-IN** (`Core.mark_churning(cls)`): some screens are DESTROYED and recreated on a quick
  close+reopen (e.g. `AT_UIStartSaveLoad`), leaving `cached_all`'s class-list holding the OLD collapsed
  instance → `first_on_screen` misses the new one for up to ~10s (fresh FindAllOf saw raw=2/onscreen=1 while
  cache returned nil). The churn-force re-scans a recently-on-screen class immediately (budget-gated). CRITICAL:
  it MUST be opt-in — applying it to every class lagged pause/items/radar navigation (opening/closing many
  submenus put lots of classes in the churn window, saturating the scan budget). Only saveload opts in.
- **Stale-UI menus need a native "count/has-items" flag, not UI reads.** When a virtualized/pooled list is
  EMPTY, its row text, detail pane, and visible-widget count all keep the LAST item (stale) — there is no
  fresh reflected empty signal. Find a native int via F4 mem-diff (switch populated→empty, watch it flip to 0).
- **F4 mem-diff (`dev_memdiff.lua`) is the fast path** for these non-reflected ints — beats static Ghidra when
  input is delegated / anchors aren't unique (saveload's UFunction table had no InputUp/Down; Ghidra stalled,
  F4 nailed 0x410 in 30s). Add the host class to `CANDIDATES`, widen the window to cover the class tail.

## Also this session
- Empty item categories: the cursor is FROZEN (idx=0, nothing to navigate) — there are no per-slot moves to
  announce; announcing the section "vacío" once on entry is the correct/only behavior. The announcer swallows
  the name on a tab-change tick, so `ann:reset()` once per empty-entry (gated) forces the announcement.
- `dev_memdiff` CANDIDATES now include `AT_UIStartSaveLoad` and `Start_Item_C`.
