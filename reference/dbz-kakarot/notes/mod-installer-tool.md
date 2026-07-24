# mod-installer-tool

> Reusable config-driven mod installer (Rust/NWG) at D:\code\tools\mod-installer; GitHub-release installer for the accessibility mods


Reusable **installer** for the accessibility mods, modeled on the "Blindfold Installer"
(Balatro). Lives OUTSIDE the mod repos at `D:\code\tools\mod-installer`; its own
**PRIVATE** git repo `Ali-Bueno/mod-installer`. The Kakarot mod repo
`Ali-Bueno/dbz-kakarot-access` is now **PUBLIC** (user's call 2026-07-17) so the installer
can read its releases unauthenticated — a private repo returns 404 to the unauthenticated
releases API, which silently emptied the release list.

- **Stack:** Rust + `native-windows-gui` (native Win32 controls → screen-reader
  accessible via MSAA/UIA, exact classic look, native Sí/No `MessageBox`). Single
  ~1.8 MB native exe. Chosen over Python/PyInstaller because PyInstaller exes get
  **antivirus-flagged** (user's call, 2026-07-17), and over egui/tkinter for a11y.
- **Config-driven:** one crate, `configs/<mod>.toml` per mod, selected at build time
  via `MOD_CONFIG` env (default `kakarot`); `build.rs` embeds it + the Common-Controls-v6
  DPI manifest. `cargo build --release` → `target/release/installer.exe`.
- **Modules:** `config.rs` (serde toml), `steam.rs` (registry→libraryfolders→appmanifest
  autodetect), `github.rs` (ureq releases API + download), `installer.rs` (extract overlay,
  merge mods.txt without clobbering other mods, preserve settings across update, version.txt),
  `app.rs`+`actions.rs` (UI: status line, dir+Browse, log box, Install / Install from file /
  Uninstall(3 dialogs: mod→injector→settings) / Open Mods folder; manual GetClientRect dock layout).
- **Release model:** the asset is an **overlay zip rooted at the game dir** (paths relative
  to `<game_dir>`). For Kakarot: `AT/Binaries/Win64/…` = UE4SS (dwmapi.dll, UE4SS.dll,
  UE4SS-settings.ini + built-in Mods) + `Mods/KakarotAccess/`. Built by **`package.ps1`** in
  the Kakarot repo (`-Version vX.Y.Z`, auto-detects game, excludes dev dumps/DBZK_Fix,
  writes version.txt + clean mods.txt). Does NOT publish; prints the `gh release create` line.
- **Kakarot config facts:** repo `Ali-Bueno/dbz-kakarot-access`, Steam appid **851850**,
  folder `DRAGON BALL Z KAKAROT`, marker `AT/Binaries/Win64/AT-Win64-Shipping.exe`, injector
  = UE4SS (proxy `dwmapi.dll`), asset glob **`KakarotAccess*.zip`** (widened when the zip lost its
  version suffix, commit 4cfae2c), dev_build disabled (ships prebuilt DLLs, can't run from source).

**EVERY RELEASE MUST CARRY TWO ASSETS — the zip AND `<Mod>Installer.exe`** (rule burned in
2026-07-21). README.md tells users to download `KakarotAccessInstaller.exe`, and both it and the
in-app updater resolve `/releases/latest/download/…`, which points at whatever release is NEWEST.
Publishing v0.1.1 with only the zip therefore **404'd the installer download for everyone** even
though v0.1.0 still had it — "latest" moved, the asset didn't. The private-repo fact is a red
herring for this symptom (users never fetch the mod-installer repo; it only 404s if you paste a
link to it). Build per mod with `MOD_CONFIG=<mod> cargo build --release`, rename
`target/release/installer.exe` → `<Mod>Installer.exe`, `gh release upload`. The step is now printed
by `package.ps1` at the end of every packaging run. Decision 2026-07-21 (user, for ALL mods): the
installer source stays PRIVATE and the exe ships as an asset of each mod's public release.
- **Status 2026-07-17:** SHIPPED. Current release **v0.1.0** (earlier v1.0.0/v0.9.0 deleted — user
  reset to 0.1) on the public Kakarot repo, assets `KakarotAccessInstaller.exe` +
  `KakarotAccess-v0.1.0.zip`. Direct links: installer = `/releases/latest/download/KakarotAccessInstaller.exe`,
  latest = `/releases/latest`. Forum post (EN+ES) at `C:\Users\ali-b\Desktop\KakarotAccess - post.txt`.
- **Main-button semantics (2026-07-21, user request):** the label IS the update notification for a
  screen-reader user, decided by `verb_for(installed, latest)` in `actions.rs` (single source of
  truth, shared with the `pick_version` dialog title): different tags → **"Update"**, same tag or no
  release info but something installed → **"Reinstall"**, nothing installed / unreadable version →
  "Install". Never "Update" on a failed online check. The button is **no longer disabled when up to
  date** — that was exactly the state a user is in when a release breaks and they need to roll BACK,
  and the version picker behind the button already lists every tag. Rollback is clean:
  `installer.rs install_zip` snapshots settings, REMOVES the mod folder ("so stale files don't
  linger"), extracts, restores. The in-app version picker DOES work now (commits 3e65131 + a44c475,
  keyboard-accessible) — the older "not yet done" note below is stale.
  Debug keybinds (F4/F6/F7/F9/F10/Ctrl+F5/Ctrl+Shift+F5) gated behind `build_flags.debug`; package.ps1
  sets debug=false + drops discover/dev_memdiff/dev_log in releases. README now says controller REQUIRED,
  lists pending: Encyclopedia Z, baseball, Story menu, talking to party in towns. Version picker has
  Tab/Esc/Enter + focus-on-open. Online Install works;
  Uninstall detects the mod by folder presence (not just version.txt — dev junction has none) and
  its "return to fully vanilla" step removes UE4SS **and the whole Mods folder**. Discord/Patreon
  buttons wired from `[links]`. Mod README rewritten + pushed. **Not yet done:** in-app version
  picker (nwg modal loop conflicts with the main dispatch loop — Install=latest for now, older via
  Install-from-file). DEV-MACHINE CAVEAT: Install/Uninstall act on the junctioned Mods\KakarotAccess,
  so they break the hot-reload setup (recreate the junction after). See [dbz-kakarot-engine-facts](dbz-kakarot-engine-facts.md)
  [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md).
