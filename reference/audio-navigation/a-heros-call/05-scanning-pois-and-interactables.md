# 05 — POI Scanning, Examine & Interact

Source: `FirstPerson/Ian/POIHelpers.cs`, `POIList.cs`, `POI.cs`, plus the action handlers in
`FPExploring.cs`. Interactable data lives in `RPG/Ian/MapScriptObject.cs` + `MSOStrain.cs`.

Where the radar (Doc 02) is *continuous ambient* feedback, **scanning is on-demand speech**: "tell me
what's in front of me." A scan casts a ray and produces an ordered, spoken list of everything notable
along it — region transitions, walls/doors, objects — each with a distance.

---

## 1. The scan raycast — `GetLineCollisionWithPointsOfInterest`

```csharp
public static POIList GetLineCollisionWithPointsOfInterest(
        IFPWorld ent, V2 directionUnitVector, float maxDistance)
{
    IClientMap map = ent.Map;
    POIList list = new POIList();
    FPTile playerTile = FSO.FPML.GetPlayerTile(ent);
    TString curRegion = playerTile.Region.FriendlyName;
    V2 position = ent.Position;
    float step = 0.1f;                                  // march granularity (tiles)
    V2 v = step * directionUnitVector;

    // First: does the ray hit a map OBJECT? (separate circle/segment test, Doc 03 CH.GetObjectCollision)
    SObjectCollision oc = CH.GetObjectCollision(position, directionUnitVector, maxDistance,
                                                FSO.FPML.GetVisibleObjects(ent.Map.GetObjects()));
    IClientMapObject obj = oc?.Object;

    for (float d = 0f; d <= maxDistance; d += step) {
        position += v;
        int tx = (int)Math.Floor(position.X), ty = (int)Math.Floor(position.Y);
        if (tx < 0 || ty < 0 || tx > map.PaddedWidth || ty > map.PaddedHeight) return null;
        FPTile tile = map.Tiles.Get(tx, ty);
        if (tile == playerTile) continue;

        // (1) Wall / door blocks the ray -> final POI, stop.
        if (tile.Terrain.Wall || tile.Terrain.Door) {
            AddRangeToLastRegion(list, curRegion, d);
            AddWallPOI(list, position, d, tile);        // "<region> <terrain> <dist>"
            return list;
        }
        // (2) Object within its radius -> object POI; focus it if within 2.5; stop.
        if (obj != null && (position - obj.Position).Length < obj.Radius) {
            AddRangeToLastRegion(list, curRegion, d);
            POI p = AddSObjectPOI(list, position, d, tile, obj);   // "<name> <dist>"
            if (d <= 2.5f) list.FocusedInteractible = p;           // <-- close enough to interact
            list.FirstInteractible = p;
            return list;
        }
        // (3) Region/terrain name changed -> emit a section POI at this boundary.
        if (GetSectionName(tile) != curRegion) {
            AddRangeToLastRegion(list, curRegion, d);
            if (GetSectionName(tile) == FPRegion.None.FriendlyName) {
                if (list.Count == 0) AddEndOfRegionPOI(list, curRegion, position, d, tile);
            } else {
                AddSectionPOI(list, position, d, tile);            // "<region|terrain> <dist>"
            }
            curRegion = GetSectionName(tile);
        }
    }
    AddRangeToLastRegion(list, curRegion, d);
    AddOutOfSightPOI(list, position, d, null);          // "As far as I can see <dist>"
    return list;
}

private static TString GetSectionName(FPTile t)
    => t.Terrain.Impassable ? t.Terrain.FriendlyName : t.Region.FriendlyName;
```

### What the spoken strings look like (localized via `TranslateSO._`)

| Builder | Text template | When |
|---------|---------------|------|
| `AddWallPOI` | `"{region} {terrain} {dist}"` or `"{terrain} {dist}"` | ray hits wall/door |
| `AddSObjectPOI` | `"{objectFriendlyName} {dist}"` | ray hits an object |
| `AddSectionPOI` | `"{region|terrain} {dist}"` | crossing into a new named region/impassable |
| `AddEndOfRegionPOI` | `"end of {region} {dist}"` | region runs out into unnamed space |
| `AddOutOfSightPOI` | `"As far as I can see {dist}"` | nothing blocked within maxDistance |
| `AddRangeToLastRegion` | appends a second number to make a **range** `"… {from} {to}"` | a region spanned >1 tile before the next POI |

`Math.Round(distance)` is used for spoken distances. `AddRangeToLastRegion` is the clever bit that
turns "Corridor 3" + "Corridor 7" into a spoken **range** so a long stretch reads as one item.

`POIList` is just `List<POI>` plus two slots: `FocusedInteractible` (object within 2.5 tiles, the one
examine/interact will act on) and `FirstInteractible`.

---

## 2. Where scans are triggered (`FPExploring`)

| Trigger | Direction | Distance | Speech |
|---------|-----------|----------|--------|
| `RunForwardScan` (key `FPScanForward`) | `Front` | `GameConfig.ScanDistance` | `AnnouncePOIs(list)` |
| `RunLeftScan` (`FPScanLeft`) | `Left` | `ScanDistance` | `AnnounceSidePOIs(list, Left)` |
| `RunRightScan` (`FPScanRight`) | `Right` | `ScanDistance` | `AnnounceSidePOIs(list, Right)` |
| `RunForwardComparison` (auto, every frame) | `Front` | `ScanDistance` | speaks only when the POI list **changed** |

### Auto-forward scan (`RunForwardComparison`) — the "look ahead while walking" feature

```csharp
if (!announcedRegion && (Now - LastForwardComparison).TotalSeconds < 0.1) return;   // 10 Hz cap
POIList list = POIHelpers.GetLineCollisionWithPointsOfInterest(mWorld, Front, ScanDistance);

ChangeFocusedUnit(list.FocusedInteractible?.SObject);     // updates the currently-targeted object

if (!POIHelpers.ArePOIsSame(list, LastForwardPois)) {     // only speak on change
    LastForwardComparison = Now;
    bool movingForward  = LastTravel.AlmostEquals(Front);
    bool movingBackward = LastTravel.AlmostEquals(Back);
    if (AutoForwardScanEnabled && !movingForward && !movingBackward && !mTurnedWithMouseThisLoop)
        AnnouncePOIs(list, …);
    LastForwardPois = list;
}
```

Key design choices to copy:
- **Diff-gated speech** (`ArePOIsSame` compares each `POIText`): never re-speak an unchanged view.
- **Suppress while moving straight forward/back or while mouse-turning** — those are exactly the cases
  where the radar already covers you and re-reading would be chatter.
- Toggle with `ToggleAutoForwardScanEnabled`.

---

## 3. Interactables: `MapScriptObject` / `MSOStrain`

An interactable is a `MapScriptObject` (implements `IAudibleObject`, `IClientMapObject`, etc.). Its
template ("strain") `MSOStrain` carries the data the navigation/audio systems read:

```csharp
// MSOStrain (template) — the navigation-relevant fields
string Interact;                 // script snippet name to run on interact (blank = not interactable)
TString Description;             // examine text
string FocusSound;               // 3D one-shot when this becomes the focused object
string RadarSound;               // directional radar sound id
string ConstantSound;            // looping ambient (3D positioned, occluded)
ConstList<string> SequenceSoundStrings;  // sequence of vocal/sfx
float Radius;                    // circle radius used by the scan's object-hit test
bool HeardThroughWalls;          // bypass occlusion
bool SilentInMenus;              // mute while a menu is open
```

The `MapScriptObject` exposes `RenderPosition`, `FriendlyName`, `Interact`, `Description`, etc.; the
audio system (Doc 08) starts/stops its sounds as it enters/leaves the active actor set.

---

## 4. Examine & Interact (`FPExploring`)

```csharp
// FPExamine: ray 5 tiles along Front; speak description (or name if none)
void AttemptExamine() {
    POIList list = POIHelpers.GetLineCollisionWithPointsOfInterest(mWorld, Front, 5f);
    var so = list.FocusedInteractible?.SObject;
    if (so == null) { DiagSO.Say("Nothing to examine"); return; }
    TranslateSO.TSay(so.Description.IsBlank() ? so.FriendlyName : so.Description);
}

// FPInteract: ray 5 tiles; run the object's Interact script
void AttemptInteract() {
    POIList list = POIHelpers.GetLineCollisionWithPointsOfInterest(mWorld, Front, 5f);
    var so = list.FocusedInteractible?.SObject;
    if (so != null && !so.Interact.IsBlank()) {
        mWorld.FPState.InteractedUnit = so;
        FSO.FPL.RunSnippet(mWorld, so.Interact, out _, out bool openedMenu);
        if (openedMenu) so.Message(FPMessages.PlayerStartedTalkingToYou);
    } else DiagSO.Say("No interaction available");
}
```

Note both use **range 5** (not the long scan distance) and act on `FocusedInteractible` — i.e. the
object must be within the **2.5-tile focus window** set during the scan to be acted on, but the ray is
allowed to reach 5 to find/aim at it. When an object becomes focused, `ChangeFocusedUnit` plays its
`FocusSound` (a 3D cue at the object), giving audible confirmation of what you're targeting.

---

## 5. Porting notes

- The scan is just the Doc 03 raycast plus "name the things you cross." In a 3D game, replace the tile
  march with a physics ray (or several rays in a small fan) and classify hits: wall layer → "wall",
  enemy/interactable layer → that entity's name, trigger volumes → region name. Keep the **0.1-tile
  march / fine step** idea if you want region-boundary distances; otherwise use the physics hit
  distance directly.
- Reproduce **diff-gated auto-scan** and the **2.5 focus / 5 act** split; both massively reduce verbal
  clutter while keeping a reliable "what am I aimed at" channel — directly useful for SF6 (which enemy
  am I facing + range) and RE7 (what's the interactable/door ahead).
- Spoken distance = `round(rayDistance)`; range-collapse consecutive same-name hits.
