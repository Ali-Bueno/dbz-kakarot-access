# 09 — Speech / Screen-Reader Output Layer

Source: `GameEngine/Ian/SpeechContext.cs`, `Translate/Ian/TranslateSO.cs` + `TranslateAPI.cs`,
`Core/Ian/DiagSO.cs`. This is the channel that turns "entering Armory", scan results, examine text,
and beacon readouts into actual screen-reader / TTS output.

---

## 1. Call chain (top to bottom)

```
gameplay code
  TranslateSO.TSay("entering {0}", region.FriendlyName)   // localized, with substitutions
      -> TranslateAPI.TSay(base, args)
          -> Diag.Write(GetTranslatedText(ts))            // Diag.Write is the single sink
              -> SpeechContext.InnerSpeakAsync(text)      // routed to the active output backend
DiagSO.Say(text)        // NON-localized convenience; in many builds Diag.Write == log only
```

- **`TranslateSO.TSay` / `TranslateSO._`** is the localized path (looks up a base string + fills
  `{0},{1}…`). Use it for anything player-facing.
- **`DiagSO.Say/DSay/ESay`** are protected helpers on the `DiagSO` base; in the decompiled build they
  forward to `Diag.Write` (diagnostic/log). Player-facing speech goes through the `Translate` layer and
  `SpeechContext`.

---

## 2. `SpeechContext` — the backend router

```csharp
public sealed class SpeechContext : GESO {
    public Action<string> InterceptSpeech;   // test/override hook: if set, receives text and returns
    public MySAPI SAPI { get; }              // Windows SAPI5 TTS
    private readonly BlockingCollection<Action> mCommands;   // background speech queue

    public void SpeakAsync(string text)        => mCommands.Add(() => InnerSpeakAsync(text));
    public void SpeakLowPriority(string text)  => mCommands.Add(() => InnerSpeakAsync(text));
    public void SAPICancelAndSay(string text)  => mCommands.Add(() => { SAPI.CancelAll(); if(!text.IsBlank()){ SAPI.SpeakAsync(text); SpeechLog.Log(text);} });
    public void CancelAllSpeech()              => mCommands.Add(() => { /* per-mode interrupt, below */ });
}

private void InnerSpeakAsyncWithoutLogging(string text) {
    if ((G.GameConfig?.NoSpeech ?? false) || text.IsBlank()) return;
    if (InterceptSpeech != null) { InterceptSpeech(text); return; }   // hook wins
    SpeechMode mode = m_world.UserSettings.SpeechMode;
    switch ((int)mode) {
        case 3: break;                          // Silent
        case 2: SetFormText(text); break;       // show in window/form text
        case 1: Tolk.Output(text, false); break;// Screen reader (NVDA/JAWS/Narrator) via Tolk
        default: SAPI.SpeakAsync(text); break;  // 0: SAPI TTS
    }
}
```

### SpeechMode

| Mode | Backend | Notes |
|------|---------|-------|
| 0 | **SAPI5** TTS (`MySAPI` over SpVoice COM) | default self-voicing |
| 1 | **Tolk** → NVDA / JAWS / Narrator | the screen-reader path |
| 2 | Form/window text | visual fallback |
| 3 | Silent | speech off |

### Interruption (`CancelAllSpeech`) — per mode

```csharp
if (mode == 2 && FormIsFocused) Platform.SendBogusCancelSpeechKey();
else if (mode == 1 && FormIsFocused) Tolk.Silence();     // screen-reader stop
else SAPI.CancelAll();                                    // TTS stop
```

`HandleRegion` (Doc 04 §7) calls `CancelAllSpeech()` before announcing a new region so the latest
area interrupts whatever was being read — the right UX for navigation.

### Threading

A background thread loops on `mCommands.TryTake(out item, 10ms)` and executes each queued speech
action (`Perf.TimeIt(... item)`). So speech never blocks the game loop; ordering is FIFO with
`CancelAllSpeech` enqueued like any other command (it cancels whatever the backend is currently
uttering when it runs).

---

## 3. Relevance to your stack: Tolk vs PRISM

The game uses **Tolk** (`Tolk.Output(text, interrupt)`, `Tolk.Silence()`) for the screen-reader path —
exactly the legacy .NET pattern. Your projects standardize on **PRISM** for native work and keep Tolk
only as a fallback for legacy .NET/BepInEx mods. The mapping is 1:1:

| AHC (Tolk) | PRISM equivalent |
|------------|------------------|
| `Tolk.Output(text, false)` | `prism_backend_speak(be, text, /*interrupt=*/false)` |
| `Tolk.Output(text, true)` / then speak | `prism_backend_speak(be, text, /*interrupt=*/true)` |
| `Tolk.Silence()` | speak empty with interrupt, or backend stop |
| SpeechMode routing | `prism_registry_acquire_best` picks NVDA/JAWS/SAPI/VoiceOver/etc. automatically |

So when porting AHC-style announcements into your mods: replace the `SpeechContext` backend switch with
a single PRISM call (`interrupt=true` on context changes like region/zone, `interrupt=false` for
additive info), or Tolk via `DavyKager` in legacy .NET mods.

---

## 4. Design rules worth copying

- **One sink, many backends.** All speech funnels through a single method that picks the backend; this
  is what lets a mod swap Tolk↔PRISM↔SAPI in one place.
- **Async queue + explicit interrupt.** Don't block the game; expose `Speak(text, interrupt)` and call
  `interrupt=true` exactly on context changes (new region/zone/menu), `false` for incremental info.
- **Localized templates** (`TSay("entering {0}", name)`) keep strings translatable and out of logic.
- **Diff-gating lives upstream** (Doc 05): the speech layer just speaks; the *decision* not to repeat
  unchanged info is made by the navigation code.
