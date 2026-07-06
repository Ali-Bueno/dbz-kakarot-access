-- Screen adapter: area map (Map_M_C) + world map (Map_World_C).
--
-- Both are free-cursor map screens. The readable, game-authored signals (blueprint
-- headers Map_M.hpp / Map_World.hpp + the F7 dumps 2026-07-06):
--   * Map_M_C.Txt_Name — the area's name banner ("East Ravine Area"); Txt_Area also
--     declared (icon-hover text).
--   * Map_World_C.Txt_Area / Txt_Name — the hovered-area name on the world map: it
--     fills while the free cursor sits on an area and is how a destination is picked
--     ("move the cursor until you hear the area, then Confirm").
-- The screen name comes from the mod's section table (the on-screen titles are image
-- fonts); the keyhelp help line ("View current area info…") is the tooltip. Hosts are
-- pooled and MULTI-INSTANCE (Map_M_C_2 / Map_World_C_2 live) and, like the other
-- fullscreen menus, are expected to park HitTestInvisible when closed -> the open
-- gate is the host's own slate enum 0 on the first on-screen instance.

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")
local Keyhelp = require("keyhelp")

local Map = {}

local ann = Core.make_announcer()
local tick = 0
local state = nil    -- { screen, name } computed in is_active

local function clean(t) return t and A.markup_to_speech(t) or nil end

local function node_text(node)
    local t
    pcall(function() t = clean(Core.read_text(node)) end)
    return t
end

-- The genuinely OPEN instance of a pooled map host (slate enum 0), or nil.
local function open_host(cls)
    for _, o in ipairs(Core.cached_all(cls, tick)) do
        if Core.on_screen(o) then
            local ok, v = pcall(function() return o:GetVisibility() end)
            if ok and tonumber(v) == 0 then return o end
        end
    end
    return nil
end

function Map.is_active()
    tick = tick + 1
    -- World map first: it opens over / replaces the area map.
    local host = open_host("Map_World_C")
    if host then
        -- The hovered area (Txt_Area, else Txt_Name). Empty = the cursor sits on open
        -- terrain, spoken as "empty terrain" so LEAVING an area is heard too (what the
        -- terrain actually is — sea, mountains — isn't exposed by the game's UI).
        local area = node_text(host.Txt_Area) or node_text(host.Txt_Name)
        state = { screen = I18n.header(8), name = area or I18n.t("map_empty") }
        return true
    end
    host = open_host("Map_M_C")
    if host then
        state = { screen = I18n.header(21),
                  name = node_text(host.Txt_Name) or node_text(host.Txt_Area) }
        return true
    end
    state = nil
    return false
end

function Map.reset() ann:reset() end

function Map.update()
    local s = state
    if not s then return end
    -- screen name on entry; the current/hovered area (or "empty terrain") as it
    -- changes; the game's help line as the tooltip.
    ann:focus(s.screen, nil, s.name or s.screen, nil, Keyhelp.helpmsg)
end

return Map
