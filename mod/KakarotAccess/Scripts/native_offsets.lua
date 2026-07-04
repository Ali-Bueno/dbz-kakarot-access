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

    -- Tips / tutorial book: UAT_UITips (Tips_C). The 2-page book keeps both windows loaded and
    -- only one is the visible FRONT page; which one is a non-reflected member. CONFIRMED by F4:
    -- 0x420 = front window index (0 = Tips_Win00, 1 = Tips_Win01), 0x424 = current page index
    -- (0-based). Both update immediately on navigation (no lag), unlike the widgets' own page
    -- numbers. We read the front window directly, so no two-window heuristic is needed.
    tips = {
        frontWindow = 0x420,     -- int32, 0 = Tips_Win00 is front, 1 = Tips_Win01  (CONFIRMED)
        pageIndex   = 0x424,     -- int32, current page (0-based)  (CONFIRMED, unused for now)
    },

    -- Overworld Party menu: UAT_UIStartParty. The selection is the plain struct field
    -- partySelectData.cursorIndex (FStartpartyPartySelectData). From the CXX header dump the
    -- struct sits at container +0x420 and cursorIndex is its first int32 -> abs 0x420. Tried
    -- via reflection first (host.partySelectData.cursorIndex); this native offset is the
    -- fallback. UNVERIFIED at runtime -> if wrong the reader stays inactive (harmless).
    startParty = {
        cursorIndex = 0x420,     -- int32, selected party row (fallback if reflection fails)
    },

    -- Fishing minigame: UAT_UIMgameFishing (Mgame_Fishing_C). "Press the button inside
    -- the red outline": a cursor sweeps back and forth and must be pressed inside a
    -- random target zone. All live state is non-reflected tail (0x508..0x528).
    -- CONFIRMED by runtime sampling (dump_fishing.txt, 2026-07-03, two sessions):
    --   cursor 0x518 sweeps ~52..124 as a triangle wave (~127 units/s);
    --   zone [0x520, 0x524] is static per attempt (width 37, random position) — both
    --   FAILED presses froze the cursor OUTSIDE [zoneLow, zoneHigh], confirming the rule;
    --   0x51C counts cursor bounces (vs the reflected CursorLapLimit);
    --   0x514 ticks slowly down (timer-like; unused).
    fishing = {
        cursor   = 0x518,    -- float, sweeping cursor position  (CONFIRMED)
        zoneLow  = 0x520,    -- float, red-outline zone lower edge  (CONFIRMED)
        zoneHigh = 0x524,    -- float, red-outline zone upper edge  (CONFIRMED)
        bounces  = 0x51C,    -- int32, cursor bounce count  (CONFIRMED)
        phase    = 0x50C,    -- int32, minigame phase: 1 = hook bar, 2 = closing ring, 0 = idle

        -- Phase 2 lives on the UAT_UIBattleRushSpeedCore widget (visible only then;
        -- its own non-reflected tail, class 0x400). CONFIRMED live 2026-07-03:
        -- ringSize shrinks ~304 units/s from ~730 toward ringTarget=190 (press
        -- moment); a press at 332 (0.47 s early) FAILED, matching the model.
        ringSize   = 0x3E0,  -- float on the core: the closing ring's current size
        ringTarget = 0x3E4,  -- float on the core: the target size (the center button)
    },

    -- Community board (Start_Commu_Brd_C -> UAT_UICommunityBoard): the free cursor over
    -- the emblem sockets. SOLVED STATICALLY (Ghidra 2026-07-04): the game's own socket
    -- hit-test is FUN_1414f2ab0(frame, bCheckEmblem) — see code/decompiled/
    -- manual_1414f2ab0.c. It reads the cursor from the frame's WL_PanelCursor widget at
    -- RenderTransform.Translation (UWidget+0x90/+0x94, raw read — reflection aborts),
    -- and each socket's hit position = panel.PointerCenterOffset (0x3F0/0x3F4, reflected)
    -- + a hidden board position (panel+0x550/0x554) + a hidden frame-side adjust;
    -- socket 1 (the leader pedestal) has its own adjust and range. First hit wins,
    -- iterating LAST -> FIRST. Returns the 1-based WL_PanelTbl index or -1.
    --
    -- HISTORY: host+0x500 once read as "hovered index frozen at 10" — it is really the
    -- board UI MODE: 7 = free-cursor browsing, 9 = emblem detail open, 10 = the Soul
    -- Emblems grid open ON TOP (FUN_1414d59c0 sets it), 16 = tutorial link demo
    -- (FUN_1414d79e0, Cpl021A.. + CommunityLink_41_12). A frozen cursor with mode 10
    -- just means the grid was open.
    commuBoard = {
        -- on the HOST (Start_Commu_Brd_C / UAT_UICommunityBoard)
        mode         = 0x500,   -- int32 board UI mode (7 browse / 9 detail / 10 grid / 16 tutorial)
        subState     = 0x4F3,   -- u8 pick/place sub-state (6 = picking, 7 = placing; from decide)
        hoveredCache = 0x5D8,   -- int32 the game's OWN cached hovered index, written by its
                                -- hover tracker FUN_1414e3170 every tick: 1-based socket, or
                                -- -1 over empty/no-data sockets (it stores hit_test(frame, 1));
                                -- 0x80000001 sentinel right after a board switch
        -- on the cursor WIDGET (frame.WL_PanelCursor, a UCanvasPanel)
        cursorX      = 0x90,    -- float RenderTransform.Translation.X (the game's own source)
        cursorY      = 0x94,    -- float RenderTransform.Translation.Y
        -- on the FRAME (WL_BrdFrame / UAT_UICommunityBoard_PanelFrame), hidden members
        activeCount  = 0x628,   -- int32 number of active sockets this board (hit-test bound)
        hitAdjX      = 0x428,   -- float hit-test adjust X (normal sockets)
        hitAdjY      = 0x42C,   -- float hit-test adjust Y
        hitRange     = 0x420,   -- float hit half-extent (normal sockets, square test)
        leaderAdjX   = 0x430,   -- float adjust X for socket 1 (leader pedestal)
        leaderAdjY   = 0x434,   -- float adjust Y for socket 1
        leaderRange  = 0x424,   -- float hit half-extent for socket 1
        heldEmblem   = 0x7B8,   -- ptr, non-null while an emblem is IN HAND (being placed)
        -- on each SOCKET PANEL (UAT_UICommunityBoard_Panel)
        panelPosX    = 0x3F0,   -- float PointerCenterOffset.X (reflected, read raw)
        panelPosY    = 0x3F4,   -- float PointerCenterOffset.Y
        panelBoardX  = 0x550,   -- float hidden board-layout position X
        panelBoardY  = 0x554,   -- float hidden board-layout position Y
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
