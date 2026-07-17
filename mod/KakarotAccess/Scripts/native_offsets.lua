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
        fixedSubIndex = 0x4e8,   -- int32, selected row of the FIXED-rows submenus (Community:
                                 -- Board/Emblems — subIndex stays 0 there)  (CONFIRMED via subhunt
                                 -- 2026-07-06: toggled 0<->1 exactly with the two rows)
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

    -- Save / Load data-slot menu: UAT_UIStartSaveLoad. The list is a VIRTUALIZED 3-bar
    -- window (UISaveLoadBar_List), so the widget pool-position is NOT the save's ordinal.
    -- The real selection lives in the non-reflected tail (0x408..0x430). CONFIRMED by runtime
    -- diffing (F4 dev tool, 2026-07-11, whole list of ~11 saves): 0x410 stepped cleanly
    -- 0->1->2->...->10 (the ABSOLUTE selected index); 0x414 = scroll offset (top row of the
    -- window, 0 until the cursor passes row 3, then 1..8); 0x418 = highlighted position WITHIN
    -- the visible window (0,1,2 — saturates at 2). Verified relation: index = scroll + windowPos.
    -- We only need selectedIndex; ordinal shown to the player = selectedIndex + 1.
    saveLoad = {
        selectedIndex = 0x410,   -- int32, ABSOLUTE selected save row (0-based)  (CONFIRMED)
        scrollOffset  = 0x414,   -- int32, top row of the visible window  (CONFIRMED, cross-check)
        windowPos     = 0x418,   -- int32, highlighted bar within the 3-bar window (0..2)  (CONFIRMED)
    },

    -- Item / inventory menu: Start_Item_C -> UAT_UIItemMenu. The item list is a fixed pool and
    -- its whole UI goes STALE on an empty category (row text, detail pane, visible-count all keep
    -- the last item), so there is no fresh "empty" signal in reflection. This non-reflected int32
    -- in the class gap (0x518..0x6A0) is != 0 while the current category HAS items and 0 when it's
    -- EMPTY. CONFIRMED by F4 runtime diff (2026-07-11): a populated category -> empty "Recuperación"
    -- flipped 0x620 from 1 to 0. Read via mem.lua; used by screen_list to announce empty categories.
    itemMenu = {
        hasItems = 0x620,   -- int32, != 0 = current category has items, 0 = empty  (F4-confirmed)
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
                                -- 0x80000001 sentinel right after a board switch.
                                -- FROZEN while an emblem is HELD: the tracker early-outs on
                                -- frame+0x7B8 != 0 (FUN_1414f91e0) and never writes it — in
                                -- placing mode ONLY the hit-test replica is valid (2026-07-16)
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

    -- Soul Emblems grid (AT_UICommunityStart): cursor state lives in the EmbList
    -- widget's non-UPROPERTY gap (UAT_UICommunityStart_EmbList, 0x3D0..0x3F0 between
    -- AnimOutL and PageChangeType). FULLY MAPPED by runtime diffing (gridhunt,
    -- 2026-07-06, two passes — vertical then horizontal): +0x3D0 = column 0..6
    -- (wraps 0->6 moving left), +0x3D4 = row 0..2, and +0x3EC = row*7+col = the
    -- COMPLETE 0-based EmbAry slot index (verified across a row change: col 1,
    -- row 0->1 => 0x3EC jumped 1->8).
    commuGrid = {
        cursorIndex = 0x3EC,   -- int32 on EmbList: full selected EmbAry slot  (CONFIRMED)
        colIndex    = 0x3D0,   -- int32 on EmbList: cursor column 0..6  (CONFIRMED)
        rowIndex    = 0x3D4,   -- int32 on EmbList: cursor row 0..2  (CONFIRMED)
    },

    -- World map UAT_UIMapWorld (BP Map_World_C) — the fast-travel confirm mechanism, recovered
    -- from Ghidra (2026-07-09, code/decompiled/manual_1415c0890.c = confirm core FUN_1415c0890;
    -- FUN_1415cf800 = the selIndex setter; FUN_1415c0e30 = d-pad left/right). InputConfirm and
    -- MouseClickDecide both funnel into the confirm core, which requires state ∈ {6, 0xc} AND
    -- ready != 0 AND selIndex != -1, then opens the "WM_MoveConf" YesNo and warps on Yes.
    -- To fast-travel to a chosen point: (state==0xc) write selIndex, ensure ready, reflection-call
    -- InputConfirm(), answer Yes. NEVER travel to a locked point: the core does NOT re-validate,
    -- so the mod MUST check InfoIcon[idx] unlocked flag (+0x11) != 0 first.
    mapWorld = {
        state        = 0x508,  -- int32: 6=area-select, 0xc=destination-select, 0xe=warping, 9/10=info win
        selIndex     = 0x514,  -- int32: selected InfoIcon index (-1 = none); WRITE this to pick a point
        ready        = 0x51d,  -- u8: confirm-ready gate (must be != 0 for InputConfirm to act)
        -- InfoIcon UPROPERTY TArray<FWorldMapsSymbol> — the ordered destination universe.
        infoIconData = 0x4d0,  -- ptr: array data
        infoIconCount= 0x4d8,  -- int32: array count
        infoIconStride = 0x20, -- bytes per FWorldMapsSymbol entry
        entryIcon    = 0x08,   -- ptr: UAT_UIMapWorldIcon* for this entry (from entry base)
        entryUnlocked= 0x11,   -- u8: unlocked/selectable flag (0 = LOCKED — do not travel)
        entryAreaId  = 0x14,   -- (from entry base) area id
        entryId      = 0x1c,   -- (from entry base) int id
    },

    -- World-map fast-travel cursor: Map_World_Curs_C (the free analog cursor under
    -- BP_ATGameInstance). Its screen position is UWidget RenderTransform.Translation
    -- (UWidget+0x90/+0x94, same member as commuBoard.cursorX/Y). Unlike the community
    -- board — where that member IS the game's authoritative cursor — on the WORLD map it
    -- is only the VISUAL reflection: writing it moves the sprite but NOT the real cursor
    -- (verified 2026-07-09, dump_map_snap: held=true yet fast-travel stayed on the old
    -- point). The authoritative cursor is an internal WORLD/map-space variable (drives both
    -- the map pan and the "¿Ir a X?" hit-test) — NOT yet located. So we do NOT write these;
    -- fast travel is driven through the game's own input (stick injection). Kept as the
    -- documented read offset for the visual cursor and as the starting point for a future
    -- memory-diff hunt of the real cursor variable.
    mapCursor = {
        cursorX = 0x90,   -- float RenderTransform.Translation.X (VISUAL only — see note above)
        cursorY = 0x94,   -- float RenderTransform.Translation.Y
    },

    -- Skill Tree: UAT_UISkillTree (the tree inside Start_Skilltree_C / UAT_UISkillTreeMenu).
    -- Reflection can't tell an ACQUIRED node from a LOCKED one, but the game caches the answer
    -- on each node widget. SOLVED STATICALLY (Ghidra 2026-07-14, code/decompiled/
    -- manual_141672080.c = the post-cursor-move refresh, manual_141664ab0.c = OnInputDecide):
    --   cell = *(int*)(tree + 0x4cc + (row*0x1e + col)*4)     -- 1-based node index, <=0 = empty cell
    --   zorb = ((UAT_UISkilltreeZorb**)(tree + 0x438))[cell-1]
    --   id   = ((FName*)(tree + 0x1538))[cell-1]              -- also cached on the widget at +0x418
    -- and OnInputDecide gates on the node's own state byte (zorb + 0x460), refusing to act on
    -- exactly {2,5,8} (the `0x124 >> state & 1` test) = the already-ACQUIRED nodes. The byte is
    -- written from the game's authoritative tri-state FUN_14145c4b0: node in HaveSkillTreeId -> 2
    -- (acquired), in OpenSkillTreeId -> 1 (purchasable), else 0 (locked).
    skillTree = {
        cursorCol   = 0x15f8,   -- int32, cursor column (RAW grid index)  (CONFIRMED)
        cursorRow   = 0x15fc,   -- int32, cursor row (RAW grid index)  (CONFIRMED)
        maxCol      = 0x15ec,   -- int32, column bound (from the cursor-move impls)
        maxRow      = 0x15f4,   -- int32, row bound
        grid        = 0x4cc,    -- int32[], cell -> 1-based node index (0 = empty)  (CONFIRMED)
        gridCols    = 0x1e,     -- 30 columns per row (the row stride of that grid)  (CONFIRMED)
        zorbData    = 0x438,    -- TArray<UAT_UISkilltreeZorb*> data ptr (node widgets, 0-based)
        nodeIdData  = 0x1538,   -- TArray<FName> data ptr (the tree-node ids, parallel to zorbData)
        nodeIdCount = 0x1540,   -- int32 count
        character   = 0x1584,   -- int32 CHARACTER_TYPE of the tree being shown
        -- on each node widget (UAT_UISkilltreeZorb)
        zorbState   = 0x460,    -- u8 state: {0,1} LOCKED, {2,5,8} ACQUIRED, {3,4,6,7,9,10} OPEN
        zorbNodeId  = 0x418,    -- FName (8 bytes) of this node — cross-check against nodeIdData
    },

    -- Battle pause: UAT_UIXCmnPause.  The selected row index is a non-UPROPERTY member
    -- in the tail 0x438..0x500. Not yet pinned statically (nav is C++-direct, no named
    -- UFunction). ListBarArray is the reflected TArray at 0x3a8 (data) / 0x3b0 (count).
    -- TO FIND: with the game running, diff mem.bytes(pause, 0x438, 0xC8) while moving the
    -- pause cursor and note which int32 steps 0->1->2; set selectedIndex to that offset.
    -- Boot agreement viewer: UAT_UIXcmnAgreement (class size 0x5E8). The doc/page state
    -- sits AFTER the reflected ImageTextureMap (0x508), at 0x5A8..0x5BC. CONFIRMED by
    -- Ghidra (2026-07-17, scripts in code/ghidra/agree_*.java): MouseClickTabRight impl
    -- FUN_1416eee20 does n=*(this+0x5A8)+1, bounds it against *(this+0x5B0), writes the
    -- request to 0x5AC; the deferred commit FUN_1416f0030 -> SetPage FUN_141700200 clamps
    -- and stores 0x5A8 (stable after a flip settles). Texture loader FUN_1416daef0 picks
    -- the asset-name format by *(this+0x5B4): 0="Eula_%02d", 1="Privacypolicy_%02d",
    -- 2|3="Kpi_%02d" (3 = the W180 region variant) — i.e. docId names the document.
    agreement = {
        page      = 0x5a8,   -- int32, current page, 0-based, committed post-flip  (CONFIRMED)
        pageWant  = 0x5ac,   -- int32, requested page during the flip animation  (CONFIRMED)
        pageCount = 0x5b0,   -- int32, page count of the current document  (CONFIRMED)
        docId     = 0x5b4,   -- int32: 0=EULA, 1=privacy policy, 2|3=KPI / data analysis  (CONFIRMED)
    },

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
