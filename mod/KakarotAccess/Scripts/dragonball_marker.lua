-- Dragon Balls are registered directly with the native minimap, without an
-- ATMapIconComponent on the pickup. Read those displayed markers, not spawn
-- tables or save data. No scans, hooks, or actor/widget caches are added here.
local Core = require("ui_core")
local Mem = require("mem")
local Off = require("native_offsets").miniMapIcon
local Marker = {}
local icon_class -- native UClass only; never a per-world actor/widget handle

-- Returns the live pickup, or nil. The second result is false only when native
-- state was unreadable, so a transient read failure need not end an active pick.
function Marker.actor(icon, minimap)
    if not Core.valid(icon) then return nil, true end
    -- Validate the native base before private reads. A reflected pointer array
    -- may contain subclasses, which retain this layout; an exact name check
    -- would silently exclude them. UClass is process-lifetime, not a HUD cache.
    if not Core.valid(icon_class) then
        local ok, cls = pcall(StaticFindObject, "/Script/AT.AT_UIMiniMapIcon")
        if not ok or not Core.valid(cls) then return nil, false end
        icon_class = cls
    end
    local ok, is_icon = pcall(function() return icon:IsA(icon_class) end)
    if not ok then return nil, false end
    if is_icon ~= true then return nil, true end
    local kind = Mem.u8(icon, Off.iconType)
    if kind == nil then return nil, false end
    if kind ~= Off.dragonBallType then return nil, true end
    local active = Mem.u8(icon, Off.active)
    if active == nil then return nil, false end
    if active ~= 1 then return nil, true end
    local owner = Core.member(icon, "WL_Owner")
    if not Core.valid(owner) then return nil, false end
    local owner_addr, map_addr = Mem.raw_addr(owner), Mem.raw_addr(minimap)
    if not owner_addr or not map_addr then return nil, false end
    if owner_addr ~= map_addr then return nil, true end
    local widget = Core.member(icon, "WL_Icon_ImgSw")
    if not Core.on_screen(widget) or not Core.pane_rendered(widget) then return nil, true end
    local actor = Core.member(icon, "TargetActor")
    if not Core.valid(actor) then return nil, true end
    local hidden = Core.member(actor, "bHidden")
    if hidden == true or hidden == 1 then return nil, true end
    return actor, true
end

-- Revalidate a pick against the CURRENT marker list, not a sweep snapshot that
-- can outlive collection. nil means the map/read is unavailable; false is a
-- complete readable list with no matching displayed ball. The caller already
-- owns the free-roam/transition gate. Handles are never kept between calls.
function Marker.contains(minimap, actor)
    if not Core.valid(minimap) or not Core.on_screen(minimap) then return nil end
    if not Core.valid(actor) then return false end
    local wanted = Mem.raw_addr(actor)
    if not wanted then return nil end
    local arr, n = Core.array_of(minimap, "MapIconList")
    if not arr or not n then return nil end
    local complete = true
    for i = 1, n do
        local candidate, readable = Marker.actor(arr[i], minimap)
        if candidate and Mem.raw_addr(candidate) == wanted then return true end
        if not readable then complete = false end
    end
    if complete then return false end
    return nil
end

return Marker
