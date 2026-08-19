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

---

## 2026-08-15 — re-asked with the MCP and the offline pak index

The 2026-07-10 conclusion **still stands** (no crash-safe live resolver), but two tools that did
not exist then — the live Inspector over MCP, and the offline pak index — settled three of its
open questions and opened one genuinely new route. Nothing below required a live call on the
crashing family; the DataTable/messageData work never touched the running game at all.

**NEW: two fields on `AT_CharacterBase` the July pass never saw** (from a live `kak_class` dump):
- **`speakerID`** (StrProperty) — the dialogue speaker key. Content NOT yet read: the Inspector's
  `watch` printed the FString HANDLE, not its text, because it deliberately excluded
  Str/Name/Text properties. Fixed (see below); needs a game restart to load.
- **`m_param_id`** (EnumProperty) — a numeric parameter-table id. Read **0** on the first live
  `QuestCharacter`, so it is either unset for talkable NPCs or was read on the wrong instance.
  Worth re-reading on several real characters before drawing a conclusion — see why it matters
  under *the roster* below.
- Also present: `fn GetCharacterCode_BPCharaCode()`. NOT called — reflected calls are the
  crashing family on this build.

**DEAD, and now for a concrete reason: `DT_CharacterDataTable`.** Extracted offline from
`pakchunk0` and inspected. Its row struct `ATCharacterDataTable` declares **no name, label or
message-key column at all** — only `RacialType`, `Height`, `Level`, `MotCode`, `SaveIndex`,
`SortIndex`, `UniqueID`, `Width`, the `GuestCharacter`/`PlayerCharacter`/`SupportCharacter`
flags and the `Summon*_01..08` families. ~164 `Cpl###`-keyed rows, no literal names anywhere.
`ZCW_DT_UIDataModelChara` is an empty table (zero declared properties, 49-byte `.uexp`).
Do not re-mine either.

**NEW ROUTE — the name roster inside `messageData`.** `AT/Content/Message/PLAT_W/<lang>/
messageData` (`.uasset` 1.9 MB + `.uexp` 17.2 MB) contains **two structurally different
sub-tables**:
1. A **dialogue stream** of flat adjacent triples `[numeric id][speaker code][line]`, where the
   speaker code is exactly the `CplNNN<letter>` / `NpcNNN<letter>` space (200 bare `^Cpl\d{3}[A-Z]$`
   FNames; 11,100 keys containing `Cpl`, 10,595 containing `Npc`). Recoverable from a strings pass
   with 3-line grouping — cheap. Gives code → *lines*, NOT code → *name*.
2. A **character-name roster**: the bare display names really are in there — `Goku`, `Gohan (Kid)`,
   `Gohan (Teen)`, `Gohan (Adult)`, `Vegeta`, `Appule`, `Frieza Force Grunt`, `Frieza`, `Krillin`,
   `Yamcha`, `Yajirobe`, `Piccolo`, `Nappa`, `Zarbon` … in one flat sequential block. **No string
   key sits adjacent to them**: each is a length-prefixed FString preceded by an 8-byte binary
   field, i.e. a NUMERIC row id. So this block is *not* recoverable by a strings pass; it needs a
   real property/array parser (est. 1–2 days incl. verifying the id scheme).
   The reason `m_param_id` matters: it is a numeric id on the actor, and this roster is keyed by a
   numeric id. If those are the same space, actor → name is solved. **Unverified — check
   `m_param_id` on several real characters before spending a day on the parser.**
   Caveat if you do build it: this is per-language data, so a roster mined from `en_US` names
   characters in English only; the mod ships 13 languages.

**CHEAPER ALTERNATIVE that needs no parser — the nameplate, with a uniqueness rule.** The game
draws the character's own **localized** name over them: `Info_Name_Parent_C` →
`Ins_Info00`/`Ins_Info01` → `Txt_Name`/`Txt_Name01..03` (native twins on `AT_UIInfoNameCore`:
`NameTxt`, `NameTxt_Large`, `PopularNameTxt`, `PopularNameTxt_Large`). One live host, **two
slots**, and **no pointer back to the actor** — both classes declare zero own functions, the text
is set from native C++. So pairing name→code needs the playbook's uniqueness rule: bind only when
exactly ONE nameable character is in range. Being in the player's own language, this beats an
`en_US` roster for a 13-language mod, and it grows the hand-verified map automatically from real
play instead of from guesses.

### THE ANSWER (same day, later): `AAT_CharacterBase::CharacterType`

**Everything above is superseded as the PRIMARY route.** The name is a plain reflected ENUM on the
character base class, and its enum's value names are the characters themselves.

```
AAT_CharacterBase :: CharacterType : CHARACTER_TYPE @0x0758      <-- read this
AAT_CharacterBase :: m_param_id    : PARAM_ID       @0x075C
AAT_CharacterBase :: speakerID     : FString        @0x0890
AAT_Character     :: CharacterName : FString        @0x09E8
```

`enum class CHARACTER_TYPE` (CXXHeaderDump/`AT_enums.hpp`:98) has **277 values**. Values **1–119 are
human-readable character names** — `Goku=1`, `Gohan=4`, `Vegeta=8`, `Apuru=11`, `Freezer=18`,
`Kuririn=21`, `Yamcha=22`, `Tien=23`, `Chiaotzu=24`, `Piccolo=25`, `Raditz=26`, `Nappa=27`,
`Saibamen=28`, `Cui_C01=33`, `Dodoria_C01=34`, `Zarbon_A_C01=35`, `Recoom=37`, `Burter_C01=38`,
`Jeice_C01=39`, `Guldo=40`, `Ginyu=41`, `Trunks=42`, `Android20=45`, `Android17=46`, `Android18=47`,
`Cell_A=48`, `CellJr_C01=51`, `Goten=52`, `Dabura=53`, `PuiPui_C01=54`, `Yakon_C01=55`, `Buu_A=56`,
`Gotenks=60`, `Vegito=61`, `Mira=85`, `Bonyu=86`, `Beerus=93`, `Whis=94`, `Shin=119`, … Values
**120–276 are raw `CplNNN` codes** (later/DLC content) and carry no name.

Why this beats every other route considered:
- It is a **VALUE read on a base-class property**, so it works on BOTH branches (`AAT_Character`
  enemies and `AQuestCharacter` NPCs) — one code path, no per-class special-casing.
- Enum reads take no validity call and no pointer hop: the safest operation on this engine.
- No DataTable, no `messageData` parser, no icon/brush walk, no live actor-icon lookup.

Naming needs light normalisation: strip the systematic variant suffixes (`_A`/`_B`/`_C`/`_Z`/`_E`/
`_F`/`_G`, `_CNN`, `_L`) and map the romaji spellings to the display names already used elsewhere in
this mod — `Kuririn`→Krillin, `Apuru`→Appule, `Freezer`→Frieza, `Saibamen`→Saibaman,
`Recoom`→Recoome. `screen_community.lua`'s `CHAR_TOKENS` (62 entries) is the existing precedent for
the spellings, and its comment records the relevant decision: these are **proper nouns, so no
localization is needed**.

Scale of the win: `nav_tracker`'s `CPL_NAMES` has **4** verified entries and `game_character_name`
(the game's own `GetCharacterName`) returns "" for every id, so today the radar names 4 characters.
This enum names ~119.

**Dead ends confirmed by the same sweep** (do not re-investigate):
- The character actor stores **no icon or texture**. `AAT_Character::MapIconComponent` /
  `MobIconComponent` are typed-enum components only; `UATMapIconComponent` has just 6 members
  (`MapIconType`, `SearchRangeRadius`, `bShowMapIcon`, `bShowLandmarkIcon`, `LandmarkType`,
  `bIsHiddenTownToFieldIcon`) and cannot say WHICH character it belongs to. So the
  "reuse the community board's icon→token trick on a world actor" idea has nothing to read.
- No asset path pairs a `cplNNN` code with an icon token (`Charicon` files are
  `<Family>_<Tok><VV>_<NN>_<NN>`, e.g. `D_Gok00_00_00`, and a grep for paths containing both a
  `cpl\d{3}` and any known token returns **zero**). The icon families are `Charicon_D` (886 files),
  `_Ev`, `_F`, `_Cz`; 2,796 paths total. Anonymous NPCs use a numbered `A##` token family
  (`D_A0800_…`) with no name in it.
- Code→icon and code→name DO exist, but only as DataTable row types
  (`FCharacterIconInfo{CharaCode, Filename}`, `FCharacterIconCodeList{CharNameMsgId,
  CharIconCodeType:E_CHARACTER_NAME}`, `FCharacterName{CharacterName}`), i.e. asset lookups —
  irrelevant now that the enum answers directly. `E_CHARACTER_NAME` is a second readable roster
  (`CHARA_GOKU=0`, `CHARA_GOHAN=2`, …) but appears only in those rows, not on the actor.

**Tooling changed today** (both need a game restart to load — the Inspector loads once at start,
and the dev channel's poll loop keeps its command table across a hot reload):
- `tools/ue4ss-inspector`: `props`/`probe`/`watch` now render Str/Name/Text properties as text
  (guarded `:ToString()`, no validity call — they are value types). They were excluded before,
  which is why `speakerID` read as a handle.
- `dev_charnames.lua` + the dev-channel command `charnames`: walks live characters reading
  `UniqueId` / `speakerID` / `m_param_id` / `CharacterName` through the **strict** property gate
  (this is a multi-candidate probe, and this note is explicit that `CharacterName` on
  `QuestCharacter` is the uncatchable abort), plus the nameplate text, with incremental step
  markers.

## 2026-08-18 — the enum is authored on ONE branch only (playtest regression)

The `CharacterType` naming shipped on 2026-08-15 (commit `2e0b891`) was wired into BOTH namers.
The player's first playtest: *"me está detectando todo como goku — tengo a Dodoria a unos 300
metros y dice que es Goku"*. Measured live over the MCP, two reads settle it:

| Actor | Class | `CharacterType` | Authored? |
|---|---|---|---|
| field enemy, `/Game/Maps/Area11/Area11_P` (Namek) | `AT_Character_cpl004p1c02_BP_C` | 11 → `Apuru` | **yes** |
| `Itm098c01` — an ITEM, `UniqueId = None` | `QuestCharacterBase_C` | **1 → `Goku`** | **no — BP default** |

**The rule: the enum is only meaningful on a class that exists to BE one character.**
`QuestCharacterBase_C` is the GENERIC class the game reuses for talkable NPCs, items and quest
markers alike; nobody authors `CharacterType` on it, so every instance carries the Blueprint
default, and that default happens to be 1 = Goku. `AT_Character_cplNNN_BP_C` is per-character, so
its value is real.

**There is no way to tell an authored 1 from a defaulted 1 from the value alone**, so the
discrimination has to live at the CALL SITE. `npc_name` no longer reads the enum at all (back to
`UniqueId`); `enemy_display_name` keeps it — all three of its call sites are already behind an
`IsA(AT_Character)` gate (the enemy collector, the lock-on label, the companion collector).

Corroboration for `cpl004` = Appule, from `pak_index.txt`: its assets sit under `Cpl004`/`C004A`
with a DLC variant `Cpl004E` carrying `frie` (Frieza-force) tokens, and the live actor was on a
Namek field map — where Appule is a field enemy. The index never spells the name, so this is
supporting evidence, not proof.

**Still unverified:** whether any `AT_Character_cplNNN_BP_C` also ships an unauthored 1. One
sample is not a survey — if an ENEMY is ever announced as Goku, that is the tell.

**New dead end:** `navdump` FROZE the running game on 2026-08-18 (the dev channel acknowledged the
command, the file was never written, and both MCP channels went silent from that moment). Do not
run it on a live session until the census walk is bounded — see
[dbz-kakarot-crash-bug](dbz-kakarot-crash-bug.md).

### 2026-08-18 — `pak_index.txt` does NOT map CplNNN → character name (dead end, do not retry)

Swept ids `cpl015`–`cpl020` looking for a readable character token on the same path. Result:

- **Zero lines** anywhere in the 348k-path index carry a `cplNNN` id AND a character name.
- Character names DO appear in the index, but in a separate hierarchy that never mentions a cpl id:
  `AT/Content/MasterData/AI/BattleAI/Zarbon/BOSS_BATTLE_07/DT_BattleAI_Zarbon_Bs07_EnemyBehavior`,
  `.../Zarbon/EVENT_BATTLE_12/...`. The two naming spaces simply do not meet in asset paths.
- Skill tokens under `EFF/e/skill/CplNNN/` are NOT a character tell: `cpl004` (live-proven Apuru)
  carries the generic `Rkidan`, and an agent reading only that token guessed Tien. Do not identify
  a character from its skill asset names.
- `cpl015` and `cpl016` do not appear in the index at all.

**Nor can the enum ORDER be used to interpolate.** The runs are ordered by cpl id and each id owns a
contiguous block (proven three ways from the header alone: `Cpl003_C/_D` in the tail ↔ Vegeta at
8–10, `Cpl004_C05` ↔ Apuru at 11–14, `Cpl005_G` ↔ Freezer at 15–20) — **but there are gaps**: live
reads give cpl057 → 75 (FZBit) and then cpl064 → 78 (AlienA), so six ids in between own nothing.
Counting runs forward from a known anchor therefore drifts, and any id derived that way is a guess.

**The one decisive, zero-guess measurement:** an enemy's Blueprint class name carries its cpl id
(`AT_Character_cpl004p1c02_BP_C`) and its `CharacterType` carries the real name. Read both off a
live battle actor and the pairing is proven. That is how any new CPL_NAMES entry should be earned.

### 2026-08-18 — Cpl019 = Zarbon, proven live (and the method that proves any other)

Entering the Namek boss fight spawned `AT_Character_cpl019_A_BP_C`; its `CharacterType` read **35**
= `Zarbon_A_C01`. The Blueprint class name carries the cpl id, the enum carries the real name, and
one live battle actor pairs them with no inference at all. `Cpl019 = "Zarbon"` is now in CPL_NAMES.

**This is the only way a CPL_NAMES entry should be earned.** Interpolating from the enum's run
order — which is genuinely ordered by cpl id, with contiguous per-id blocks — placed Zarbon at
cpl017. That was **wrong by two**, because the runs have gaps. The structure looked reliable enough
to trust and was not; the measurement cost one command.

**Generalizable:** any enemy the player fights pairs its cpl id with a real name this way, so the
mod could LEARN the map at runtime (record `cplNNN` → `CharacterType` name whenever an
`AT_Character` is seen, then name quest actors sharing that `UniqueId`). Not built — noted as the
zero-guess route to naming story actors before their fight, since a character is only learned after
being seen as an enemy at least once.

## 2026-08-18 — THE ANSWER, and it was here since July: feed GetCharacterName the speakerID

`UAT_BlueprintFunctionLibrary::GetCharacterName` has been recorded at the top of this file since
2026-07-10 as *"returns "" for EVERY code — safe but useless"*. **That is wrong, and every naming
mechanism built since (CPL_NAMES, NPC_NAMES, the CHARACTER_TYPE enum) was a workaround for a
non-problem.** The function was being called with the BARE code. It wants the **`speakerID`** —
the same code plus a one-letter variant suffix.

Live, in game, es_MX:

| call | result |
|---|---|
| `GetCharacterName("Cpl002")` | `""` ← the call the July pass made |
| `GetCharacterName("Cpl002A")` | **"Gohan"** |
| `GetCharacterName("Npc004A")` | **"Bulma"** |
| `GetCharacterName("Cpl019A")` | **"Zarbon"** |
| `GetCharacterName("Cpl004A")` | **"Appule"** |
| `GetCharacterName("Cpl064A")` | **"Oficial del Ejército de Freezer"** |
| `GetCharacterName("Cpl065A")` | **"Comando del Ejército de Freezer"** |

- **Localized.** Those strings came back in the player's language because the game had
  `/Game/Message/PLAT_W/es_MX/messageData` loaded. `FMessageInfoTbl` (AT.hpp:7725) has per-language
  `Speaker_*` / `Message_*` columns including `_esmx`, `_spa`, `_eng`, …
- **Covers everyone**, not just fighters: story NPCs (Bulma) and anonymous mobs (which get a proper
  descriptive name rather than our generic noun).
- `speakerID` is a **StrProperty on AAT_CharacterBase**, so it answers on the enemy branch and the
  QuestCharacter branch alike — one read, both namers.
- `UEventBlueprintFunctionLibrary::GetMessageFromID` (AT.hpp:41265) returns the identical string;
  `GetSpeakerFromID` (AT.hpp:41256) returns `""` for these ids. Either of the first two works.
- **Safe.** No crash, on a heavily-populated streaming map. The shape is FString-in/FString-out on a
  static BlueprintFunctionLibrary — the same shape as the call July already recorded as safe, and a
  different shape from the fatal `MessageManager:GetNounParam*` (FName param / struct return on a BP
  object). That distinction held.

**The lesson worth more than the finding:** a dead end recorded as *"function X is useless"* was
really *"function X is useless with the argument we tried"*. The note never said which argument.
When a documented dead end blocks something this central, re-test it with every id spelling you
have before building a workaround — the speakerID was already in the codebase (`dev_charnames.lua`
had been reading and printing it since 2026-08-15) and nobody fed it back in.

CPL_NAMES / NPC_NAMES / `char_types.DISPLAY` are now FALLBACKS, kept for ids the message table has
no row for. They are no longer the primary path in either namer.

### 2026-08-18 (later) — `speakerID` is EMPTY on the ENEMY branch; derive the id from the class name

The speakerID fix named NPCs but the Namek mobs were still announced as "alien" from the enum. Cause,
measured on `AT_Character_cpl002_B_BP_C`: **`speakerID` and `CharacterName` are both blank strings on
`AAT_Character`**, while `CharacterType` read 4. So the localized lookup could never fire on enemies.
Nothing appeared in the log because no gate refused anything — the value was simply empty.

The id is recoverable from the **generated Blueprint class name**, which embeds the code:

| class | derived id | `GetCharacterName` |
|---|---|---|
| `AT_Character_cpl003_A_BP_C` | `Cpl003A` | **"Vegeta"** |
| `AT_Character_cpl004p1c02_BP_C` | `Cpl004A` | **"Appule"** |
| `AT_Character_cpl019_A_BP_C` | `Cpl019A` | **"Zarbon"** |
| `AT_Character_cpl064c01_BP_C` | `Cpl064A` | **"Oficial del Ejército de Freezer"** |
| `AT_Character_cpl065c01_BP_C` | `Cpl065A` | **"Comando del Ejército de Freezer"** |
| `AT_Character_cpl002_B_BP_C` | `Cpl002B` → `Cpl002A` | "" → **"Gohan"** |
| `AT_Character_cpl057Ac01_BP_C` | `Cpl057A` | **""** — no message row |

Rule (`char_types.speaker_ids_from_class`): match `[Cc]pl(%d+)`, take an **UPPERCASE** letter
immediately after the digits or after one underscore, default `A`. Requiring uppercase is what stops
the costume suffixes (`c01`, `p1c02`) from being read as a variant. A non-A letter is tried first and
then `A`, because a variant may have no row of its own — `Cpl002B` is exactly that case, and it is
why the fallback exists rather than being defensive padding.

`Cpl057A` (FZBit) and `Npc086A` return "": not every id has a row, so the enum/table fallback chain
stays. FZBit is still deliberately unmapped in `char_types.DISPLAY`, so those mobs speak the generic
enemy noun — correct behaviour, not a regression.

### 2026-08-18 (later still) — the variant letter is a RANK, not a constant

`GetCharacterName` keys on `CplNNN` + a letter, and the letter is **not always A**. Each family holds
several ranks, all with their own localized name:

| id | A | B | C | D |
|---|---|---|---|---|
| Cpl004 | Appule | Recluta del Ej. de Freezer | Guardia | Explorador |
| Cpl064 | Oficial del Ej. de Freezer | Cabo | Sargento | — |
| Cpl065 | Comando del Ej. de Freezer | Supercomando | Comando élite | — |
| Cpl057 | **(empty)** | Dron de ataque | Dron de asistencia | Dron de recuperación |

So "take the first letter that answers" — the obvious rule — is WRONG: it would call every drone
"Dron de ataque" and every AlienA "Oficial". `Cpl057A` being empty is what makes the naive rule look
plausible and the correct one non-obvious.

**The actor already says which rank it is.** `CharacterType`'s position inside its family's run of
enum values lines up with the answering letters *in order*, so the rule is: take the **P-th
NON-EMPTY letter**, where P is that position. Verified against every family observed (cpl004, 057,
064, 065, plus Zarbon/Vegeta/Gohan), and cross-checked against play — the mob the player fought
announced as "Cabo del Ejército de Freezer", which is `Cpl064B` = `AlienA_C02` = index 1. It matches.

`char_types.variant_index` walks backwards while the stripped base token matches (`Apuru_C02` ->
`Apuru`), which works because each family occupies one contiguous block of the enum — the same
structure the header's tail confirms with `Cpl003_C/_D`, `Cpl004_C05`, `Cpl005_G`.

**Still unnamed: `Npc086`** (the Namekian child of the fruit side-quest). All seven letters A–G
return "". Some NPCs genuinely have no row in the message table; the fallback chain stays for them.

### 2026-08-18 — NPCs the character table cannot name: `NpcTalkComponent.m_SpeakerId`

`Npc086`, the Namekian child of the fruit side-quest, returns "" from `GetCharacterName` on all seven
letters — yet the game names him on screen: *"Niño namekuseijin: ¿Vas a conseguirme algunas
frutas?…"*. He is named in the **speaker** column of the message table, not in the character table,
and the id that reaches it is a THIRD id space. Read live off his actor:

```
Sub_Npc086_01_Client.NPCTALK   (UNpcTalkComponent, AT.hpp)
   m_SpeakerId        Npc_ex675
   m_talkParamId      Sub_Npc086_01_Npc086_03
   CurrentMessageId   ¿Vas a conseguirme algunas frutas? ¿Pero por qué...?
```

Three things worth keeping:

1. **`m_SpeakerId` is not `UniqueId` and not `speakerID`.** `Npc_ex675` bears no relation to `Npc086`,
   which is why every letter of `Npc086` came back empty. A quest NPC can carry an id from a pool
   the character table never indexes.
2. **`CurrentMessageId` holds the RESOLVED LOCALIZED LINE**, not an id, despite the name. The
   component does its own text resolution, so that member is a ready-made subtitle source.
3. `UNpcTalkComponent` is attached as a component named `NPCTALK` on quest actors and
   `NpcTalkComponent` on `AT_Character`s — both spellings observed live in the same `findall`.

**UNVERIFIED:** whether `GetSpeakerFromID("Npc_ex675")` returns "Niño namekuseijin". That is the one
call left to make, and it is the whole feature — if it answers, every NPC the character table cannot
name gets its on-screen name from its own talk component. The probe run that would have answered it
**took the game down** (see the crash-bug note), so this is the first thing to run next session.

### 2026-08-19 — the name is in the message table, and an ASCII scan cannot see it

The live probe that would have resolved `Npc_ex675` **took the game down twice more** (see the
crash-bug note), so the question was answered **offline instead, from the paks** — no game needed,
no risk, and it is now the preferred way to ask this kind of question.

```
repak.exe get pakchunk0-WindowsNoEditor.pak     AT/Content/Message/PLAT_W/es_MX/messageData.uexp > md.uexp
```

`Npc_ex675` is present in the table's name map, and the table holds a **contiguous block of
character display names** that contains the exact string the player saw:

```
… Mujer egoísta | Niño namekuseijin | Alienígena masculino | Alienígena femenino | Niña |
  Súper Saibaman | Príncipe de los saiyajin | … | El guardián de la Tierra | Líder de los namekuseijin
```

**The trap that cost most of the session:** UE stores an FString as **plain ASCII when it is
ASCII-only and as UTF-16LE the moment it contains one accented character**. Both encodings sit in
the same file. A `grep`/ASCII-run scan therefore reads only the unaccented half of a Spanish table
and reports a name as ABSENT with total confidence — it found "Colono namekuseijin" and missed
"Niño namekuseijin" fifteen rows away. Two consequences worth carrying to any pak text search:

- **Always scan both encodings**, and treat an ASCII-only miss as *no evidence*, never as absence.
- `bash` **silently drops NUL bytes in command substitution**, so a UTF-16 grep pattern built with
  `$(printf ...)` collapses to ASCII and appears to work. Use python/perl for binary search.

One more self-inflicted one, same family: extracting in a `for pak in …` loop where the redirect
`> out` runs before `repak get` fails **truncates the good extraction from the previous pak**. The
file then reads as 0 bytes of "compressed data" and invites a wrong conclusion about encryption.

**Status: CONFIRMED IN PLAY (2026-08-19).** The player walked up to the Namek fruit-quest child and
the radar named him. So the talk component closes the gap the character table left: an NPC with an
empty `speakerID` and an unindexed `UniqueId` still names itself, in the player's language, from its
own conversation data. Treat `m_SpeakerId` as a first-class id source alongside `speakerID`, not a
special case for quest NPCs.

**Implementation.** `char_types.talk_speaker_id` reads `m_SpeakerId`
through the same strict gate as every multi-candidate probe, and `npc_name` consults it right after
`speakerID`. It only ever ADDS a name: an id the resolver has no row for returns "" and the chain
falls through to the existing behaviour. The remaining check is a listening one — walk up to that
child and confirm the radar says "Niño namekuseijin" instead of "personaje".
