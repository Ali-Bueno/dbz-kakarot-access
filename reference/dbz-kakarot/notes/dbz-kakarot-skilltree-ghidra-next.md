# dbz-kakarot-skilltree-ghidra-next

> Skill-tree lock SOLVED via Ghidra (per-node state byte on the zorb widget) and VERIFIED in-game 2026-07-14


2026-07-14: the skill-tree lock is **solved and verified in-game**, far more cheaply than planned. No
`USkillSave` walk is needed — **the game caches its own tri-state on each node widget**:
`state = u8[zorb + 0x460]` → `{0,1}` locked, `{2,5,8}` acquired, `{3,4,6,7,9,10}` purchasable. Reached from
the tree: cursor `UAT_UISkillTree+0x15F8/+0x15FC` (RAW grid col/row) → `cell = i32[+0x4CC + (row*30+col)*4]`
(1-based, ≤0 = empty) → `zorb = ptr[+0x438][cell-1]`. Offsets + their proving `FUN_` addresses are in the
mod's `STATUS.md` *Derived facts*; do not re-RE them. The feature is CLOSED: the "requiere nivel N" reason was
deliberately DROPPED (user, 2026-07-14) — pressing A on a locked node makes the game state the requirement
itself, in the message window the dialog reader already speaks, so the tri-state is all the reader needs.
Its RE (`i32[row+0x58]` via the DataTable `RowMap`) stays recorded in STATUS only in case that changes.

**Reusable pattern:** when reflection can't see a computed state, look for where the GAME caches its own
answer (a state byte on the widget it drives) before reconstructing the computation from the save data.
The proof it's the right byte: find the input handler that GATES on it (`OnInputDecide` refused to act on
exactly `{2,5,8}`).

**Why it matters beyond this mod:** the source of the RE-tool rule now in the project `CLAUDE.md` (§2) —
dumps say *what exists*, Ghidra says *what the game does*, runtime diff *pins an address*. A class that
dumps EMPTY (`USkillSave`) or exposes zero reflected functions is telling you on day one that reflection
will never answer; we spent seven in-game capture rounds learning that. Stop rule: two capture rounds with
no signal → go native. Related: [dbz-kakarot-decompilation-setup](dbz-kakarot-decompilation-setup.md), [dbz-kakarot-event-driven-ui-cache](dbz-kakarot-event-driven-ui-cache.md),
[dbz-kakarot-skillpalette-done](dbz-kakarot-skillpalette-done.md).
