# dbz-kakarot-status-level

> DBZ Kakarot character LEVEL solved: StatusInstance+0x1C int32 (reflected hops AttributeComponent@0x8E8→StatusInstance@0x100); SI+0x394 float = current HP; GetPowerCompareRank reflected shortcut


Character live LEVEL (the "Lv N" by enemy names) — SOLVED 2026-07-17 by **live getter-chain
decoding** (a technique worth reusing on this RTTI-stripped exe).

- **ENEMY level = `ATEnemyStatus + 0x390` (int32).** Read path in the mod: reflected hops
  `AAT_Character.AttributeComponent` (@0x8E8) → `.StatusInstance` (@0x100) → `Mem.i32(si,
  0x390)` (`native_offsets.lua: statusInstance.level`). ENEMY instances only — on
  `ATPlayerStatusInstance` slot 0x390 is a POINTER (player level = `[[si+0x390]+0x328]`,
  different leaf). `SI+0x394` (float) = enemy current HP. Consumers: `enemy_level()` →
  enemy-cache `disp` → proximity alert + R3 picker ("Soldado, nivel 3").
- **The winning technique** (after static Ghidra hit the no-RTTI wall — ~200 candidate
  vtables — and a value-pattern guess failed): `Nav.dump_levels` (Ctrl+Shift+F5) reads the
  component's LIVE vtable (`at_ptr(obj,0)`, slot 0x3E8 per Ghidra's chain proof), gets the
  exe base (module_base, else MZ-scan down in 64K steps — needed because hot reload restores
  the BOOT snapshot of mem.lua, so new wrappers don't exist until full restart), dumps the
  getter's opcode bytes, and `follow_getter` decodes forwarder idiom
  `mov rcx,[rcx+i32]; mov rax,[rcx]; jmp [rax+i32]` + leaf loads to walk to the final member
  offset automatically, printing "LEVEL RESOLVED holder/member/value". One capture round.
- **Falsification lesson:** `+0x1C` "varied like a level" across characters but was a
  per-character id (read 94 on an enemy a level-6 player beat easily). Only matching the
  DISPLAYED value confirms an offset.
- Ghidra evidence files: `code/decompiled/manual_140f8aba0.c` (level served virtually,
  vtable[0x3E8], floor 1), `_ufunc_GetPowerCompareRank.c` (reflected 0–6 stronger/weaker
  rank on `UCharacterAuraComponent` — unused bonus for fight-or-flee), `_range_140f3c000.txt`
  (StatusInstance stat-getter slots 0x428–0x5c8 if HP/ATK ever wanted).
- **Bonus, unused:** `SI+0x394` (float) = current HP (fell 500→287 while an enemy was hit);
  `code/decompiled/_range_140f3c000.txt` maps the StatusInstance stat-getter vtable slots
  (0x428–0x5c8) if HP/ATK are ever wanted. And a REFLECTED shortcut exists for fight-or-flee:
  `GetPowerCompareRank` on `UCharacterAuraComponent` returns the 0–6 relative-strength rank
  (the aura). Not wired.
- **Consumers:** `nav_tracker.enemy_level()` → enemy cache `disp` label ("Soldado, nivel 12") →
  proximity alert + R3 radar picker. Guard band 1..999 (getter floor 1 per Ghidra; top = misread
  filter only).

Related: [dbz-kakarot-radar-picker](dbz-kakarot-radar-picker.md), [dbz-kakarot-decompilation-setup](dbz-kakarot-decompilation-setup.md).
