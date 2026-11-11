-- GENERATED FILE -- DO NOT EDIT BY HAND.
-- Regenerate with tools/item-ids/dump_course_teachers.py after a game update.
-- NPCs that teach a full-course meal recipe, keyed by their talk id (UNpcTalkComponent
-- m_talkParamId): `mnq` by "<C1m13>|<actor code>", `step` by "<D3m09010>|<actor code>" where
-- one mini-quest has several teachers, `quest` by sub-quest prefix. Values are recipe item
-- ids; the spoken name is the game's own Item_<id>_Name. Ids only, no text.
return {
    mnq = {
        -- talk C1m09041_mag0001s -> C1m09041_mag0001s_2_1_END
        ["C1m09|mag0001s"] = { 96003 },
        -- talk C1m13041_mad0001s -> C1m13041_mad0001s_2_5_END
        ["C1m13|mad0001s"] = { 96002 },
        -- talk C1m36031_mad0001s -> C1m36031_mad0001s_2_2_END
        ["C1m36|mad0001s"] = { 96005 },
        -- talk C3m17071_fad0001s -> C3m17071_fad0001s_3_1_END
        ["C3m17|fad0001s"] = { 96010 },
        -- talk C3m42031_fag0001s -> C3m42031_fag0001s_1_5_END
        ["C3m42|fag0001s"] = { 96008 },
        -- talk C3m54051_fag0001s_p1 -> C3m54051_fag0001s_2_6_END
        ["C3m54|fag0001s"] = { 96009 },
        -- talk C3m54073_mad0002s -> C3m54073_mad0001s_1_6_END
        ["C3m54|mad0002s"] = { 96007 },
        -- talk C4m66051_fch0001s_p1 -> C4m66051_fch0001s_2_6_END
        ["C4m66|fch0001s"] = { 96012 },
    },
    step = {
        -- talk D3m09010_mad0001s -> D3m09010_mad0001s_1_5_END
        ["D3m09010|mad0001s"] = { 96002 },
        -- talk D3m09011_mad0001s -> D3m09011_mad0001s_1_3_END
        ["D3m09011|mad0001s"] = { 96003 },
        -- talk D3m09012_mad0001s -> D3m09012_mad0001s_1_6_END
        ["D3m09012|mad0001s"] = { 96005 },
        -- talk D3m09013_mad0001s -> D3m09013_mad0001s_1_4_END
        ["D3m09013|mad0001s"] = { 96007 },
        -- talk D3m09014_mad0001s -> D3m09014_mad0001s_1_6_END
        ["D3m09014|mad0001s"] = { 96009 },
        -- talk D3m09015_mad0001s -> D3m09015_mad0001s_1_4_END
        ["D3m09015|mad0001s"] = { 96010 },
        -- talk D3m09016_mad0001s -> D3m09016_mad0001s_1_4_END
        ["D3m09016|mad0001s"] = { 96012 },
    },
    quest = {
        -- command Sub_Cooking_01_WINDOW
        ["Sub_Cooking_01"] = { 96004 },
        -- command Sub_Cooking_02_WINDOW
        ["Sub_Cooking_02"] = { 96006 },
        -- command Sub_Cooking_03_WINDOW
        ["Sub_Cooking_03"] = { 96014 },
        -- command Sub_Cooking_04_WINDOW
        ["Sub_Cooking_04"] = { 96015 },
        -- reward Sub_Cpl032_01_Reward
        ["Sub_Cpl032_01"] = { 96011 },
        -- command Sub_Cpl041A_01_S080_START
        ["Sub_Cpl041A_01"] = { 96013 },
    },
}
