# dbz-kakarot-chinese-speech

> Chinese game language produced corrupted speech. The cause was **not** in this mod: a UTF-8
> bug in the simdutf copy bundled inside PRISM v0.16.7, reached only on AVX-512 CPUs.
> Reported 2026-09-20, diagnosed and fixed 2026-09-21.

## Symptom and the measurement that cracked it

A player on an ASUS ROG Ally (Ryzen Z1 Extreme = **Zen 4**), NVDA 2026.2, system code page CP936,
heard corrupted mixed-script characters whenever the game language was Simplified Chinese. English
was fine. The report included a `DEBUG_LOG` capture (`speech.lua`) proving the Lua string handed to
`prism.say` was already valid Chinese — which split the problem in half and pointed below the mod.

Byte comparison of what was sent against what was spoken:

| sent | UTF-8 | heard | rule |
|---|---|---|---|
| 商店 | `E5 95 86` `E5 BA 97` | 商 U+0E97 | 2nd char: `E5` -> `E0` |
| 始 (4th char) | `E5 A7 8B` | U+09CB | same |

Characters at ODD positions survive; the rest lose the **top 4 bits** of the codepoint (`cp & 0x0FFF`).
With pure hanzi, character *n* starts at byte 3n, so "even character" and "even byte offset" are the
same thing here — the offsets do not separate the two readings. Decoding as CP936 was tested and
ruled out (it yields U+935F U+55D7 U+7C35, nothing like the result).

## Cause

PRISM v0.16.7 bundles simdutf, whose **icelake (AVX-512)** UTF-8 -> UTF-16 kernel builds lane masks
from a *signed* `1 << n` where `n` can reach 32 — undefined behaviour. MSVC emits a wrong mask and
the transcoder drops the lead byte's payload nibble. simdutf is dispatched by CPUID, so only CPUs
that select the AVX-512 kernel (Zen 4/5, some Intel) are affected, which is why it looked random
across the player base.

Upstream: [ethindp/prism#96](https://github.com/ethindp/prism/issues/96) (same signature,
`游` U+6E38 -> U+0E38). Fixed by simdutf PR #1004, vendored into PRISM v0.18.0.

**Dead end, do not chase:** SAPI appearing to work is not a code difference — `sapi.cpp` makes the
identical simdutf call as `nvda.cpp`. It is the same UB landing differently at two call sites. The
decisive control is CPU dispatch (`SIMDUTF_FORCE_IMPLEMENTATION=haswell` fixes every backend), not
the backend.

## What changed here

- `libs/prism/` bumped **v0.16.7 -> v0.18.2**. Verified: PE dates and export counts identify both
  builds (42 vs 59 exports), the buggy shift is present in the v0.16.7/v0.17.3 amalgamation and
  absent in v0.18.0+, and **no export was removed**.
- **`PrismConfig` grew from 1 byte to ~48** between those versions, and `prism_config_init` returns
  it BY VALUE — a >8-byte struct returns via hidden pointer on the x64 MSVC ABI, so a bridge built
  against the old header would have used the wrong convention. **Bumping the DLL without rebuilding
  `prism_bridge.dll` is memory corruption, not a version mismatch warning.** Rebuilt and smoke-tested
  (`is_ready` true, backend acquired).
- `prism_bridge.c` now returns `ok, PrismError` from `say`/`output`/`braille` instead of a bare
  boolean, and `speech.lua` logs each distinct failure code once. PRISM refuses a whole utterance on
  `PRISM_ERROR_INVALID_UTF8` (13) and speaks nothing; that was invisible in every layer. Verified
  live: a lone `0xE5` returns `false, 13`, valid text returns `true, 0`.
- `Core.clip` (`ui_core.lua`) replaces every byte-truncation of GAME TEXT in a log path. A cut inside
  a multi-byte character makes the string invalid UTF-8, and UE4SS's `print` transcodes to UTF-16 and
  RAISES on it. `screen_dialogue.lua`'s `trace_bubble` was the only such site with no debug flag, so
  it ran in release: `t:sub(1, 70)` on hanzi is a deterministic break (70 = 3x23+1), aborting
  `Dialogue.is_active` up to 20 times per session.
- `package.ps1` refuses to package an INCOHERENT PRISM runtime: `Scripts/prism.dll` must hash-match
  `libs/prism/bin/prism.dll`, and `prism_bridge.dll` must be newer than both `prism_bridge.c` and
  `prism.h`. The DLLs are gitignored, so a release carries whatever sits in that folder on the
  packaging machine, and both failures ship silently. It fails OPEN when `libs/prism/bin` is absent
  (a fresh clone has nothing to compare against) and CLOSED only on a real mismatch.
- `A.markup_to_speech` now normalises **U+3000** (ideographic space) and U+00A0 before trimming —
  Lua's `%s` is ASCII-only, so a padding-only widget used to survive the trim as non-empty text.

## Duration model (fixed in the same batch)

`speak_seconds` estimated speech time from **byte** length, which feeds the queue expiry, the
requeue windows and `protect_until`. It now counts CHARACTERS, weighting ideographs by 3.

The trap worth remembering: the byte model was **accidentally correct for CJK**. A hanzi is 3 UTF-8
bytes, so `bytes / 14` gave it ~4.7 characters/s — near the real rate of spoken Mandarin. Switching
to a plain character count would have fixed Russian by breaking Chinese. Weighting ideographs by the
3 that `#text` already charged them keeps every CJK estimate **bit-identical** while Cyrillic, Thai
and Arabic stop being inflated 2-3x.

Byte width alone cannot make this decision: **Thai is 3 bytes per letter and spoken at letter
speed**. The classification is by Unicode block (Kana, CJK Ext A, CJK Unified, Hangul Syllables).
`utf8.codes` raises on malformed input, so it is pcall-guarded and falls back to the byte count --
this is the speech path. Covered by `tools/tests/test_speech_duration.lua`, whose first assertion is
that the ideographic estimates did not move.
