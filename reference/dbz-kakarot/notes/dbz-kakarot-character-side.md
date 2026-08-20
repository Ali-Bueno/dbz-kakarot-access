# Ally or enemy? The game's own per-actor answer

**Why:** the radar announced **Krillin, a party companion, as an enemy** (user, 2026-08-19), and the
companions category was empty at the same time. Both symptoms had one cause.

## What was wrong

The radar asked `IsA(AAT_CharacterPlayableBase)` and treated the answer as "which side is this
character on". It is not that — it is a **class-identity** test, true only for the handful of
blueprints that happen to derive that class. Measured live:

```
Player (Gohan):  AT_Character_cpl002_B_BP_C -> ATCharacterCpl002B -> ATCharacterCpl002Base
                    -> AT_CharacterPlayableBase -> AT_Character -> ...
Krillin:         AT_Character_cpl006_BP_C   -> ATCharacterCpl006 -> AT_Character -> ...
```

Krillin does not derive it. So he failed the companion scan's positive test *and* the enemy scan's
negative one, landing in **enemies**; and because his `SpawnType` is 0 there was no specific noun,
so he came out as the generic "enemy". `AAT_CharacterPlayableBase` is an **empty marker class** of
the same size as `AAT_Character` (AT.hpp:14630), which is why it carries no side information.

`SpawnType` cannot substitute for it. The enum is `E_ENEMYSPAWN_TYPE`
(`NONE/ENCOUNT/QUEST/BOSS` = 0/1/2/3, AT_enums.hpp:10965) and describes **how an enemy was
spawned**: a level-placed roaming mob reads 0 exactly as Krillin does.

## The answer: `AttributeComponent` is polymorphic by side

`AAT_Character.AttributeComponent` (AT.hpp:14371, offset 0x08E8, **reflected**) points at a
per-side component class:

```
UATCharacterAttributeComponentBase            AT.hpp:26065
  +- UATPlayerAttributeComponent              AT.hpp:29077   <- ALLY
  |    +- UATMainPlayerAttributeComponent     AT.hpp:28891     the character you control
  |    +- UATSupportPlayerAttributeComponent  AT.hpp:29539     the party companion following you
  |    +- UATLimitedSupportPlayerAttributeComponent  AT.hpp:28870
  +- UATEnemyAttributeComponent               AT.hpp:28335   <- ENEMY
  +- UATAtrociousAttributeComponent           AT.hpp:24955   <- ENEMY (special)
```

Confirmed against a live ObjectDump of this save: Krillin's actor carries
`ATSupportPlayerAttributeComponent`, the player's carries `ATMainPlayerAttributeComponent`, and all
59 roaming field enemies carry `ATEnemyAttributeComponent`.

`Nav._char_side(actor)` implements it: one reflected hop, no out-params, no manager lookup.
**nil means DON'T KNOW** — 16 of 76 character actors in that dump carry no attribute component —
and both callers treat nil as unknown rather than as "enemy", keeping the old class test as the
fallback for that case.

## Dead ends — do NOT spend a session on these

| Looked promising | Reality |
|---|---|
| `FPartyMember` (AT.hpp:8648), reached via `UATParty.Party` | The struct reflects **ZERO** members. The property is a readable handle to an opaque blob |
| `UATSupportSystem` (AT.hpp:29547) | No reflected properties at all |
| `bEnableHiddenOnJoinedSupport` | An `AQuestCharacter` VISIBILITY toggle on a sibling branch of `AAT_Character` — a party member's actor can never have it |
| Every `TArray<CHARACTER_TYPE>` in the headers | All `FTableRowBase` quest DATA TABLES: what a party *should* be per quest, not what it *is* |
| A team/faction UPROPERTY or `IsAlly`-style getter on `AT_Character`/`AT_CharacterBase` | Does not exist. A whole-header grep finds only two hits, both on UI widgets |
| `StatusCmp`'s class | Same class for every character. The polymorphism is one level deeper, on `AttributeComponent.StatusInstance` (`ATEnemyStatus` vs `ATPlayerStatusInstance`) — usable, but two nil-able hops and no main/support split |

Also reachable if a future feature needs the *identity* of the field companion:
`UATPartyManager.AccompanyChara` (AT.hpp:29033, reflected) on `BP_ATGameModeMain_C_0.ATPartyManager`,
and `UATParty.GuestCharacters` for quest guest allies.

## The second bug in the same report: nearby NPCs were undetectable

Standing 2 m from Bulma, the **Personajes** category did not exist. She was not missing — she was
**claimed by another category**. She was the current quest target, so the navi-icon walk (section 1
of the sweep) filed her under *Misiones*, and `add_target`'s dedup is by ADDRESS and GLOBAL, so the
NPC scan (section 3) hit it and returned. With her gone the group had no members left — the other
`QuestCharacterBase_C` instances nearby are hidden by the game's absence observer or past the 300 m
cap, and NPCs are explicitly barred from the empty-group rescue — and a zero-item group is never
emitted at all.

Everything else checked out and was ruled out by measurement: `bHidden = false`,
`CurrentHiddenType = 0`, the property gate logged no refusal for her class, the scan looks for
exactly her class (`QuestCharacterBase_C`), and the sweep's time box cannot starve later sections
(it marks the build partial and refuses to publish it).

Fix: the NPC scan opts out of the global dedup (`add_target`'s `dup_ok`) and keeps its own, so a
character who is also the quest marker is listed in **both** categories. She genuinely is both.

**The general lesson:** a global dedup by address makes the FIRST scan to reach an actor its sole
owner, and every later classification of that actor fails silently, with no log line. When a
category is unexpectedly empty, ask which OTHER section might already have claimed its members.
