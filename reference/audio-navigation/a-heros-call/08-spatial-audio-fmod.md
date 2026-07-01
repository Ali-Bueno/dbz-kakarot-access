# 08 — Spatial Audio (FMOD): Panning, Attenuation, Occlusion

Source: `Sound/Ian/FModSoundContext.cs`, `FModEventWrapper.cs`, `I2DSoundSource.cs`,
`I3DSoundSource.cs`, `IMovableSound.cs`, `SoundConfig.cs`, `MapSoundHandler.cs` (driver, Doc 00 §4).

Two parallel audio paths: **flat stereo pan** (radar cues — discrete L/C/R) and **true 3D positioned**
(everything in the world — objects, terrain, ambient), the latter with distance attenuation, wall/door
occlusion, and indoor/outdoor colouring. Both are FMOD Studio *events* whose DSP/mix behaviour is
authored in the FMOD project and *parameterized from C#*.

---

## 1. Stereo panning for radar (discrete) — the `"Panning"` parameter

Radar cues don't use 3D position; they set a single FMOD event parameter named **`"Panning"`** to a
discrete value:

```csharp
// FModSoundContext.GetRadarSound
public I2DSoundSource GetRadarSound(string name, float panningValue) {
    FModEventWrapper ev = InnerGetEvent(name);
    if (ev == null) { DiagSO.ESay("unable to find radar sound " + name); return null; }
    ev.mEventInstance.setParameterValue("Panning", panningValue);   // 0=L, 1=C, 2=R
    return ev;
}
```

`ReactiveRadar` builds its beams with `0f / 1f / 2f` (Left/Front/Right; Doc 02). The actual L↔R balance
curve for those three values is authored inside the FMOD event on the `"Panning"` parameter. So in
FMOD-land, "panning" is just an automation parameter you map to a panner; in a non-FMOD port you'd map
`{0,1,2}` to stereo balance `{-1, 0, +1}` (or use 3 mono sounds routed L/C/R).

---

## 2. 3D positioning — listener & sources

### Listener (once per frame, from player pose) — `MapSoundHandler`

```csharp
V2 fwd = MyMath.GetUnitVectorFromCompassDegrees(facingDeg);   // heading -> unit vector
V3 pos = VectorHelpers.MyTransform(new V3(0,-0.0001f,0), facingDeg, playerPos.AsV3()); // ~player pos
var attr = new FMOD.Studio._3D_ATTRIBUTES {
    forward  = { x = fwd.X, y = fwd.Y },
    up       = { x = 0, y = 0, z = 1 },     // mFPUpVector = (0,0,1)
    position = { x = pos.X, y = pos.Y },
};
mSC.mFModStudio.setListenerAttributes(0, attr);
```

The world is 2D (XY); `up` is `+Z`. FMOD then computes pan/attenuation for every 3D event relative to
this listener. (The tiny `-0.0001` Y offset avoids a degenerate zero vector in `MyTransform`.)

### Source position — `FModEventWrapper.Update`

```csharp
public void Update(V3 position) {
    var attr = new _3D_ATTRIBUTES { position = position.AsFModV3() };
    mEventInstance.set3DAttributes(attr);
}
```

`_3D_ATTRIBUTES { VECTOR position, velocity, forward, up }`. Sources set only `position` (no Doppler
on world emitters). Positions are tile-units; tile emitters sit at tile centers `(X+0.5, Y+0.5)`.

---

## 3. Distance attenuation

From `Sound/Ian/SoundConfig.cs` and `FModEventWrapper`:

| Parameter | Value | Meaning |
|-----------|-------|---------|
| `SoundMinDistance` | `1f` | below this, full volume (FMOD `set3DMinMaxDistance` min) |
| `SoundMaxDistance` | `15f` | rolloff reference max |
| `SoundRolloffFactor` | `1f` | curve steepness |
| `MuteDistance` (`FModEventWrapper`) | `13f` | beyond this the source is culled/muted |

The **rolloff curve itself** (inverse / linear / custom) is authored in the FMOD project; C# only sets
the min/max bounds. `MuteDistance` is used by the driver to cull and to size search radii (e.g. shared
terrain sounds search `MuteDistance * 2` tiles for the nearest emitting tile). Helper to convert a
"mute gain" into a distance:

```csharp
// SoundLogic.GetMuteDistanceBasedOnMuteGain
return (minDistance / muteGain - minDistance) / rolloffFactor + minDistance;
```

---

## 4. Occlusion (wall/door muffling) — the navigation-critical part

The obstruction *amount* is the raycast thickness from Doc 03 (`GetWallAndDoorDepth`). It is applied in
two steps by `ObstructionHelpers.PositionSoundForObstruction` (Doc 03 §4):

1. **Hard mute** if `depth >= 1f` (≥1 tile of wall/door → silent).
2. Otherwise push `depth` (0..1) plus indoor flags into the event:

```csharp
// FModEventWrapper.SetObstructionAndIndoorStatus
mEventInstance.setParameterValue("Obstruction",   obstruction);          // 0..1 -> low-pass muffle
mEventInstance.setParameterValue("PlayerIndoors",  playerIndoors ? 1 : 0);
mEventInstance.setParameterValue("SoundIndoors",   soundIndoors  ? 1 : 0);
```

The FMOD project's DSP chain reads `"Obstruction"` to drive a low-pass filter (and likely volume), and
the two `*Indoors` booleans to pick reverb/EQ. There is also a lower-level
`IMovableSound.SetDirectLowPass(gainHF, gain)` / `RemoveDirectLowPass()` for direct LPF control if you
aren't routing through the event parameter.

So the **occlusion model is: raycast tile thickness → {mute if ≥1 tile, else 0..1 muffle amount} → FMOD
low-pass.** Simple, cheap, and reproduces "muffled through the wall, silent through two walls."

---

## 5. Interfaces (contracts to re-implement)

```csharp
// I2DSoundSource (radar/UI) : ISound, IDisposable
bool Mute; bool IsLooping; bool IsDisposed; float Panning; float Pitch;
void Play(); void Stop(); void PlayAndDispose(); void PlayAndCallback(Action cb);
bool IsCompleted(); double GetPercentCompleted();

// IMovableSound (anything 3D-positioned & occludable)
string OriginalFile; bool Mute; float MuteDistance;
void Update(V3 position);
void SetDirectLowPass(float gainHF, float gain = 1f); void RemoveDirectLowPass();
void SetObstructionAndIndoorStatus(float obstruction, bool playerIndoors, bool soundIndoors);

// I3DSoundSource : IMovableSound (+ IsLooping, Volume, SecondarySound, …) — world emitters
```

`IAudibleObject` (Doc 05 §3) is the world-object side: `RenderPosition`, `HeardThroughWalls`,
`SilentInMenus`, and the sound ids `FocusSound / RadarSound / ConstantSound / ConstantEvent /
SequenceSoundStrings`.

---

## 6. FMOD playback API surface used

```csharp
FMOD.Studio.System.create(out mFModStudio);
mFModStudio.initialize(1024, INITFLAGS.LIVEUPDATE, FMOD.INITFLAGS.NORMAL, IntPtr.Zero);
mFModStudio.getEvent(path, out EventDescription ed);
ed.createInstance(out EventInstance inst);
inst.start();  inst.stop(STOP_MODE.ALLOWFADEOUT);
inst.set3DAttributes(attr);  inst.setParameterValue(name, value);
inst.setVolume(v); inst.setPitch(p);
mFModStudio.setListenerAttributes(0, listenerAttr);
mFModStudio.update();                  // pumped on the sound thread every 10 ms
```

### Threading model (important for mods)

- Game thread enqueues sound commands into a `BlockingCollection<Action>`.
- A dedicated **sound thread** ticks every **10 ms**: drains commands under `lock(mLockObject)` then
  calls `mFModStudio.update()`. All FMOD calls happen on this thread / under the lock.
- Copy this pattern: never call FMOD from arbitrary threads; marshal onto one audio thread.

---

## 7. Porting notes (non-FMOD engines)

- **Radar pan**: 3 mono variants per cue routed L/C/R, or one sound + a stereo-balance set to
  `{-1,0,+1}`. You do not need FMOD for the radar.
- **3D world sounds**: use the engine's spatial audio (Unity `AudioSource.spatialBlend=1` +
  `rolloffMode`; Wwise/FMOD in RE-Engine games). Min/Max ≈ `1`/`15` units, cull at `13`.
- **Occlusion**: compute `depth` with your raycast (Doc 03 port), then `if depth≥1 mute; else set a
  low-pass cutoff scaled by (1-depth)`. Unity: `AudioLowPassFilter.cutoffFrequency`. This single rule
  is what sells "behind a wall."
- Feed an `indoors` boolean to pick a reverb preset per listener/source if available.
- Keep all audio mutation on one thread.
