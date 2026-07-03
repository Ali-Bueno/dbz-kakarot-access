-- Native (non-UPROPERTY) member offsets, recovered from the decompiled game exe
-- with Ghidra. These are the fields UE reflection / UE4SS Lua CANNOT see, so we read
-- them through mem.lua + mem_bridge.dll. See code/hybrid-mod-feasibility.md for how
-- each was found. A game update can shift these -> re-run the Ghidra workflow in
-- code/ and edit this ONE file (no recompile).
--
-- Offsets are byte offsets from the object base (obj:GetAddress()).

return {
    -- Field / overworld Start menu: UAT_UIStartTop.
    -- CONFIRMED: In_Curs impl (Ghidra FUN_1417bb1c0) reads the selected ring index as
    --   idx = *(int*)(this+0x4e4);  item = UIStartTopList[idx]  (bounds-checked 0<=idx<count)
    -- and UIStartTopList is the reflected TArray at 0x428 (data) / 0x430 (count).
    startTop = {
        selectedIndex = 0x4e4,   -- int32, the highlighted ring item  (CONFIRMED)
        subIndex      = 0x4ec,   -- int32, selected row of an OPEN submenu (System/Story)  (CONFIRMED
                                 -- via F4: stepped 0->1->2 in the System submenu; sibling of 0x4e4)
        depthFlag     = 0x4dc,   -- int32 nav-depth flag  (CONFIRMED via labeled F4 snapshot):
                                 --   1 = browsing the RING (top level); 0 = INSIDE a submenu.
                                 -- The reliable ring-vs-submenu discriminator (the _Sub arrays and
                                 -- on_screen both lie — they retain/mirror stale items on the ring).
        listData      = 0x428,   -- TArray<UAT_UIStartTopList*> data ptr (reflected, cross-check)
        listCount     = 0x430,   -- int32 count
    },

    -- Each main-menu list item (UAT_UIStartTopList) carries its entry id as a non-reflected byte.
    -- CONFIRMED: FUN_1416bca00 returns *(byte*)(item + 0x404) for the selected item, and the value
    -- is a START_TOP_LIST_ID (ITEM=2, PARTY=3, CHARACTER=5, DLC=14, ...) — NOT EXCmnHeaderFontType.
    -- Maps to a name via I18n.startlist(id). (Items are image-only, so this is the ONLY per-item
    -- name source; the header shows a fixed "Main Menu".) Verified vs a screenshot.
    startTopList = {
        sectionId = 0x404,       -- u8, START_TOP_LIST_ID of this menu item  (CONFIRMED)
    },

    -- Field menu header section: UAT_UIXCmnHeader (the section label shown at top).
    -- CONFIRMED: SetFontType impl (Ghidra FUN_1416f7bd0). FontType arg = section id
    -- (indexes ~59 Lang_* labels). It stores a mode flag + the resolved label FString:
    header = {
        modeFlag  = 0x400,   -- u8: 0 = normal, 1 = "Commu" board mode  (from SetFontType)
        -- CONFIRMED at runtime (field-scan): the readable label FString lives at 0x428. Its
        -- content is the section's asset path, e.g. "/Game/Art/UI_L10N/en_US/.../Lang_MainMenu";
        -- the "Lang_<Token>" tail names the section (mapped via I18n.section).
        label     = 0x428,   -- FString: active section label asset  (CONFIRMED)
        labelAlt  = 0x460,   -- FString: alternate slot seen in SetFontType (fallback)
    },

    -- Overworld Party menu: UAT_UIStartParty. The selection is the plain struct field
    -- partySelectData.cursorIndex (FStartpartyPartySelectData). From the CXX header dump the
    -- struct sits at container +0x420 and cursorIndex is its first int32 -> abs 0x420. Tried
    -- via reflection first (host.partySelectData.cursorIndex); this native offset is the
    -- fallback. UNVERIFIED at runtime -> if wrong the reader stays inactive (harmless).
    startParty = {
        cursorIndex = 0x420,     -- int32, selected party row (fallback if reflection fails)
    },

    -- Battle pause: UAT_UIXCmnPause.  The selected row index is a non-UPROPERTY member
    -- in the tail 0x438..0x500. Not yet pinned statically (nav is C++-direct, no named
    -- UFunction). ListBarArray is the reflected TArray at 0x3a8 (data) / 0x3b0 (count).
    -- TO FIND: with the game running, diff mem.bytes(pause, 0x438, 0xC8) while moving the
    -- pause cursor and note which int32 steps 0->1->2; set selectedIndex to that offset.
    pause = {
        -- CONFIRMED by runtime diffing (F4 dev tool): int32 at 0x43C steps 0->1->2 and
        -- wraps as you move the battle-pause cursor (3 rows). In the hidden tail 0x438..0x500.
        selectedIndex = 0x43c,   -- int32, selected pause row  (CONFIRMED)
        listData      = 0x3a8,   -- TArray<UAT_UIGameoverBar*> data ptr (reflected)
        listCount     = 0x3b0,   -- int32 count
        tailScanFrom  = 0x438,   -- diff window start (for the F4 dev tool)
        tailScanLen   = 0xC8,    -- diff window length (0x438..0x500)
    },
}
