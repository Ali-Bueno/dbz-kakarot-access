# PRISM — consuming the prebuilt release (verified, DO NOT re-investigate)

> **Read this when:** you're wiring PRISM into a mod for the first time (any engine) and need the
> concrete facts about the release layout, exports, call sequence and what to ship. One-time-per-mod
> reference — not needed once the adapter is working. For *how the call physically reaches PRISM on
> each engine* (the transport), see [the integration index](README.md).
>
> Checked against **PRISM v0.18.2** by downloading the asset and reading the binary/headers.
> **No need to compile the PRISM repo or use CMake/vcpkg** to consume it: the release is enough.

- **Per-platform releases** on GitHub (`ethindp/prism`, assets of the latest release). For Windows:
  **`prism-windows-x64.zip`** and `prism-windows-arm64.zip`. **There is no x86 (32-bit) build** → if the
  game/host is 32-bit, the PRISM prebuilt is NOT usable (you'd have to compile). Most modern x64 games →
  use `x64`.
- **Stable C API**: `include/prism.h` is wrapped in `extern "C"`, pure C types (`char*`, `float`,
  `size_t`, opaque pointers `PrismContext*`/`PrismBackend*`), `__cdecl` convention, `PRISM_API` macro
  (`__declspec(dllimport/dllexport)`). **No C++ type crosses the boundary** → a DLL built with **MinGW or
  MSVC** can consume `prism.dll` (which is built with MSVC) **with no ABI problems**.
- On **x64 the exports are NOT decorated** (`prism_init`, `prism_backend_speak`, …) → the most robust
  path is **dynamic loading**: `LoadLibrary("prism.dll")` + `GetProcAddress` by name. Zero link config,
  zero import lib, and you control when it loads (key if it's an injected DLL: load outside `DllMain`).
- **Windows zip contents**: `include/prism.h`; `dynamic/release/bin/prism.dll` (~1.1 MB, **the one you
  distribute**) + `tolk.dll` (~525 KB, NVDA/JAWS fallback backend); `dynamic/release/lib/prism.lib`
  (MSVC import lib, optional); a CMake package (target **`prism::prism`**). It also includes `static/`
  with a ~79 MB `prism.lib` → **it's MSVC C++: NOT linkable from MinGW** (MSVC only, yields a single
  self-contained DLL).
- **`prism.dll` runtime dependencies**: only system DLLs + UCRT (`KERNEL32`, `USER32`, `ole32`,
  `OLEAUT32`, `RPCRT4`, `UIAutomationCore`, `api-ms-win-core-*`). **Does NOT depend on
  `vcruntime140`/`msvcp140`** → **no VC++ Redistributable required**. Asian-reader backends
  (`PCTKUSR.dll`, `ZDSRAPI_x64.dll`, Orca/SpeechDispatcher bridges) load **on demand**; for NVDA/JAWS/SAPI
  **you don't have to distribute them**.
- **Minimal sequence**: `prism_config_init` → `prism_init(&cfg)` → `prism_registry_acquire_best(ctx)` →
  `prism_backend_initialize(be)` → `prism_backend_speak(be, txt, interrupt)` → … → `prism_backend_free` +
  `prism_shutdown`. Use `interrupt=true` when switching context; log the chosen backend with
  `prism_registry_name`. Others: `prism_backend_braille`, `prism_backend_is_speaking`, `set_rate/volume/pitch`.
- **Static linking (source-level ports only)**: injected/plugin hosts dynamic-load `prism.dll`, but a mod
  compiled *into* the game (decomp / libultraship PC ports) can link `prism.lib` and call the API directly.
  The static `prism.lib` is **MSVC C++** → MSVC toolchain only (not MinGW). Still copy `prism.dll` (+
  `tolk.dll`) next to the exe as a POST_BUILD step.
- **Version note — use PRISM ≥ v0.18.0** (v0.17.0 is NOT enough, see below). Releases v0.16.2–v0.16.7 had a regression
  ([issue #49](https://github.com/ethindp/prism/issues/49)) where the **NVDA backend's `initialize()`
  falsely succeeded when NVDA wasn't running, shadowing JAWS/other readers** — so `acquire_best` could
  pick a dead backend. **Fixed in v0.17.0** (2026-07-07); `acquire_best` is correct there. Only if you're
  pinned to an affected version, the workaround is to enumerate the registry and pick the highest-priority
  backend reporting `IS_SUPPORTED_AT_RUNTIME` instead of `acquire_best` — and retire that workaround once
  you upgrade to ≥ v0.17.0.
- **Why v0.18.0 and not v0.17.x — CJK text is silently mangled below it.** Every release through
  v0.17.3 bundles a simdutf whose **icelake (AVX-512)** UTF-8→UTF-16 kernel builds lane masks from a
  signed `1 << n` with `n` up to 32 (UB). On a CPU that dispatches to that kernel (Zen 4/5, some
  Intel) multi-byte characters lose the top 4 bits of their codepoint — Chinese/Japanese/Korean come
  out as mixed-script garbage while ASCII is fine, on **every** backend. Fixed in v0.18.0
  ([prism#96](https://github.com/ethindp/prism/issues/96), simdutf PR #1004). Pinned to an older
  build? `SIMDUTF_FORCE_IMPLEMENTATION=haswell` in the environment works around it.
- **Upgrading PRISM means REBUILDING your bridge — `PrismConfig` is not ABI-stable.** It went from
  `{ uint8_t version; }` (v0.16.7) to 8 fields (v0.18.2), and `prism_config_init` returns it **by
  value**: on the x64 MSVC ABI a struct over 8 bytes returns through a hidden pointer, so a bridge
  compiled against the old header calls the new DLL with the wrong convention and corrupts memory.
  Ship the header, the lib and the DLL from the SAME release, and recompile.
- **Deploy**: copy `prism.dll` (+ `tolk.dll`) next to the mod DLL. That's all.
