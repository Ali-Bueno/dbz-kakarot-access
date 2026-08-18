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
