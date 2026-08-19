# dbz-kakarot-item-names

> Naming collectibles (Dragon Balls, D Medals, chests, placed items) on the radar. The chain is
> mapped; the last hop is UNTESTED because no item actor was loaded when this was written.

## What the radar says TODAY (read from the code, 2026-08-18)

| Thing | Class / source | Spoken now |
|---|---|---|
| Dragon Ball | `EMapIcon::DRAGON_BALL` = 28 | **"Bola de Dragón"** — works, category `dragonball` |
| D Medal | `DMedalTreasure_BP_C` (< `AAccessPointItemBase`) | "objeto" (`cat_item`) |
| Event chest | `EventItemTreasure_BP_C` (< `AAccessPointItemBase`) | "objeto" |
| Placed item | `APlacementObjectInfo` | the RAW id, in English — e.g. "Lost Seaweed Area11 01" |

`AAccessPointItemBase` gets **no name at all** in the collectible scan (`nav_tracker.lua`, section 4:
only `c.action` and `c.item` produce a name), and `APlacementObjectInfo` gets `drop_item_name`, which
speaks the drop-table key whenever it contains a 3-letter run. That is why half the collectibles say
"objeto" and the other half read out an internal identifier.

## The name chain (from the CXX dump)

```
AItemStaticActor.ItemId (FName, reflected)        AT.hpp:15780
  └─ ADragonBallStaticActor derives from it       AT.hpp:15370   (+ bool m_IsFloat)
        │
        ▼
item table row: FItemInfoParam.JName (FName)      AT.hpp:6867  ← the NAME's message id
  (FItemTableImportant, …Material, …Heal, …Recipe all inherit FItemInfoParam)
        │
        ▼
UEventBlueprintFunctionLibrary::GetMessageFromID(FString) -> FString   AT.hpp:41265
        → the LOCALIZED item name
```

`GetMessageFromID` is the same call that resolves character names (see
[dbz-kakarot-npc-names](dbz-kakarot-npc-names.md) — it is a general message-id lookup, not a
character one), and it is proven safe on this build.

## Tested and EMPTY (do not retry these ids)

Fed to both `GetMessageFromID` and `GetCharacterName`, live, 8 actors each — every one returned "":

- `APlacementObjectInfo` → `ItemTableComponent.FieldItemDropData.FixedId` / `NormalId`
  (`Lost_Seaweed_Fixed`, `Lost_Seaweed_Area11_01`)
- `AAccessPointItemBase` → `TreasureSaveName` (`L11_DMEDAL_310`, `L11_EVENT_095`)

These are **drop-table keys and save keys, not message ids**. The lesson from the character round
applies in reverse: there, the right id existed and was being fed the wrong spelling; here the ids
being fed are the wrong KIND of id entirely.

`EITEM_ID` in `AT_enums.hpp` is a stub — `NONE = 0, EITEM_MAX = 1`. Items are keyed by FName, not by
that enum. Do not build anything on it.

## Next step (needs the game running, near an item)

`dev_charnames.lua` now walks `DragonBallStaticActor` and `ItemStaticActor`, reads `ItemId`, and
feeds it to both resolvers. **Neither class was loaded on the Namek map**, so `ItemId` has never been
read. Run `charnames` standing near a Dragon Ball or a static pickup:

- If `GetMessageFromID(ItemId)` answers, items name themselves the same way characters do and the
  whole feature is a few lines.
- If it returns "", the missing hop is the item TABLE row (`ItemId` → `JName`), which means reading
  DataTable rows from Lua — the known blocker, since UE4SS exposes no TMap/RowMap iteration.

Open question for the player, not answerable from code: Dragon Balls already announce as "Bola de
Dragón". Whether the ask is the STAR NUMBER ("Bola de 4 estrellas") or just naming the OTHER
collectibles decides how much work this is — the star number needs a per-ball index nobody has
looked for yet, and `ADragonBallStaticActor` exposes only `m_IsFloat`, so it is probably in `ItemId`.

## 2026-08-18 — shipped: noun by class name (the interim, not the real fix)

The player's answer to the open question was *"que se nombren los demás coleccionables"* — Dragon
Balls stay as they are, the rest need names. Shipped now, offline, no live data required:

- `nav_tracker.lua` section 4 gains a block-local `COLLECTIBLE_NOUN` list of class-name patterns ->
  i18n noun, replacing the single hard-coded `find("Memories")`. **First match wins**, which matters:
  `DMedalTreasure_BP_C` contains both `DMedal` and `Treasure`.
- New i18n keys `cat_dmedal` ("Medalla D" / "D Medal") and `cat_treasure` ("tesoro" / "treasure").
- `drop_item_name` now strips the drop-table bookkeeping: `_Fixed`, `_Normal`, `_AreaNN_NN`, a
  trailing `_NN`. `Lost_Seaweed_Area11_01` was being read out in full; it now says "Lost Seaweed".
- `is_memory` is derived from the resolved noun instead of a second `find`, so they cannot drift.

Coverage checked against the pak index rather than against the one area that was loaded — every
collectible Blueprint in the game is one of: `BP_FieldMemoriesActor`, `BP_DLC6_FieldMemoriesActor`,
`BP_DLC7_FieldMemoriesActor`, `BP_DLC7_FieldMemoriesActor_FM_019`, `DMedalTreasure_BP`,
`EventItemTreasure_BP`, `TreasureEventItem_BP`, `ItemStaticActor_BP`, `DragonBallManager_BP`. The
four memory variants all contain `FieldMemories` and `TreasureEventItem` contains `Treasure`, so the
four patterns cover all eight pickups.

**`ItemStaticActor` was deliberately NOT added to the scan list.** It is the class that would carry a
real `ItemId`, but `findall ItemStaticActor` returned **no instances** live, and the playbook rule is
explicit: every class name an adapter names joins the ABSENT scan set and costs a full `FindAllOf`
every ~4 s forever, so name only what a dump has actually shown. A pak path proves an asset exists,
not that it is ever loaded. Dragon Balls already reach the radar through map icon 28 anyway.

**These nouns are DERIVED FROM CLASS NAMES, not from the game's own text**, and that is the
limitation to fix next: `ItemId -> JName -> GetMessageFromID` gives the real localized name. If the
game words a D Medal or a chest differently on screen, the game's wording wins over this table.

## 2026-08-18 — action points: why the bonfire "was not on the radar", and why shops showed as Sites

Player report: *"Sitios está detectando las tiendas / máquinas de cocina y la tienda que puso Bulma
en Namek"*. Measured live — the seven `AFieldActionPointActor`s loaded on Namek:

| Actor class | Own map icon? | Was announced as | Now |
|---|---|---|---|
| `BP_BonfireActor_C` | **no** | "Sitio", **no name** | Sites / `cat_bonfire` |
| `FoodNobody_Store_BP_C` | **no** | "Sitio", **no name** | Shops / `cat_food_shop` |
| `RestaurantNobody_Store_BP_C` | **no** | "Sitio", **no name** | Shops / `cat_restaurant` |
| `TCGMachine_BP_C` | yes | Minigames (icon wins) | unchanged |
| `TrainingPoint_BP_C` ×3 | yes | Sites / `cat_practice` | unchanged |

**Root cause: `ActionName` is EMPTY on all of them** (read live on the bonfire and the food stall),
and the action-point branch labelled every non-memory point with the GROUP's own label as its noun.
So the naming path existed and produced nothing.

**This also closes the earlier "add bonfires to the radar" request.** The bonfire was never missing —
`EMapIcon::Bonfire` = 64 is mapped, `cat_bonfire` = "hoguera de cocina" has existed all along — but
the live bonfire actor carries **no `ATMapIconComponent` at all**, so it never went through the icon
path that would have named it. It arrived through the action-point path and came out anonymous. The
lesson: an icon type being mapped proves nothing about whether the ACTOR carries that icon.

**Why the shops arrive here at all:** they DO have map icons, but on separate icon-only actors
(`FoodMapIcon_010`, `CookingMapIcon_010`) rather than on the store. The icon actor is correctly
grouped under Shops; the store actor came through the action-point path and was misgrouped. It is now
routed to Shops rather than skipped, because skipping would lose any store in an area that has no
icon actor — and `add_target` dedupes by ADDRESS, so the pair stays two entries, both in the right
group and adjacent by distance.

Unrecognised action-point classes keep the old behaviour, so the change can only improve a
classification, never remove one.


## 2026-08-19 — SOLVED: the item name needs one more hop than we were making

The player reported a Namek quest fruit that the radar called "tesoro" while the game, on pickup,
called it **"Fruta namekuseijin"**. Both were right about different things: the radar was describing
the *container* (the actor's class name contains `Treasure`, so it got our generic noun) and the game
was naming the *contents*.

**The chain, verified offline from the paks — no game running, no crash risk:**

```
actor -> ItemTableComponent -> FieldItemDropData.FixedId / .NormalId   ("Lost_Seaweed_Fixed")
      -> row in Parameter/Field/LotteryLostPropertyItemsTable          -> DropItemId_1 = Item_36029
      -> message key `Item_36029_Name`                                 -> "Algas marinas"
```

Two of these were already in hand and the conclusion was still wrong, so the lesson is worth stating:
the 2026-08-18 round tested `Lost_Seaweed_Fixed` against the resolver, got "", and concluded that
collectibles cannot be named. **That id was never an item id** — it is a drop-table ROW KEY. One
missing hop looked exactly like a dead end.

**Why this one needs a shipped table when nothing else in the mod does.** The item name space is
NUMERIC (`Item_11001`..`Item_96016`, 956 entries with a `_Name` key) while the actor only ever hands
us an alphabetic slug. Several distinct slugs collapse onto one numeric id and the ranges jump
arbitrarily, so no string surgery bridges them; the bridge is a DataTable row read, and a
`UDataTable`'s `RowMap` is not reflected, so Lua cannot do it at runtime. Hence a GENERATED
`item_drop_ids.lua` (slug -> number), produced by `tools/item-ids/dump_drop_item_ids.py`.

The map holds **no text at all**, only id pairs. The displayed name still comes from the game's own
message table at runtime, so it stays in the player's language automatically and a new translation
never touches it. Only an item renumbering in a game patch would, and then the generator is re-run.

**Two measurement traps cost most of this session, both the same shape — trusting a text search over
binary data:**

1. **UTF-16.** UE stores an FString as ASCII when it is ASCII-only and as UTF-16LE the moment it
   contains one accented character; both live in the same file. An ASCII-only scan of a Spanish table
   reads half of it and reports a present string as ABSENT with total confidence.
2. **FName numbers.** An FName is (name-table index, number) — `Item_36029` is stored as the entry
   `Item` plus a binary number. `grep` for `Item_36029` finds NOTHING in the very table that defines
   it. This nearly made me discard a correct sub-agent result as fabricated: its numeric ids looked
   unsupported because the name map only contained a bare `Item`.

Corollary for anything read out of a pak: **a text-search miss is not evidence of absence.** Confirm
with a parser, or with a known-good pair that must come out right.
