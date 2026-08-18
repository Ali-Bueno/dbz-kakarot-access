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
