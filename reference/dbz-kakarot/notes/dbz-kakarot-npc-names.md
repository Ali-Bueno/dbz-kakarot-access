# dbz-kakarot-npc-names

> DBZ Kakarot NPC/enemy display names — no safe auto-resolver exists; MessageManager name calls CRASH; use hand-verified CplNNN map only


Resolving a DBZ Kakarot field character's internal code → localized display name for the
radar picker ([dbz-kakarot-radar-picker](dbz-kakarot-radar-picker.md)). **Exhaustively searched 2026-07-10 (live probe
+ Ghidra decompile + all object dumps): there is NO crash-safe automatic resolver.** Do not
re-investigate the live-call paths — they crash.

**Key spaces (proven):**
- Field talkable NPC (`QuestCharacter`/`QuestCharacterBase_C`): reflected **`UniqueId`** (FName)
  = raw `CplNNN`/`NpcNNN`, no suffix (Cpl003, Npc019). ONLY safe field on this class —
  `CharacterName` is NOT declared here, reading it is the uncatchable abort.
- Field enemy (`AT_Character_cplNNN_BP_C`): reflected **`CharacterName`** (FString @0x9E8)
  = `CplNNNcNN` with variation suffix (Cpl059c02). Safe ONLY on AT_Character actors.
- Soul-emblem table keys on `CplNNN{A..G}` (Cpl021A) — a DIFFERENT, emblem-specific space,
  not the field code.

**Dead ends (do NOT retry):**
- `UAT_BlueprintFunctionLibrary::GetCharacterName(code)` → returns "" for EVERY code
  (Cpl001/002/011 incl.), any format (upper/lower/`c00`/`c01`/`c02`). Safe but useless.
- `BP_ATGameInstance_C.MessageManager:GetNounParamFromName(code)` / `GetNounParam(code)`
  **CRASHED the game** — `EXCEPTION_ACCESS_VIOLATION` deref 0x70 (2026-07-10). This reflected
  family (struct return / FName-param marshalling) is the uncatchable-abort family on this
  build, like the LineTrace/ProjectPoint out-param calls. NEVER call them.
- No `CharacterId`/`NameId`/`DisplayName`/`CharaName` property confirmed on any character
  class; no static CplNNN→name table embedded in the decompiled binary to transcribe.
- Auto-learning from dialogue (`Field_Talk_Win_C.Txt_Speaker`) is unreliable: the on-screen
  speaker changes per line (Goku may speak first), so it can't be trusted to label the
  targeted NPC without guessing.

**Only correct path = hand-verified map** (`CPL_NAMES`/`NPC_NAMES` in `nav_tracker.lua`),
grown from data the USER confirms (talk to the NPC, hear the real name, map that code).
VERIFIED so far: Cpl001 Goku, Cpl002 Gohan, Cpl009 Yajirobe, Cpl011 Piccolo. Unknown story
NPCs near the Saiyan-saga arrival: Cpl003, Cpl006, Cpl007, Cpl008, Cpl013 (user had Vegeta/
Nappa/Krillin + Piccolo nearby — 5 ids, can't assign without confirmation → never guess).
Diagnostic to capture ids safely: `NPC_ID_DUMP` flag (reads UniqueId only) → dumps/dump_npc_ids.txt.

Experimental/unproven (needs a careful in-game test, flag as risky): reading
`MessageManager.DataTable` ROWS directly via DataTable reflection (`GetRowNames`/`FindRow`) —
a different API from the crashing `GetNounParam`; row-key format unknown, and messageData is
keyed by message id (dialogue), not character code, so low expected value.
