-- char_types.lua — CHARACTER_TYPE (the enum) -> a spoken character name.
--
-- WHY THIS EXISTS. Naming a field character used to fail almost always: `resolve_char_id` asked
-- the game's own resolver (`UAT_BlueprintFunctionLibrary::GetCharacterName`, which returns "" for
-- EVERY id on this build) and then fell back to CPL_NAMES — four hand-verified entries. So the
-- radar could name Goku, Gohan, Yajirobe and Piccolo, and called everyone else by a generic noun.
--
-- The name was in reflection the whole time. `AAT_CharacterBase::CharacterType` (@0x0758, CXX dump
-- AT_enums.hpp:98) is an EnumProperty whose VALUE NAMES are the characters. It is declared on the
-- BASE, so it answers on both branches — `AAT_Character` enemies and `AQuestCharacter` NPCs — and
-- an enum is a VALUE: no pointer hop, no validity call, the cheapest and safest read this engine
-- offers. That matters here: the probe that walked live actors dereferencing handles crashed the
-- game twice on 2026-08-15, and this replaces that whole class of approach.
--
-- Values 1..119 are real names. 120..276 are raw CplNNN codes for later/DLC content — exposed
-- through M.code() so the existing CPL_NAMES path can still try, rather than being invented here.
--
-- RAW IS GENERATED, NOT TYPED. Transcribing 277 enum entries by hand is a silent-typo machine, and
-- a wrong name is worse than no name. Regenerate after a game patch with:
--   awk '/^enum class CHARACTER_TYPE \{/,/^\};/' AT_enums.hpp
-- and rebuild this table from it.

local M = {}

-- value -> the enum's own identifier, verbatim from the header.
M.RAW = {
    [0]="None", [1]="Goku", [2]="Goku_N", [3]="Gohan_A", [4]="Gohan", [5]="Gohan_C", 
        [6]="Gohan_D", [7]="Gohan_Z", [8]="Vegeta", [9]="Vegeta_Z", [10]="Vegeta_B", [11]="Apuru", 
        [12]="Apuru_C02", [13]="Apuru_C03", [14]="Apuru_C04", [15]="Freezer_A", [16]="Freezer_B", [17]="Freezer_C", 
        [18]="Freezer", [19]="Freezer_E", [20]="Freezer_MECHA", [21]="Kuririn", [22]="Yamcha", [23]="Tien", 
        [24]="Chiaotzu", [25]="Piccolo", [26]="Raditz", [27]="Nappa", [28]="Saibamen", [29]="Saibamen_C02", 
        [30]="Saibamen_C03", [31]="Saibamen_C04", [32]="Saibamen_C05", [33]="Cui_C01", [34]="Dodoria_C01", [35]="Zarbon_A_C01", 
        [36]="Zarbon_B_C01", [37]="Recoom", [38]="Burter_C01", [39]="Jeice_C01", [40]="Guldo", [41]="Ginyu", 
        [42]="Trunks", [43]="Trunks_B", [44]="Android19_C01", [45]="Android20", [46]="Android17", [47]="Android18", 
        [48]="Cell_A", [49]="Cell_B", [50]="Cell_C", [51]="CellJr_C01", [52]="Goten", [53]="Dabura", 
        [54]="PuiPui_C01", [55]="Yakon_C01", [56]="Buu_A", [57]="Buu_B", [58]="Buu_C", [59]="Buu_E", 
        [60]="Gotenks", [61]="Vegito", [62]="RRArmyRobot_C01", [63]="RRArmyRobot_C02", [64]="RRArmyRobot_C03", [65]="PirateRobot_C01", 
        [66]="PirateRobot_C02", [67]="PirateRobot_C03", [68]="BobbidiSoldier", [69]="BobbidiSoldier_B", [70]="BobbidiSoldier_A", [71]="BobbidiSoldier_C", 
        [72]="RRBit_C01", [73]="RRBit_C02", [74]="RRBit_C03", [75]="FZBit_C01", [76]="FZBit_C02", [77]="FZBit_C03", 
        [78]="AlienA_C01", [79]="AlienA_C02", [80]="AlienA_C03", [81]="AlienB_C01", [82]="AlienB_C02", [83]="AlienB_C03", 
        [84]="AlienC_C01", [85]="Mira", [86]="Bonyu", [87]="BnBit_C01", [88]="BnBit_C02", [89]="BnBarrier", 
        [90]="PirateBit_C01", [91]="PirateBit_C02", [92]="PirateBit_C03", [93]="Beerus", [94]="Whis", [95]="Freezer_F", 
        [96]="Apuru_L", [97]="Apuru_C02_L", [98]="Apuru_C03_L", [99]="Apuru_C04_L", [100]="AlienA_C01_L", [101]="AlienA_C02_L", 
        [102]="AlienA_C03_L", [103]="AlienB_C01_L", [104]="AlienB_C02_L", [105]="AlienB_C03_L", [106]="Gotenks_B", [107]="Vegito_B", 
        [108]="Gohan_E", [109]="Gohan_F", [110]="Gohan_G", [111]="Trunks_C", [112]="Trunks_D", [113]="Trunks_E", 
        [114]="Android17_B", [115]="Android18_B", [116]="Cell_F", [117]="Babidi_A", [118]="Dabura_B", [119]="Shin", 
        [120]="Cpl027", [121]="Cpl027_B", [122]="Cpl078", [123]="Cpl079", [124]="Cpl003_C", [125]="Cpl003_D", 
        [126]="Cpl005_G", [127]="Cpl084", [128]="Cpl085", [129]="Cpl004_C05", [130]="Cpl086", [131]="Cpl018_B", 
        [132]="Cpl017_B", [133]="Cpl082", [134]="Cpl082_C02", [135]="Cpl082_C03", [136]="Cpl083", [137]="Cpl083_C02", 
        [138]="Cpl083_C03", [139]="Cpl094", [140]="Cpl094_C02", [141]="Cpl094_C03", [142]="Cpl093", [143]="Cpl093_C02", 
        [144]="Cpl093_C03", [145]="Cpl091", [146]="Cpl092", [147]="Cpl082_C04", [148]="Cpl086_C02", [149]="Cpl095", 
        [150]="Cpl096", [151]="Cpl096_C02", [152]="Cpl096_C03", [153]="Cpl097", [154]="Cpl097_C02", [155]="Cpl082_L", 
        [156]="Cpl082_C02_L", [157]="Cpl082_C03_L", [158]="Cpl093_L", [159]="Cpl093_C02_L", [160]="Cpl093_C03_L", [161]="Cpl001C", 
        [162]="Cpl001C_02", [163]="Cpl001D", [164]="Cpl008B", [165]="Cpl011B", [166]="Cpl011C", [167]="Cpl098A", 
        [168]="Cpl099A", [169]="Cpl100A", [170]="Cpl100B", [171]="Cpl100C", [172]="Cpl101A", [173]="Cpl101B", 
        [174]="Cpl102A", [175]="Cpl102B", [176]="Cpl102C", [177]="Cpl102D", [178]="Cpl102E", [179]="Cpl103A", 
        [180]="Cpl103B", [181]="Cpl103C", [182]="Cpl103D", [183]="Cpl103E", [184]="Cpl104A", [185]="Cpl104B", 
        [186]="Cpl075D", [187]="Cpl075E", [188]="Cpl075F", [189]="Cpl059D", [190]="Cpl059E", [191]="Cpl059F", 
        [192]="Cpl056D", [193]="Cpl056E", [194]="Cpl056F", [195]="Cpl058D", [196]="Cpl058E", [197]="Cpl058F", 
        [198]="Cpl001E", [199]="Cpl001F", [200]="Cpl036B", [201]="Cpl003E", [202]="Cpl105A", [203]="Cpl106A", 
        [204]="Cpl106B", [205]="Cpl107A", [206]="Cpl028F", [207]="Cpl034G", [208]="Cpl001G", [209]="Cpl001G_C02", 
        [210]="Cpl076B", [211]="Cpl108A", [212]="Cpl109A", [213]="Cpl110A", [214]="Cpl110B", [215]="Cpl110C", 
        [216]="Cpl110D", [217]="Cpl111A", [218]="Cpl111B", [219]="Cpl111C", [220]="Cpl111D", [221]="Cpl112A", 
        [222]="Cpl112B", [223]="Cpl113A", [224]="Cpl113B", [225]="Cpl113C", [226]="Cpl113D", [227]="Cpl113E", 
        [228]="Npc133A", [229]="Cpl111E", [230]="Cpl111F", [231]="Cpl111G", [232]="Cpl112C", [233]="Cpl112D", 
        [234]="Cpl111H", [235]="Cpl112E", [236]="Cpl111I", [237]="Cpl001H", [238]="Cpl001I", [239]="Cpl001J", 
        [240]="Cpl003F", [241]="Cpl003G", [242]="Cpl003G_C02", [243]="Cpl003H", [244]="Cpl076C", [245]="Cpl076D", 
        [246]="Cpl108B", [247]="Cpl108C", [248]="Cpl011E", [249]="Cpl114A", [250]="Cpl121A", [251]="Cpl123A", 
        [252]="Cpl110E", [253]="Cpl110F", [254]="Cpl110G", [255]="Cpl111J", [256]="Cpl111K", [257]="Cpl111L", 
        [258]="Cpl112F", [259]="Cpl112G", [260]="Cpl113F", [261]="Cpl113G", [262]="Cpl113H", [263]="Cpl113I", 
        [264]="Cpl113J", [265]="Cpl113K", [266]="Cpl113L", [267]="Cpl115A", [268]="Cpl116A", [269]="Cpl117A", 
        [270]="Cpl118A", [271]="Cpl119A", [272]="Cpl120A", [273]="Cpl122A", [274]="Cpl122A_C02", [275]="Cpl122B", 
        [276]="Cpl124A", [277]="CHARACTER_MAX", 
}

-- Base identifier -> spoken name. Spellings follow screen_community.lua's CHAR_TOKENS, which is
-- this mod's existing precedent (and records the decision that character names are PROPER NOUNS
-- and therefore need no localization): Kuririn->Krillin, Apuru->Appule, Freezer->Frieza,
-- Recoom->Recoome, Saibamen->Saibaman, Buu->Majin Buu, CellJr->Cell Jr., PuiPui->Pui Pui.
--
-- DELIBERATELY INCOMPLETE. Entries that are direct translations are mapped; anything genuinely
-- uncertain is left OUT, so it is spoken as its raw identifier. That is CHAR_TOKENS' own rule —
-- "an UNMAPPED token is spoken raw, still identifiable, and it tells us exactly which entry to
-- add" — and it is the same reason CPL_NAMES never grew by guessing. Unmapped on purpose today:
-- RRBit / FZBit / BnBit / PirateBit (what a "Bit" is called in English here is not established)
-- and BnBarrier (an obstacle, not a character).
M.DISPLAY = {
    -- exact matches, tried BEFORE any suffix stripping (the suffix carries meaning here)
    Freezer_MECHA = "Mecha Frieza",

    Goku = "Goku", Gohan = "Gohan", Goten = "Goten", Vegeta = "Vegeta",
    Trunks = "Trunks", Piccolo = "Piccolo", Kuririn = "Krillin", Yamcha = "Yamcha",
    Tien = "Tien", Chiaotzu = "Chiaotzu", Gotenks = "Gotenks", Vegito = "Vegito",
    Raditz = "Raditz", Nappa = "Nappa", Saibamen = "Saibaman",
    Apuru = "Appule", Cui = "Cui", Dodoria = "Dodoria", Zarbon = "Zarbon",
    Freezer = "Frieza", Recoom = "Recoome", Burter = "Burter", Jeice = "Jeice",
    Guldo = "Guldo", Ginyu = "Ginyu",
    Android17 = "Android 17", Android18 = "Android 18",
    Android19 = "Android 19", Android20 = "Android 20",
    Cell = "Cell", CellJr = "Cell Jr.",
    Dabura = "Dabura", Babidi = "Babidi", PuiPui = "Pui Pui", Yakon = "Yakon",
    Buu = "Majin Buu", Mira = "Mira", Bonyu = "Bonyu",
    Beerus = "Beerus", Whis = "Whis",
    -- Shin IS the Supreme Kai; the English release calls him that. Flagged as the one entry here
    -- taken from the series rather than from a table already in this repo.
    Shin = "Supreme Kai",
    -- direct translations of generic-enemy identifiers (RR = Red Ribbon, Bobbidi = Babidi)
    RRArmyRobot = "Red Ribbon robot", PirateRobot = "pirate robot",
    BobbidiSoldier = "Babidi's soldier",
    AlienA = "alien", AlienB = "alien", AlienC = "alien",
}

-- Variant suffixes, stripped repeatedly until a DISPLAY entry matches. Ordered longest-first so
-- `Apuru_C02_L` loses `_L`, then `_C02`, and lands on `Apuru`.
local SUFFIXES = { "_L$", "_C%d%d$", "_%u%u+$", "_%u$" }

-- Spoken name for a CharacterType value, or nil when the enum has no name for it (the caller
-- keeps its generic noun). 0 (None) and CHARACTER_MAX are not characters.
function M.name(v)
    v = tonumber(v)
    if not v or v <= 0 then return nil end
    local raw = M.RAW[v]
    if not raw or raw == "CHARACTER_MAX" then return nil end
    local hit = M.DISPLAY[raw]
    if hit then return hit end
    for _ = 1, #SUFFIXES do          -- bounded: each pass removes at most one suffix
        local before = raw
        for _, pat in ipairs(SUFFIXES) do
            local stripped = raw:gsub(pat, "")
            if stripped ~= raw then raw = stripped break end
        end
        if raw == before then break end
        hit = M.DISPLAY[raw]
        if hit then return hit end
    end
    return nil
end

-- The raw CplNNN/NpcNNN code for the unnamed tail of the enum (values 120+, later/DLC content).
-- Returned so the existing CPL_NAMES/NPC_NAMES path can still try, instead of inventing a name.
function M.code(v)
    v = tonumber(v)
    if not v or v <= 0 then return nil end
    local raw = M.RAW[v]
    if not raw then return nil end
    return raw:match("^(Cpl%d+)") or raw:match("^(Npc%d+)")
end

-- Spoken name for a LIVE character actor, plus the raw code when the enum has no name.
--
-- ONLY CALL THIS ON A CHARACTER-SPECIFIC CLASS (2026-08-18, measured live). The enum is declared
-- on AAT_CharacterBase, so every descendant answers it — but only classes that exist to BE one
-- character ever author it. `AT_Character_cpl004p1c02_BP_C` answered 11 (Apuru), a real value;
-- `QuestCharacterBase_C`, the GENERIC class the game reuses for NPCs, items and quest markers,
-- answered 1 on an ITEM whose UniqueId was "None" — 1 is Goku, and it is simply the Blueprint
-- default nobody overrode. Asking a generic class therefore renames everything to Goku, which is
-- how this shipped broken: the radar called Dodoria at 300 m "Goku". There is no way to tell an
-- authored 1 from a defaulted 1 on the value alone, so the discrimination has to happen at the
-- CALL SITE: nav_tracker only calls this behind an IsA(AT_Character) gate.
--
-- `Core` is passed IN rather than required, so this stays a pure data module with no dependency
-- back into the UI substrate (nav_tracker already holds Core; a require here would be a cycle).
--
-- STRICT gate. `CharacterType` is declared on AAT_CharacterBase, so both branches really do have
-- it — but strict costs only a single tick of fallback when the per-tick property-set budget has
-- not reached this class yet, and every caller below has a working fallback chain. After two
-- crashes on 2026-08-15 caused by reads that were not gated tightly enough, the bounded option
-- wins. Reading an enum is a VALUE read: no pointer hop and no validity call on the result.
function M.of_actor(Core, actor)
    if not Core or actor == nil then return nil, nil end
    local v = Core.member(actor, "CharacterType", true)
    if v == nil then return nil, nil end
    return M.name(v), M.code(v)
end

-- The character's DIALOGUE SPEAKER KEY: `speakerID`, a StrProperty declared on AAT_CharacterBase,
-- so it answers on BOTH branches (AT_Character enemies and QuestCharacter story actors). It is the
-- `UniqueId` plus a one-letter variant suffix — `Cpl019` -> `Cpl019A`, `Npc004` -> `Npc004A`.
--
-- WHY THIS IS THE WHOLE BALL GAME (2026-08-18). `UAT_BlueprintFunctionLibrary::GetCharacterName`
-- has been recorded in this repo since 2026-07-10 as "returns \"\" for EVERY code — safe but
-- useless", and every naming mechanism since (CPL_NAMES, NPC_NAMES, the CHARACTER_TYPE enum) exists
-- to work around it. The function was never useless: it was being called with the BARE code. Fed
-- the speakerID instead, live in game:
--
--   GetCharacterName("Cpl002")  -> ""                                  (the old call)
--   GetCharacterName("Cpl002A") -> "Gohan"
--   GetCharacterName("Npc004A") -> "Bulma"
--   GetCharacterName("Cpl019A") -> "Zarbon"
--   GetCharacterName("Cpl064A") -> "Oficial del Ejercito de Freezer"
--
-- Localized to the player's language (the game had /Game/Message/PLAT_W/es_MX/messageData loaded),
-- covering story NPCs and generic mobs alike, straight from the game's own message table. The one
-- letter was the entire difference. `UEventBlueprintFunctionLibrary::GetMessageFromID` (AT.hpp:41265)
-- returns the identical string, so either call works; GetCharacterName is kept because the resolver
-- and its cache already exist in nav_tracker.
function M.speaker_id(Core, actor)
    if not Core or actor == nil then return nil end
    return Core.name_str(Core.member(actor, "speakerID", true))
end

-- FALLBACK for the ENEMY branch: derive the message-table id from the BLUEPRINT CLASS NAME.
--
-- WHY IT IS NEEDED (2026-08-18, measured live). `speakerID` answers on QuestCharacter but is an
-- EMPTY string on `AAT_Character` — read off `AT_Character_cpl002_B_BP_C`, where both `speakerID`
-- and `CharacterName` came back blank while `CharacterType` read 4. So the localized lookup could
-- never fire on enemies and they fell through to the enum, which is why the Namek mobs were still
-- announced as "alien" instead of "Oficial del Ejercito de Freezer".
--
-- The id is recoverable anyway, because the enemy's generated class name embeds it, and
-- GetCharacterName wants that code plus a one-letter variant. Verified against every class name
-- observed live on Namek:
--
--   AT_Character_cpl019_A_BP_C     -> Cpl019A  (proved "Zarbon")
--   AT_Character_cpl064c01_BP_C    -> Cpl064A  (proved "Oficial del Ejercito de Freezer")
--   AT_Character_cpl065c01_BP_C    -> Cpl065A  (proved "Comando del Ejercito de Freezer")
--   AT_Character_cpl004p1c02_BP_C  -> Cpl004A  (proved "Appule")
--   AT_Character_cpl057Ac01_BP_C   -> Cpl057A
--   AT_Character_cpl002_B_BP_C     -> Cpl002B, then Cpl002A  (Cpl002A is proved "Gohan")
--
-- The letter is taken from the class when it declares one (`_A`, `057A`) and defaults to "A"
-- otherwise; a non-A letter is tried FIRST and then A, because a variant may have no message row
-- of its own. `game_character_name` caches misses, so a wrong candidate costs one call ever.
-- Only a lowercase run follows the digits in the costume forms (`c01`, `p1c02`), so requiring an
-- UPPERCASE letter is what keeps those from being mistaken for a variant.
function M.speaker_ids_from_class(Core, actor)
    if not Core or actor == nil then return nil end
    local cn
    pcall(function() cn = actor:GetClass():GetFName():ToString() end)
    if type(cn) ~= "string" then return nil end
    local digits, tail = cn:match("[Cc]pl(%d+)(.*)")
    if not digits then return nil end
    local base = "Cpl" .. digits
    local letter = tail:match("^_?(%u)")
    if letter and letter ~= "A" then return { base .. letter, base .. "A" } end
    return { base .. "A" }
end

return M
