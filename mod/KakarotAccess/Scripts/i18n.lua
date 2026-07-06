-- Localization for the mod's OWN spoken strings.
--
-- Game-authored text (menu labels, action names, tooltips) is ALREADY localized and
-- read live from the game's widgets, so it is never translated here — only the mod's
-- own connective/descriptor words (button names, "controller:" label, screen names…).
--
-- The active language follows the GAME's language, NOT UE culture (which reports "en"
-- regardless). It is detected from the loaded message table path:
--   CFGameInstance.MessageManager.DataTable = .../Message/PLAT_*/<lang>/messageData
-- e.g. ".../Message/PLAT_W/es_ES/messageData" → base language "es". Detected once on
-- the game thread and cached (call I18n.refresh() to re-detect, e.g. after a language
-- change). Unknown/undetected → DEFAULT.

local I18n = {}

local DEFAULT = "en"
local lang = nil -- cached base code ("es", "en", …) once resolved

-- ---- language detection ----------------------------------------------------

local function detect()
    local gi = FindFirstOf("BP_ATGameInstance_C") or FindFirstOf("GameInstance")
    if not gi or not gi:IsValid() then return nil end
    local mm
    pcall(function() mm = gi.MessageManager end)
    if not mm or not mm:IsValid() then return nil end
    local path
    pcall(function()
        local dt = mm.DataTable
        if dt and dt:IsValid() then path = dt:GetFullName() end
    end)
    -- ".../Message/PLAT_W/es_ES/messageData…" → "es" (es_ES and es_MX both → es).
    return path and path:match("/Message/[%w_]+/(%a%a)_%u%u") or nil
end

-- Active base language (cached; retries detection until the GameInstance is ready).
function I18n.language()
    if not lang then lang = detect() end
    return lang or DEFAULT
end

-- Drop the cached language so the next lookup re-detects (call after a language change).
function I18n.refresh() lang = nil end

-- ---- string tables ---------------------------------------------------------
-- Keys are stable ids; add a language by adding a table. Missing key → English → key.

local S = {
    es = {
        -- Controller buttons, by canonical (KeyConfig/Xbox-style) token.
        buttons = {
            A = "botón A", B = "botón B", X = "botón X", Y = "botón Y",
            LB = "botón LB", RB = "botón RB",
            LT = "gatillo LT", RT = "gatillo RT",
            L3 = "clic del stick izquierdo", R3 = "clic del stick derecho",
            Start = "botón Start", Back = "botón Select",
            Up = "cruceta arriba", Down = "cruceta abajo",
            Left = "cruceta izquierda", Right = "cruceta derecha",
        },
        button_fallback = "botón %s",
        controller_prefix = "mando: ",
        -- Keyhelp-bar glyphs, by texture token.
        keyhelp = {
            Btn_L1 = "L1", Btn_R1 = "R1", Btn_L2 = "L2", Btn_R2 = "R2",
            Btn_L3 = "L3", Btn_R3 = "R3",
            Btn_Options = "Options", Btn_Pad = "panel táctil",
            Btn_Key_Ud = "arriba y abajo", Btn_Key_Lr = "izquierda y derecha",
            Btn_Key_All = "dirección",
        },
        combo_join = " más ",
        no_keyhelp = "No hay ayuda de botones",
        save_changes = "Guardar cambios",
        main_menu = "Menú principal",
        shop = "Tienda",
        training = "Super ataques",
        ki_required_fmt = "Ki necesario %s",
        paused = "Pausa",
        page = "página %s de %s",
        pos = "%d de %d",
        not_acquired = "No conseguido",
        new_label = "Nuevo",
        commu_lv = "nivel de comunidad %s",
        lvl = "nivel %s",
        empty_socket = "Hueco vacío",
        leader = "líder",
        placed = "Colocados:",
        board_hint = "%d huecos. Pulsa aceptar en un hueco vacío para abrir Emblemas de alma y colocar uno.",
        board_free = "libre",
        board_holding = "Emblema en mano. Mueve el cursor a un hueco y acepta para colocarlo",
        board_socket = "hueco %d de %d",
        reader_on = "Lector de menús activado",
        reader_off = "Lector de menús desactivado",
        -- Navigation radar (nav_tracker.lua).
        nav_on = "Radar de misión activado",
        nav_off = "Radar de misión desactivado",
        nav_tracking = "Rastreando %s, %d metros",
        nav_main = "misión principal",
        nav_sub = "misión secundaria",
        nav_other = "objetivo",
        nav_arrived = "Objetivo alcanzado",
        nav_arrived_pickup = "Has llegado. Pulsa el botón de acción para interactuar",
        nav_no_target = "Sin objetivo que rastrear",
        nav_route_on = "Guía por ruta activada",
        nav_route_off = "Guía por ruta desactivada",
        nav_around = "rodeando obstáculo",
        nav_meters = "%d metros",
        nav_clock = "a las %d",
        nav_up = "arriba",
        nav_down = "abajo",
        nav_ahead = "adelante",
        nav_behind = "detrás",
        nav_left = "izquierda",
        nav_right = "derecha",
        nav_companion = "compañero %d de %d",
        nav_no_companion = "Sin compañeros cerca",
        nav_quest_mode = "Objetivo de misión",
        -- Hold-R2 radar target picker (radar_menu.lua). Categories (L1/R1) + item nouns.
        radar_cat_quests = "Misiones",
        radar_cat_npc = "Personajes",
        radar_cat_fishing = "Pesca",
        radar_cat_gathering = "Recolección",
        radar_cat_shops = "Tiendas",
        radar_cat_minigames = "Minijuegos",
        radar_cat_dragonball = "Bolas de Dragón",
        radar_cat_collectibles = "Coleccionables",
        radar_cat_sites = "Sitios",
        radar_cat_enemies = "Enemigos",
        radar_cat_other = "Otros",
        cat_memory = "recuerdo",
        cat_item = "objeto",
        cat_npc = "personaje",
        cat_fishing = "zona de pesca",
        cat_collect = "punto de recolección",
        cat_hunt = "presa",
        cat_ore = "mineral",
        cat_dragonball = "Bola de Dragón",
        cat_food_shop = "tienda de comida",
        cat_cooking_shop = "cocina",
        cat_material_shop = "tienda de materiales",
        cat_restaurant = "restaurante",
        cat_info_shop = "tienda de información",
        -- interactable sites (EMapIcon, see nav_tracker ICON_NOUN)
        cat_bonfire = "hoguera de cocina",
        cat_trainroom = "sala de entrenamiento",
        cat_practice = "entrenamiento",
        cat_practice_battle = "combate de práctica",
        cat_turtleschool = "Escuela Tortuga",
        cat_timemachine = "máquina del tiempo",
        cat_develop = "laboratorio de desarrollo",
        cat_medical = "enfermería",
        cat_replay = "repetición de historia",
        cat_skyplatform = "plataforma celeste",
        cat_submarine = "submarino",
        cat_windroad = "túnel de viento",
        cat_portal = "salida de zona",
        -- field enemies
        cat_enemy = "enemigo",
        cat_enemy_quest = "enemigo de misión",
        cat_enemy_boss = "enemigo jefe",
        cat_enemy_base = "base enemiga",
        cat_enemy_airship = "aeronave enemiga",
        cat_enemy_group = "grupo de enemigos",
        cat_animal = "animal",
        nav_aiming = "apuntando a",
        nav_on_target = "en la mira",
        nav_stealth_front = "de frente, te puede ver",
        nav_stealth_side = "de lado",
        nav_stealth_back = "por detrás",
        radar_nothing = "No hay nada que rastrear cerca",
        radar_empty_cat = "Categoría vacía",
        radar_menu_off = "Menú de rastreo no disponible",
        radar_chain_done = "No quedan objetos en la categoría",
        nav_stopped = "Rastreo detenido",
        list_empty = "Lista vacía",
        map_empty = "terreno vacío",
        hp_self_fmt = "vida %d por ciento",
        hp_enemy_fmt = "%s, %d por ciento",
        shop_buy_fmt = "comprar: %s",
        shop_have_fmt = "tienes: %s",
        results_total = "Total",
        fish_press = "pulsa %s",
        fish_mash = "machaca %s",
        fish_button = "el botón",
        fish_buttons = "%s, luego %s",
        fish_hooked = "¡Enganchado!",
        -- Overworld menu section names, keyed by EXCmnHeaderFontType enum value (the shared
        -- UAT_UIXCmnHeader shows these as image fonts, so the mod supplies the spoken name).
        header = {
            [0] = "Comunidad", [1] = "Tablón de comunidad", [2] = "Objetos",
            [3] = "Opciones", [4] = "Guardar", [5] = "Emblemas de alma", [6] = "Resultado",
            [7] = "Menú principal", [8] = "Mapa del mundo", [9] = "Personaje",
            [10] = "Personalizar", [11] = "Desarrollo", [12] = "Mejoras", [13] = "Cocina",
            [14] = "Árbol de habilidades", [15] = "Personalizar habilidades", [16] = "Misiones",
            [17] = "Tutorial", [18] = "Cargar", [19] = "Dragon Balls", [20] = "Tienda",
            [21] = "Mapa de zona", [22] = "Carrera", [23] = "Béisbol", [24] = "Grupo",
            [25] = "Misiones de comunidad", [26] = "Sistema", [27] = "Info de tienda",
            [28] = "Misiones pasadas", [29] = "Hora de comer", [30] = "Tienda de comida",
            [31] = "Tienda de materiales", [32] = "Intercambio de piezas", [33] = "Aprendizaje",
            [34] = "Entrenamiento", [35] = "DLC", [37] = "Viaje rápido",
        },
        -- Overworld main-menu list entries, keyed by START_TOP_LIST_ID (the item's 0x404 byte).
        startlist = {
            [0] = "Comunidad", [1] = "Dragon Balls", [2] = "Objetos", [3] = "Grupo",
            -- 7/8: the game's own Spanish labels (screenshot 2026-07-06)
            [4] = "Historia", [5] = "Personajes", [6] = "Sistema", [7] = "Tablero de la comunidad",
            [8] = "Emblemas de alma", [9] = "Guardar", [10] = "Cargar", [11] = "Opciones",
            [12] = "Tutorial", [13] = "Pantalla de título", [14] = "Contenido descargable",
        },
    },
    en = {
        buttons = {
            A = "A button", B = "B button", X = "X button", Y = "Y button",
            LB = "LB button", RB = "RB button",
            LT = "LT trigger", RT = "RT trigger",
            L3 = "left stick click", R3 = "right stick click",
            Start = "Start button", Back = "Select button",
            Up = "d-pad up", Down = "d-pad down",
            Left = "d-pad left", Right = "d-pad right",
        },
        button_fallback = "%s button",
        controller_prefix = "controller: ",
        keyhelp = {
            Btn_L1 = "L1", Btn_R1 = "R1", Btn_L2 = "L2", Btn_R2 = "R2",
            Btn_L3 = "L3", Btn_R3 = "R3",
            Btn_Options = "Options", Btn_Pad = "touchpad",
            Btn_Key_Ud = "up and down", Btn_Key_Lr = "left and right",
            Btn_Key_All = "direction",
        },
        combo_join = " plus ",
        no_keyhelp = "No button help",
        save_changes = "Save changes",
        main_menu = "Main menu",
        shop = "Shop",
        training = "Super Attacks",
        ki_required_fmt = "Ki required %s",
        paused = "Paused",
        page = "page %s of %s",
        pos = "%d of %d",
        not_acquired = "Not acquired",
        new_label = "New",
        commu_lv = "community level %s",
        lvl = "level %s",
        empty_socket = "Empty socket",
        leader = "leader",
        placed = "Placed:",
        board_hint = "%d sockets. Press confirm on an empty socket to open Soul Emblems and set one.",
        board_free = "free",
        board_holding = "Emblem in hand. Move the cursor to a socket and confirm to place it",
        board_socket = "socket %d of %d",
        reader_on = "Menu reader on",
        reader_off = "Menu reader off",
        -- Navigation radar (nav_tracker.lua).
        nav_on = "Quest radar on",
        nav_off = "Quest radar off",
        nav_tracking = "Tracking %s, %d meters",
        nav_main = "main quest",
        nav_sub = "sub quest",
        nav_other = "objective",
        nav_arrived = "Objective reached",
        nav_arrived_pickup = "You're here. Press the action button to interact",
        nav_no_target = "No objective to track",
        nav_route_on = "Route guidance on",
        nav_route_off = "Route guidance off",
        nav_around = "going around obstacle",
        nav_meters = "%d meters",
        nav_clock = "at %d o'clock",
        nav_up = "above",
        nav_down = "below",
        nav_ahead = "ahead",
        nav_behind = "behind",
        nav_left = "left",
        nav_right = "right",
        nav_companion = "companion %d of %d",
        nav_no_companion = "No companions nearby",
        nav_quest_mode = "Quest objective",
        radar_cat_quests = "Quests",
        radar_cat_npc = "Characters",
        radar_cat_fishing = "Fishing",
        radar_cat_gathering = "Gathering",
        radar_cat_shops = "Shops",
        radar_cat_minigames = "Minigames",
        radar_cat_dragonball = "Dragon Balls",
        radar_cat_collectibles = "Collectibles",
        radar_cat_sites = "Sites",
        radar_cat_enemies = "Enemies",
        radar_cat_other = "Other",
        cat_memory = "memory",
        cat_item = "item",
        cat_npc = "character",
        cat_fishing = "fishing spot",
        cat_collect = "gathering point",
        cat_hunt = "prey",
        cat_ore = "ore",
        cat_dragonball = "Dragon Ball",
        cat_food_shop = "food shop",
        cat_cooking_shop = "cooking shop",
        cat_material_shop = "material shop",
        cat_restaurant = "restaurant",
        cat_info_shop = "information store",
        -- interactable sites (EMapIcon, see nav_tracker ICON_NOUN)
        cat_bonfire = "cooking campfire",
        cat_trainroom = "training room",
        cat_practice = "training ground",
        cat_practice_battle = "practice battle",
        cat_turtleschool = "Turtle School",
        cat_timemachine = "time machine",
        cat_develop = "development lab",
        cat_medical = "medical room",
        cat_replay = "story replay",
        cat_skyplatform = "sky platform",
        cat_submarine = "submarine",
        cat_windroad = "wind tunnel",
        cat_portal = "area exit",
        -- field enemies
        cat_enemy = "enemy",
        cat_enemy_quest = "quest enemy",
        cat_enemy_boss = "enemy boss",
        cat_enemy_base = "enemy base",
        cat_enemy_airship = "enemy airship",
        cat_enemy_group = "enemy group",
        cat_animal = "animal",
        nav_aiming = "aiming at",
        nav_on_target = "on target",
        nav_stealth_front = "in front, it can see you",
        nav_stealth_side = "to its side",
        nav_stealth_back = "behind it",
        radar_nothing = "Nothing to track nearby",
        radar_empty_cat = "Empty category",
        radar_menu_off = "Radar menu unavailable",
        radar_chain_done = "No items left in this category",
        nav_stopped = "Tracking stopped",
        list_empty = "Empty list",
        map_empty = "empty terrain",
        hp_self_fmt = "health %d percent",
        hp_enemy_fmt = "%s, %d percent",
        shop_buy_fmt = "buy: %s",
        shop_have_fmt = "owned: %s",
        results_total = "Total",
        fish_press = "press %s",
        fish_mash = "mash %s",
        fish_button = "the button",
        fish_buttons = "%s, then %s",
        fish_hooked = "Hooked!",
        header = {
            [0] = "Community", [1] = "Community board", [2] = "Items",
            [3] = "Options", [4] = "Save", [5] = "Soul Emblems", [6] = "Result",
            [7] = "Main menu", [8] = "World map", [9] = "Character",
            [10] = "Customize", [11] = "Development", [12] = "Power up", [13] = "Cooking",
            [14] = "Skill tree", [15] = "Skill customize", [16] = "Quests",
            [17] = "Tutorial", [18] = "Load", [19] = "Dragon Balls", [20] = "Shop",
            [21] = "Area map", [22] = "Race", [23] = "Baseball", [24] = "Party",
            [25] = "Community quests", [26] = "System", [27] = "Shop info",
            [28] = "Past quests", [29] = "Meal time", [30] = "Food shop",
            [31] = "Material shop", [32] = "Parts trade", [33] = "Learning",
            [34] = "Training", [35] = "DLC", [37] = "Fast travel",
        },
        startlist = {
            [0] = "Community", [1] = "Dragon Balls", [2] = "Items", [3] = "Party",
            [4] = "Story", [5] = "Characters", [6] = "System", [7] = "Community board",
            [8] = "Soul Emblems", [9] = "Save", [10] = "Load", [11] = "Options",
            [12] = "Tutorial", [13] = "Title screen", [14] = "Downloadable content",
        },
    },
}

local function tbl() return S[I18n.language()] or S[DEFAULT] end

-- ---- accessors -------------------------------------------------------------

-- A plain localized string by key (falls back to English, then the key itself).
function I18n.t(key)
    local t = tbl()
    local v = t[key]
    if v ~= nil then return v end
    local e = S[DEFAULT][key]
    if e ~= nil then return e end
    return key
end

-- Localized controller-button name for a canonical token (A, LB, L3, …). Unknown
-- tokens use the language's "%s" fallback template so nothing is dropped.
function I18n.button(token)
    local t = tbl()
    local b = t.buttons and t.buttons[token]
    if b then return b end
    return string.format(t.button_fallback or "%s", token)
end

-- Localized keyhelp glyph name for a texture token (Btn_L1, Btn_Options, …), or nil
-- if the token has no descriptive name in any table.
function I18n.keyhelp(token)
    local t = tbl()
    return (t.keyhelp and t.keyhelp[token])
        or (S[DEFAULT].keyhelp and S[DEFAULT].keyhelp[token]) or nil
end

-- Spoken name for an overworld menu section by EXCmnHeaderFontType enum value, or nil if
-- the value has no mapped name (then the reader stays silent rather than guessing).
function I18n.header(ft)
    local t = tbl()
    return (t.header and t.header[ft])
        or (S[DEFAULT].header and S[DEFAULT].header[ft]) or nil
end

-- The header's label asset carries a "Lang_<Token>" section id (e.g. Lang_MainMenu). These
-- tokens are the game's FontType table in order, so the token -> EXCmnHeaderFontType index is
-- fixed (from the decompiled UAT_UIXCmnHeader::SetFontType). Map token -> the localized name.
local SECTION_TOKENS = {
    Commu = 0, Commuboard = 1, Item = 2, Opt = 3, Save = 4, Soulbadge = 5, Storyresult = 6,
    MainMenu = 7, World = 8, Char = 9, Customize = 10, Develop = 11, Powerup = 12, Cooking = 13,
    Skilltree = 14, Skillset = 15, Quest = 16, Tips = 17, Load = 18, Db = 19, Shop = 20,
    Areamap = 21, Minigame_Race = 22, Minigame_BaseBall = 23, Party = 24, Commuquest = 25,
    System = 26, Shop_Info = 27, Past_Quests = 28, Mealtime = 29, Shop_Food = 30,
    Shop_Material = 31, Shop_Trade = 32, Learning = 33, Trai = 34, DLC = 35, Fast_Travel = 37,
}

-- Localized section name for a "Lang_<Token>" section token; falls back to a readable form of
-- the token itself (spaces for underscores) so an unmapped section is still spoken.
function I18n.section(token)
    if not token then return nil end
    local e = SECTION_TOKENS[token]
    if e then return I18n.header(e) or token end
    return (token:gsub("_", " "))
end

-- Spoken name for an overworld main-menu entry by its START_TOP_LIST_ID (each ring/list item's
-- 0x404 byte), or nil if unmapped. This is the game's main-menu item id, NOT EXCmnHeaderFontType.
function I18n.startlist(id)
    local t = tbl()
    return (t.startlist and t.startlist[id])
        or (S[DEFAULT].startlist and S[DEFAULT].startlist[id]) or nil
end

return I18n
