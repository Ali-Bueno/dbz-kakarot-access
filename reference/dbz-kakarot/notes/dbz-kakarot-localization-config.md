# dbz-kakarot-localization-config

> Pre-release batch — external-TXT localization (13 langs), L3+R3 config menu, settings.lua, double-R3 explore-toggle fix


Pre-first-release batch for the DBZ Kakarot mod (2026-07-17, VERIFIED in-game by the user + committed & pushed to main; release NOT cut yet per the user). Four user-requested items:

1. **Localization → editable external TXT, all game languages.** `i18n.lua` gained an overlay loader: `Scripts/lang/<code>.txt` (format `key = value`, `#` comments, dotted prefixes `buttons.`/`keyhelp.`/`header.N`/`startlist.N`, `\n` newline, `\s` edge-space for combo_join/controller_prefix). Resolution: `ext[lang] → S[lang] → S.en → key`. External file WINS over the embedded table (so shipped `lang/es.txt`/`en.txt` are the editable source of truth; embedded es/en remain the fallback if TXT deleted). 13 base languages = the game's own `ELanguageType` enum (CFramework_enums.hpp): ja en fr de es it ko pl pt ru zh ar th (es_ES/es_MX→es, zh_CN/zh_TW→zh). es/en exported from the tables; other 11 machine-translated (sonnet) for the community to refine. 258 keys each. Language override via `I18n.force_language(code)` (config), re-applied by `app.start()` on every reload.

2. **Config menu, opened with L3+R3** in the overworld. New `config_menu.lua` (modal, PadPoll, screen-reader driven like radar_menu) + `settings.lua` (persists `Scripts/config.txt`, gitignored, publishes `_G.__KakarotSettings`, loaded in main.lua before the protected snapshot). Options: audio cues on/off, cue volume 0-100 %, radar auto-activate on/off, language. Wiring: `audio.lua` gates every cue on `Settings.cues_enabled()` + scales volume by `cue_gain()` (one chokepoint); `nav_tracker` auto-scan gate + `notify_objective_change` read `_G.__KakarotSettings.autotrack_enabled()` (global, NOT a new local — nav_tracker is at the 200-local cap).

3. **Double-R3 "explore other objects" toggle now turns OFF.** Bug was in `radar_menu.lua`: a double slower than the 280 ms window let the single-tap fallback open the picker on tap 1 and tap 2 only cancelled it, so `Nav.toggle_explore` never fired. Fix = wider window (~400 ms) + a slow-double RESCUE (tap 2 within ~680 ms of tap 1, no picker nav in between → close+toggle).

4. **L3+R3 must not open the radar picker** — radar_menu ignores R3 while L3 held; both overlays share the `_G.__KakarotPadModal` mutex ("radar"/"config", only one owns a blocking overlay at a time).

First run needs a FULL game restart (new files + main.lua changes); Ctrl+Shift+R for later tweaks. Related: [dbz-kakarot-radar-picker](dbz-kakarot-radar-picker.md), [dbz-kakarot-perf-architecture](dbz-kakarot-perf-architecture.md), [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md).
