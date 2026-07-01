# UI accessibility — behavior specs

Prescriptive **behavior rules** for accessibilizing in-game UI with a screen reader (via PRISM) +
audio cues: how *our* mods must behave, regardless of engine. Unlike the rest of `reference/`, this
is not reverse-engineering knowledge — it's the contract the patches/hooks delegate to.

The playbook that *uses* these specs is [the root CLAUDE.md](../../CLAUDE.md) (§8). Read the relevant
doc before implementing that part of a mod.

## Documents

| Doc | Trigger phrase | Covers |
|-----|----------------|--------|
| [Generic UI strategy](generic-strategy.md) | (read first for either trigger below) | Hybrid architecture: reusable widget-archetype readers + per-screen subscription, central registry/dispatcher, shared-logic detection, generic fallback — how to cover the max number of screens with zero duplication |
| [Menus](menus.md) | "accessibilize menus" / "accesibilizar menús" | Entering a menu/screen, tabs/sections, vertical navigation, horizontal controls (sliders/checkboxes/dropdowns), popup & confirmation dialogs |
| [Inventories](inventories.md) | "accessibilize the inventory" / "accesibilizar inventario" | Item readout, subsections (belt/equipment/storage), crafting recipes (have/needed) — builds on the menu rules |

**Read [generic-strategy.md](generic-strategy.md) first**, then the concrete spec: it defines the
reusable layer (widget archetypes + subscriptions) that the menu/inventory rules plug into, so you
don't rewrite the same reader per screen.

## Shared principles (apply to every UI doc here)

- **Context change → `interrupt=true`** (new screen, new tab, dialog opens). Incremental info
  (changing a value) → `interrupt=false` and read **only what changed**.
- **Follow the game's on-screen order.** Never reorder or invent fields; read only what's present.
- **Diff-gate**: detect real changes before announcing; never repeat the same state.
- Centralize spoken strings in a single, translatable layer — no hardcoded text inside patches.

## Planned

Future UI behavior specs (HUD, combat readouts, map/world screens) go here as their own docs, added to
the table above when written.
