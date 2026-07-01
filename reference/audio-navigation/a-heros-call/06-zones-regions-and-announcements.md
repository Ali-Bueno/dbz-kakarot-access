# 06 — Zones, Regions & Boundary Announcements

There are **two distinct spatial-grouping concepts** in A Hero's Call. Keeping them separate is
important when porting.

| Concept | Type | Purpose | Player feedback |
|---------|------|---------|-----------------|
| **Region** | `FPRegion` (per tile, `tile.Region`) | Named navigable area ("the Armory", "East Corridor") | **Speech** on enter/leave; named in scans |
| **Zone** | `Zone : IAudibleZone` (per tile, `tile.ZoneName`) | Acoustic environment (reverb/snapshot) | **Audio** reverb change (no speech) |

A tile carries both a `Region` (gameplay/orientation) and a `ZoneName` (acoustics). They often overlap
spatially but answer different questions: *"where am I?"* vs *"what does this space sound like?"*.

---

## 1. Region announcements (speech) — `FPExploring.HandleRegion`

Already shown in Doc 04 §7. Per frame: read the region of the tile under the player; if it differs from
the remembered region, **cancel current speech** and say `entering {name}` (or `leaving {name}` when
stepping into unnamed space). Returns a "changed" flag that forces a fresh forward POI scan so the new
area is immediately described.

```csharp
FPRegion region = map.Tiles.Get(floor(Position.X), floor(Position.Y)).Region;
if (region != mCurrentRegion) {
    mWorld.Speech.CancelAllSpeech();
    if (region.IsNone() && mCurrentRegion != null) TranslateSO.TSay("leaving {0}",  mCurrentRegion.FriendlyName);
    else                                           TranslateSO.TSay("entering {0}", region.FriendlyName);
    mCurrentRegion = region;
}
```

Regions also surface in scans (Doc 05): `AddSectionPOI` ("`{region} {dist}`"), `AddEndOfRegionPOI`
("`end of {region} {dist}`"), and the range-collapsing in `AddRangeToLastRegion`.

`FPRegion.None` is the sentinel for "no named region"; `region.IsNone()` / comparing against
`FPRegion.None.FriendlyName` is how unnamed space is detected.

---

## 2. Zone acoustics (reverb snapshots) — `MapSoundHandler.UpdateMapSoundss`

Zones bind a tile area to an **FMOD environmental-reverb event** (`Zone.EnvironmentalReverb`). On map
load, each `IAudibleZone` becomes a `ZoneInfo { Name, EnvironmentalReverb, Event }` where `Event` is a
preloaded FMOD event instance.

Per frame, when the player's tile `ZoneName` changes, the old zone's snapshot is switched off and the
new one on, via an FMOD parameter `"StopSnapshot"` (0 = active, 1 = stopped):

```csharp
IAudibleTile curTile = map.GetTile(floor(playerPos.X), floor(playerPos.Y));
if (curTile != mLastTile && (mLastTile == null || curTile.ZoneName != mLastTile.ZoneName)) {
    if (mLastTile != null) {                          // turn OFF previous zone's reverb
        var prev = mZones.FirstOrDefault(z => z.Name == mLastTile.ZoneName);
        prev?.Event?.mEventInstance.setParameterValue("StopSnapshot", 1f);
    }
    var cur = mZones.FirstOrDefault(z => z.Name == curTile.ZoneName);   // turn ON current
    if (cur?.Event != null) {
        cur.Event.mEventInstance.setParameterValue("StopSnapshot", 0f);
        cur.Event.Play();
    }
}
mLastTile = curTile;
```

So crossing a zone boundary cross-fades the reverb character (e.g. dry corridor → cavernous hall)
purely through audio, with no announcement. The `IsIndoors` tile flag additionally feeds the FMOD
`"PlayerIndoors"`/`"SoundIndoors"` parameters per sound (Doc 03 §4, Doc 08 §3) to colour occlusion.

---

## 3. Interfaces

```csharp
// Sound/Ian/IAudibleZone.cs
public interface IAudibleZone { string Name; string EnvironmentalReverb; }   // (+ flags)
// tile side (Doc 03): IAudibleTile.ZoneName, IAudibleTile.IsIndoors
```

---

## 4. Porting notes

- **Separate your two layers.** Use *named trigger volumes / nav regions* for the spoken "entering X"
  and *acoustic zones* for reverb. In Unity (7DTD) these can both be trigger colliders tagged
  differently; in RE Engine (SF6/RE7) hook the existing area/room ids if exposed.
- Region announce pattern: store `currentRegionId`; each frame compare with the region under the
  player; on change, **interrupt speech** then announce. Interrupting (not queueing) is deliberate —
  the newest area is what matters.
- Reverb switching maps cleanly to FMOD snapshots or to your engine's reverb zones; the
  `StopSnapshot 0/1` toggle is just "activate current, deactivate previous."
- Drive occlusion colour with an `indoors` boolean per emitter+listener if your game distinguishes
  interior/exterior; it noticeably improves the sense of space.
