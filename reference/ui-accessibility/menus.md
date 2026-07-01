# Accessibilizing menus

> When the user says **"accessibilize menus"** (or the Spanish **"accesibilizar menús"**), apply exactly
> this behavior. Shared principles live in [the topic README](README.md).

## Entering a menu / screen

When a menu or screen **opens** (focus enters it), announce **in this order**:

1. The **menu name** (the title of the screen you just entered).
2. The menu's **tooltip / description** (if it has one).

Example (user presses "Load game"; on entering): `Load game, choose a save to load`.

This gives the user immediate context about where they are before they start navigating items (see
*Vertical navigation*). Use **`interrupt=true`** here because the context changed (a new screen).

## Tabs / sections

Many menus are split into **tabs** (e.g. `Video`, `Audio`, `Dialogue`). When the user switches tabs
(left/right shoulder buttons, tab keys, etc.):

- **Announce the tab name** you moved to, nothing else.
- Examples: `Video options`, `Audio`, `Dialogue`.

After announcing the tab, normal vertical navigation reads the focused item inside it as usual. Use
`interrupt=true` on tab change (the context changed).

## Vertical navigation (up / down)

When moving between menu items (arrow up / arrow down), announce **in this order**:

1. The item's **name**.
2. The item's **current state / value** (if any).
3. The **tooltip / description** (if any).

Example: `Language, English, changes the game language`.

## Horizontal navigation and controls (left / right, dropdowns)

For **sliders, checkboxes, toggles and dropdowns**, when changing their value with left/right or when navigating inside an open dropdown:

- **Read ONLY the new value**, nothing else.
- Do **NOT** repeat the label name or the full phrase.

**Correct** (user on "Language: English", presses right):
```
Spanish
```

**Incorrect**:
```
Language: Spanish
```

Rules per control type:

- **Slider** → read only the new value (`75%`, `8`, `High`).
- **Checkbox / Toggle** → read only the new state (`On` / `Off`, `Enabled` / `Disabled`).
- **Dropdown** → on opening or navigating options, read only the value you're on (`Spanish`, `English`, `Japanese`).

## Key distinction

- **Entering an item** (vertical navigation) → name + state + tooltip (full context, because focus changed).
- **Changing an item's value** (horizontal navigation / dropdown) → only the changed value (because the user already knows the context).

This avoids redundant verbosity and keeps reading snappy.

## Popup and confirmation dialogs

When a **popup dialog** or a **confirmation dialog** appears (e.g. `Do you want to exit the game?`,
`Apply changes?`), treat it as a context change and accessibilize it:

- On open, announce the dialog's **message / question** (its title and body text, in the order the game
  shows them) with **`interrupt=true`**.
- Then read the **focused button / option** (e.g. `Yes`, `No`, `Cancel`, `Apply`).
- When the user moves between the dialog's buttons/options, read **only the focused option** (same rule
  as horizontal navigation).
- On close, return focus context to the underlying menu (re-announce the menu name only if the focus
  actually returns there and the user needs the context).

Example (exit confirmation opens, "Yes" focused): `Do you want to exit the game? Yes`. User presses
right: `No`.
