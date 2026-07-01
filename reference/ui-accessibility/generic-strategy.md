# Generic UI strategy — maximum coverage, zero duplication

> Read this **before** writing per-screen menu/inventory code. It's the architecture the concrete
> behavior specs ([menus](menus.md), [inventories](inventories.md)) plug into. Trigger phrases:
> **"accessibilize menus"** / **"accesibilizar menús"** and **"accessibilize the inventory"** /
> **"accesibilizar inventario"** both apply this strategy first, then the concrete spec.

## The problem

A game has dozens of screens (main menu, options with tabs, save/load, inventory, crafting, shop,
dialogue, map…). Writing bespoke accessibility code per screen is (a) huge, (b) duplicated — most
screens are the same handful of controls rearranged, and (c) fragile — a game update that rebuilds a
panel silently breaks a hardcoded reader.

The goal is a **hybrid** design that gets both:

- **Reuse** — one reader per *widget archetype*, shared across every screen that uses that widget.
- **Reactivity** — each screen *subscribes* to focus/update events, so dynamic UI changes re-announce
  instead of going silent.
- **Graceful fallback** — an unmapped screen still reads *something* (a tree walk) instead of nothing.

## Two layers, decoupled

```
                 ┌─────────────────────────────────────────────┐
   engine  ──►   │  Screen adapters (subscribe per screen)      │   "WHICH widgets, and WHEN"
   events        │  registry + dispatcher                       │
                 └──────────────────┬──────────────────────────┘
                                    │ normalized focus/change event
                 ┌──────────────────▼──────────────────────────┐
                 │  Widget archetype readers (reusable)         │   "HOW each control sounds"
                 │  Button · List item · TabBar · Slider ·      │
                 │  Checkbox · Dropdown · Grid cell · Dialog …  │
                 └─────────────────────────────────────────────┘
```

- The **bottom layer** never knows which screen it's on. It's a fixed set of readers, one per
  archetype, each implementing the rules already written in [menus.md](menus.md) /
  [inventories.md](inventories.md). Written once, reused everywhere.
- The **top layer** is thin per screen: "this screen exists, here's its type, here's how to hook its
  focus/update events." It classifies each focused control into an archetype and delegates. No spoken
  text lives here.

This is the [engine-adapter principle](../../CLAUDE.md#6-code-architecture-and-best-practices) applied
to UI: accessibility logic is written against normalized archetypes, not against concrete engine widget
classes.

## Step 1 — Normalize the game's widgets into archetypes

Map every concrete UI class the game uses onto **one** normalized archetype. Do the classification in
the adapter (by base class, component, prefab name, or interface), never by branching inside a reader.

| Archetype | Matches (examples) | Canonical read behavior |
|---|---|---|
| **Label / static text** | title, header, tooltip, body text | read its text (context, `interrupt=true` on screen enter) |
| **Button / action item** | menu entry, "Play", "Back", list row | name + tooltip on focus |
| **List / vertical menu item** | options list, save slots | name + state + tooltip ([menus.md vertical nav](menus.md#vertical-navigation-up--down)) |
| **Tab bar / section** | Video/Audio/Dialogue tabs | tab name only on switch ([menus.md tabs](menus.md#tabs--sections)) |
| **Slider** | volume, brightness, numeric | only the new value on change |
| **Checkbox / toggle** | on/off setting | only the new state on change |
| **Dropdown / selector** | language, resolution, cycler | only the value you land on |
| **Grid cell / slot** | inventory, hotbar, equipment, shop | item name + stats ([inventories.md](inventories.md)) |
| **Dialog / popup** | confirm, message box | message then focused button ([menus.md dialogs](menus.md#popup-and-confirmation-dialogs)) |
| **Stat / progress bar** | HP, XP, load %, cooldown | label + value/ratio when it changes |
| **Text input** | search box, rename field | label on focus, echo committed value |

If a control doesn't fit, **add an archetype** (once) rather than special-casing a screen. Two screens
that share a base widget class / prefab / navigation component → they already share a reader; don't
write a second one. Finding that shared base is the main win — see *Step 4*.

## Step 2 — Subscribe per screen (the reactive half)

Each screen adapter **subscribes** to the game's UI events instead of re-reading state every frame.
Prefer event-driven; fall back to polling only when no event exists.

Hook, in priority order, whatever the engine exposes:

1. **Focus / selection changed** — the primary signal. Fires when the user moves between controls →
   classify the newly focused widget and delegate to its archetype reader. (Unity: selectable/EventSystem
   selection; native: the game's own "current index" setter.)
2. **Screen opened / closed** — announce the screen name on open (`interrupt=true`); on close, restore
   context to whatever regains focus.
3. **UI updated / rebuilt / data-bound** — the reason this is *hybrid*, not just a one-shot scan. When a
   panel refreshes (list repopulated, value pushed from game logic, layout swapped by an update), the
   subscription re-reads the currently focused widget so it never goes stale. Diff-gate so a rebuild that
   didn't change the focused content stays silent.

If the engine gives no usable event, poll the focused-widget state at a modest cadence and
**diff-gate**: announce only on real change, never per frame. Treat this as the fallback, not the norm.

## Step 3 — Central registry + dispatcher (kill duplication)

One registry maps `screen type → adapter`; one dispatcher routes every normalized focus/change event to
the right **archetype reader**. Consequences:

- Adding a screen = register its type + point it at existing readers. No new reading code in the common
  case.
- A reader fix (e.g. how sliders announce) lands **once** and every screen benefits.
- Spoken strings stay in the single centralized, translatable string layer — never inline in an adapter.

## Step 4 — Detect shared logic before writing new code

Before writing a reader for a new screen, check whether its widgets are already covered:

- **Same base class / interface** — e.g. every option row derives from one `MenuEntry`/`Selectable`
  type → the existing list-item reader already handles it.
- **Same prefab / template / scene component** — inventory, hotbar and storage are often the *same*
  slot widget instantiated in different containers → one grid-cell reader covers all three (this is
  exactly the [inventory subsections](inventories.md#subsections) case: same reader, adapter just names
  the group on crossing into it).
- **Same navigation component** — screens sharing one navigation/controller class share focus events →
  subscribe once at that layer instead of per screen.

Rule of thumb: **if two screens would call the same archetype reader, they are one integration, not
two.** Only the subscription wiring and the group name differ.

## Step 5 — Generic fallback reader (coverage floor)

For any screen not explicitly mapped, don't go silent — walk the focused widget (or the visual tree
under focus) and read whatever text/label/value components it exposes, in on-screen order, using the
archetype readers where a control is recognizable. Partial, imperfect coverage beats an unreadable
screen, and it flags which screens still deserve a proper adapter.

## Anti-duplication checklist

- [ ] Every concrete widget class is mapped to an archetype in the adapter, not branched on inside a
      reader.
- [ ] No spoken text lives in a screen adapter — all strings in the centralized layer.
- [ ] A new screen reuses existing archetype readers; new reading code only when a genuinely new
      archetype appears.
- [ ] Screens are **subscribed** to focus/update events; per-frame polling only where no event exists,
      always diff-gated.
- [ ] UI rebuild/update events re-read focus (hybrid), so dynamic changes never leave stale state.
- [ ] Unmapped screens still hit the generic fallback reader.
- [ ] Every archetype reader obeys the concrete rules in [menus.md](menus.md) /
      [inventories.md](inventories.md) (context change → `interrupt=true`; incremental → only what
      changed).
