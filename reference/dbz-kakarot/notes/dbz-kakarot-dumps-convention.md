# dbz-kakarot-dumps-convention

> DBZ Kakarot mod — discovery dumps naming + location convention


Discovery dumps for the DBZ Kakarot accessibility mod go to `mod/KakarotAccess/Scripts/dumps/`
as `dump_<time>_<NNN>.txt`. Always call them **dumps** — one consistent name, do NOT rename them
"probes" or switch terminology mid-task (the user was annoyed by that).

**Why:** dumps live WITH the source. Only the mod itself runs from the game folder, via the
`Scripts` junction — never copy dumps into the game folder by hand.

**How to apply:** keep `discover.lua`'s OUT_DIR pointing at the repo `dumps/`; keep the `dump_`
prefix; be terminologically consistent. See [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md).

**Screenshots:** the user's game screenshots always live in `C:\Users\ali-b\Pictures\Screenshots`
(`Captura de pantalla (N).png`). Match the newest ones (by mtime) to the current dumps.
