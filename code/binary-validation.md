# Binary validation — AT-Win64-Shipping.exe

- File size: **99,909,192 bytes**
- Machine: **x86-64 (AMD64)**
- PE optional-header magic: PE32+ (64-bit)
- Image base: **0x140000000**
- Linker version (opt header): **14.13**
- COFF timestamp (raw): 1764349407 (0x6929d5df)
- Section count: 9

## ⚠️ DRM: Steam Stub (SteamStub v3) — the on-disk `.text` is ENCRYPTED

The binary is wrapped by **Steam DRM (SteamStub)**. Evidence:

- A **`.bind` section** exists (0x60000000 = EXEC|READ) — the canonical SteamStub marker.
- The **AddressOfEntryPoint (RVA 0x65d3310) lands inside `.bind`**, not `.text` — i.e. execution
  starts in the DRM stub, which decrypts the real code at runtime and jumps to the true OEP.
- **`.text` entropy = 8.000 (maximum)** → the entire code section is encrypted on disk.
- The **Rich header is stripped** by the wrapper (that's why it wasn't found), and the section
  raw-size table read as zeros initially — both consistent with SteamStub reprocessing the PE.
- `.rdata` entropy is normal (4.44) and **NOT encrypted** → RTTI strings (`.?AV`×901),
  `"UE4+Release-4.21"`, and read-only data are all readable. Only executable **code** is hidden.

**Implication for Ghidra:** decompiling the file as-is would produce noise for every function
(the bytes are ciphertext). **The DRM must be removed / the code decrypted before analysis.**
Two routes (see next step):
1. **Steamless** (static SteamStub unpacker) → writes an `.unpacked.exe` with a decrypted `.text`.
2. **Live memory dump** → run the game (SteamStub decrypts `.text` in place), read the `.text`
   region out of the process with `ReadProcessMemory`, and overlay it onto a copy of the exe.
   No external tool, but needs the game running.

Toolset confirmed MSVC (linker 14.13 ⇒ VS2017 v141) with **RTTI present** → once decrypted, enable
Ghidra's **MSVC RTTI analyzer + Microsoft demangler** to recover real class/vtable names.

## MSVC RTTI presence probe
- `.?AV` (class type descriptors): **901** occurrences
- `.?AU` (struct type descriptors): **28** occurrences
- `.?AVtype_info@@`: 1 occurrence(s)
- Verdict: RTTI is PRESENT — vtables/classes will demangle to real UE names.

## Sections
| Name | VirtAddr | VirtSize | RawSize | Flags |
|---|---|---|---|---|
| .text | 0x600 | 0x38ba600 | 0x0 | 0x60000020 |
| .rdata | 0x38bac00 | 0x1b29000 | 0x0 | 0x40000040 |
| .data | 0x53e3c00 | 0x532a00 | 0x0 | 0xc0000040 |
| .pdata | 0x5916600 | 0x362a00 | 0x0 | 0x40000040 |
| .rodata | 0x5c79000 | 0x1200 | 0x0 | 0x40000040 |
| _RDATA | 0x5c7a200 | 0xa000 | 0x0 | 0x40000040 |
| .rsrc | 0x5c84200 | 0x4d200 | 0x0 | 0x40000040 |
| .reloc | 0x5cd1400 | 0x23e600 | 0x0 | 0x42000040 |
| .bind | 0x5f0fa00 | 0x38448 | 0x0 | 0x60000000 |
