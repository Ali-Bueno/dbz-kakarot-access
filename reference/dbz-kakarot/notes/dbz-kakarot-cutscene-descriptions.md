# Cutscene descriptions: opening through Raditz

## Coverage and runtime

The development catalog contains 60 English cues across 15 identified MOV/GDM
sources, ending at C01_020_S060 (the immediate post-Raditz scene). One narrated
recap has an explicitly empty cue list. Six real-time GDM scenes have introductory
lines only; this is not full visual coverage of those scenes.

Playback uses the reflected Mana or master LevelSequence clock, not elapsed wall
time. Seek, skip, replay, settings changes and world transitions are handled by
the cue engine/adapter. Visible dialogue/subtitles take priority; descriptions
are non-interrupting and disposable rather than requeued after an interruption.
Music and sound effects, including important impacts, may accompany narration.
An absent active-clock sample retains plain cue/time history: pause or an unreadable
poll must not repeat a consumed cue. Positive transition/free-roam gates, new sources
and actual rewinds still reset it. No actor/player handles are retained for this.

The catalog text is the English source; every cue carries a derived key
(`ad_<scene>_<centiseconds>`, set at load) and lang/<code>.txt carries the same line for the
other 12 languages (2026-09-08; the adapter falls back to the catalog text when a key is
absent, and test_audio_description_cues.lua fails if any language lacks a cue). The
translations were produced from the English cues, not from the frames, so they inherit
the English timing; per-language speech-duration fit is NOT checked (the live dialogue gate
still wins). Names follow each language's DBZ Kakarot / anime localization; flagged for a
native check: de Radditz / Jindujun / Höllenspirale (game text may say Raditz / Kinto Un /
Makankosappo), ru Радиц / Макканкосаппо / Каме Хаус (no official Russian game text to
check against), ar attack/scouter calques, zh 龟屋 (vs 龟仙屋), ko 내퍼 (vs 납파), th names
(fan convention; the game ships no Thai), it Raditz / Piccolo (dub: Radish / Junior), fr
Chichi unhyphenated. **Spanish is VERIFIED against the game's own tables** (2026-09-08,
`AT/Content/Message/PLAT_W/{es_ES,es_MX}/messageData.uexp` via `repak get`, string counts):
es_ES prints Chi-chi, Krilin, Piccolo, Nube voladora, Bola Dragón de cuatro estrellas,
Cañón de Haz Especial, rastreador; es_MX prints Milk, Krillin, Píkoro, nube voladora, Esfera del
Dragón, Makankosappo, Rastreador. The game ships them as SEPARATE languages, so the mod gained
regional-variant overlays (`I18n.VARIANTS`, `lang/es_mx.txt` holds only the differing lines).
**Italian verified the same way** (it_IT table): Radish (100) not Raditz (0), Crilin, Piccolo, Freezer, Cell, Re Kaio, maestro Muten (not Genio delle Tartarughe), West City (not Città dell'Ovest), Kienzan, scimmione, Capsule Corporation, Super Saiyan; the Nimbus name did not resolve (neither Nuvola Speedy nor Nuvola d'oro occur — the table has a `Nuvola d…` form worth a closer look), so it.txt keeps Nuvola Speedy for now.
The config menu and Scripts/config.txt expose the
audio_descriptions switch. No network service, API key, generated narration audio,
modified movie or new native dependency is required at runtime.

## How to describe ANOTHER cutscene (the repeatable method, 2026-09-08)

Everything needed is local; no service is required. What the PR author used, reduced to steps:

1. **Find the movie.** Pre-rendered scenes are CriWare `.usm` files in
   `…\DRAGON BALL Z KAKAROT\AT\Content\Movies\` (236 of them; NOT in the paks). The catalog
   key is the file name upper-cased without `_mov`: `C00_000_S000_mov.usm` → `C00_000_S000_MOV`.
   The mod tells you which one is playing: with descriptions on, an undescribed scene logs
   `[KakarotAccess] audio description: no cues for <KEY>` — grep the log after a session and
   you have the exact list of what the player saw and heard nothing for.
2. **Look at it.** The installed FFmpeg (8.x) reads USM directly (mpeg1video + adpcm_adx):
   `ffmpeg -ss <start> -t <len> -i <file>.usm -vf "fps=1,scale=400:-1,tile=4x6:padding=4:margin=4" -frames:v 1 sheet.png`
   gives a contact sheet, one frame per second, read left-to-right per row (frame i is at
   start + i s). Two passes: fps=1/2 for the whole movie to see the scenes, then fps=1 over the
   fast-cut stretches. The assistant can view the PNG and describe it. (`drawtext` timestamps
   need fontconfig, which the winget FFmpeg lacks — count tiles instead.)
3. **Find the dialogue-free gaps.** Story movies carry spoken lines; a description must not
   overlap one. `silencedetect` only finds true silence (useless under music); the PR author
   analysed the two mono voice tracks of the USM's audio to get the gaps recorded as `before`.
   For a pure music montage (the title intro) there are no gaps to respect.
4. **Write compact cues** into `audio_description_cues.lua`: present tense, ≤ 7 words, `at` on
   the file's own clock (the Mana clock the adapter reads is the same timeline), starts ≥ ~1.3 s
   apart, `before` = the next dialogue boundary when there is one. `test_audio_description_cues.lua`
   checks the English speech estimate against `before`.
5. **Translate**: generate the `key = English` list, get the 12 languages (+ the `es_mx`
   overlay for lines whose names differ), append under a per-scene header at the end of each
   `lang/<code>.txt`, and run the tests — they fail if any language lacks a cue.
6. **Verify Spanish names against the game's tables** (`repak get … Message/PLAT_W/<code>/messageData.uexp`,
   then count candidate spellings); the same trick works for every language the game ships.

Real-time GDM scenes are not movies: there is no file to look at, only in-game frames, which
is why the six GDM entries are one-liners.

### Title-screen opening — C00_000_S000_MOV (2026-09-08)

41 cues over 1:49, authored from FFmpeg sheets (1 frame/s over every cut). Music montage, no
dialogue; the log had already named the source at the title screen (`no cues for
C00_000_S000_MOV`), so no adapter change was needed. Uncertain identifications, kept generic
on purpose: the blond fighter lunging at ~80 s (not named), the figure blasted at 65 s
(read as young Gohan), the golden burst at 66–67 s (read as Vegeta from the blue suit).

## Authoring and visual review — September 5, 2026

MOV timestamps were checked against the original USM footage and both mono voice
tracks. The conservative speech estimate is 0.5 + character_count/14 seconds;
it is an estimate, not a measurement of a player's configured screen reader.

ViddyScribe (https://viddyscribe.com) provided draft text for selected scenes.
Source material is attributed in the cue file and mod README. Drafts were reviewed,
shortened and retimed locally, not bulk-imported. Original movies, contact sheets,
service job identifiers and account-specific exports are not part of this repository.

The initial approximately 35-second Dragon Balls/Nimbus scene test yielded three
candidates which failed visual or timing review: a dark frame described as glowing
orbs, a too-short Shenron interval, and a Goku cue over a frame still showing Shenron.
None was integrated. The verified common voice gap is 32.185–34.900 seconds;
frame review at 33.3 seconds supports the retained cue "Goku and Gohan smile."
at 32.30 seconds.

For C01_010_S080_MOV (Raditz's arrival), the generated draft improved action
coverage but sometimes described later frames early. A crash cue preceded the
impact, a hatch cue preceded the hatch view, and a scouter-beep description
merely repeated an audible sound. Original-timeline frames were reviewed at
0.2, 6.0, 8.8, 20.0, 24.5, 31.8, 36.0, 41–45, 57.35, 71.0, 74.1, 77.3, 93.6,
98.2 and 104.0 seconds, with nearby frames checked for context.

The revised arrival scene retains 16 cues but uses 77 words / 424 characters
instead of 94 words / 519 characters, with at most seven words per cue.
Corrections include the farmer bending beside his truck, the warrior floating
out and landing, and the farmer still falling at 77.3 seconds. Piccolo and
Raditz-landing descriptions were moved off the preceding canyon views.

## Verification and limits

Three offline suites cover the clock adapter, cue state machine and catalog.
Arrival timing tests use independently recorded common voice gaps and check
cue.at + 0.5 + character_count/14 + 0.35 against the next dialogue boundary
and the next description. The extra 0.35 seconds covers late-start allowance.
Three older cues failed before correction; the revised cues pass.
Adapter regressions also cover paused movie/master-sequence clocks and temporary
source/time/actor-list gaps; these repeated a cue before the history-preservation fix.

The cue's before field is QA metadata, not a runtime speech cutoff. Some essential
battle beats have no before field and rely on the live dialogue reader. The
six introductory-only GDM entries were not expanded during compact refinement.

In-game testing must still confirm source detection, speech queue delay, actual
voice duration, dialogue separation and clarity over effects. Test normal playback,
skipping, replaying, toggling descriptions, and leaving/re-entering a scene.
Do not describe offline timing checks as proof of native audibility.
