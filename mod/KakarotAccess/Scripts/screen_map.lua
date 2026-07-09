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
local Transition = require("transition")
local Speech = require("speech")
local Input = require("input")

local Map = {}

local ann = Core.make_announcer()
local tick = 0
local state = nil    -- { screen, name, world } computed in is_active
local dests_said = false   -- world-map travel points announced once per opening

-- World-map travel GUIDANCE: d-pad picks a destination; we tell the player which way to
-- push the STICK to move the analog cursor onto it, and announce "on point" when close.
-- Cursor pos = Map_World_Curs RenderTransform.Translation; icon pos = its canvas Slot
-- offset — both in the SAME screen-pixel space (verified 2026-07-09). No input injection.
local ON_POINT_PX = 26     -- cursor within this many px of the icon = on the point (stop)
local DRIVE_NEAR_PX = 55   -- inside this, ease the push for a soft, accurate approach
local DRIVE_FAR = 1.0      -- stick magnitude while far from the point (full = fastest)
local DRIVE_NEAR = 0.7     -- stick magnitude while easing in
local guide_target = nil   -- selected destination index (into icons_pos())
local guide_btn = 0        -- previous pad bitmask (d-pad edge detection)
local guide_dir = nil      -- state gate: "moving" while driving, "on" once arrived

local function clean(t) return t and A.markup_to_speech(t) or nil end

local function node_text(node)
    local t
    pcall(function() t = clean(Core.read_text(node)) end)
    return t
end

-- TEMP (2026-07-09): can we read the world-map CURSOR position and each fast-travel ICON
-- position in the same space? That's the prerequisite for d-pad snap / audio guidance to a
-- destination. Logs cursor + icon render translations (and names) to dumps/dump_map_curs.txt
-- on change while the world map is open. Move the cursor slowly over 2-3 travel points and
-- send the file. Set CURS_DEBUG=false after.
local CURS_DEBUG = false   -- cursor+icon positions confirmed readable (dump 2026-07-09)
local curs_last, curs_next = nil, 0
local function render_xy(o)
    local x, y
    pcall(function()
        local t = o.RenderTransform.Translation
        x, y = t.X, t.Y
    end)
    return x, y
end
local function curs_debug()
    if not CURS_DEBUG or tick < curs_next then return end
    curs_next = tick + 3
    if not Core.first_on_screen("Map_World_C", tick) then return end
    local lines = {}
    for _, o in pairs(FindAllOf("Map_World_Curs_C") or {}) do
        if Core.valid(o) then
            local fn = ""
            pcall(function() fn = o:GetFullName() end)
            if fn:find("BP_ATGameInstance", 1, true) then
                local x, y = render_xy(o)
                lines[#lines + 1] = string.format("CURS render=%s,%s", tostring(x), tostring(y))
            end
        end
    end
    for _, ic in pairs(FindAllOf("Map_World_Icon_C") or {}) do
        if Core.valid(ic) then
            local nm
            pcall(function() nm = clean(Core.read_text(ic.Txt_Name)) end)
            local rx, ry = render_xy(ic)
            -- Icons render at 0,0 → their screen position lives in the canvas Slot offsets.
            local sl = "?"
            pcall(function()
                local off = ic.Slot.LayoutData.Offsets
                sl = string.format("%.1f,%.1f", off.Left, off.Top)
            end)
            -- Absolute geometry as a second candidate (works even off a canvas panel).
            local ab = "?"
            pcall(function()
                local g = ic:GetCachedGeometry()
                local p = g.AbsolutePosition or g:GetAbsolutePosition()
                ab = string.format("%.1f,%.1f", p.X, p.Y)
            end)
            lines[#lines + 1] = string.format("ICON %s render=%s,%s slot=%s abs=%s",
                tostring(nm), tostring(rx), tostring(ry), sl, ab)
        end
    end
    table.sort(lines)
    local sig = table.concat(lines, "|")
    if sig == curs_last then return end
    curs_last = sig
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_map_curs.txt", "a")
        if not f then return end
        f:write(string.format("[%d]\n%s\n\n", os.time(), table.concat(lines, "\n")))
        f:close()
    end)
end

-- Fast-travel destinations shown on the WORLD map are Map_World_Icon_C widgets, each with
-- a Txt_Name label (the point's name, e.g. "Ciénaga Blake" — dump 2026-07-09). Enumerated
-- once per map opening. Read-only for now: it names what's reachable; actually MOVING the
-- analog cursor onto a point (to trigger the "¿Ir a X?" confirm) would need cursor
-- injection, which the input bridge can't do yet.
local function travel_points()
    local names, seen = {}, {}
    for _, ic in pairs(FindAllOf("Map_World_Icon_C") or {}) do
        if Core.valid(ic) and Core.on_screen(ic) then
            local n
            pcall(function() n = clean(Core.read_text(ic.Txt_Name)) end)
            if n and n ~= "" and not seen[n] then
                seen[n] = true
                names[#names + 1] = n
            end
        end
    end
    return names
end

-- The world map is GENUINELY open only when its travel-point icons are actually rendered
-- (on-screen). The Map_World_C host stays on-screen permanently, and "minimap hidden" also
-- holds in COMBAT/cutscenes — together they false-positived the world map during a battle
-- (user 2026-07-09), which spammed the travel points AND injected the stick mid-fight.
local function world_icons_on_screen()
    for _, ic in ipairs(Core.cached_all("Map_World_Icon_C", tick)) do
        if Core.valid(ic) and Core.on_screen(ic) then return true end
    end
    return false
end

-- TEMP DIAGNOSTIC (2026-07-08): the map is the only fullscreen adapter that gates
-- is_active on visibility ALONE, so its pooled host latches "open" after close and
-- keeps ui_muted() true (radar can't re-arm). Before changing the gate we need to see
-- which signal actually flips open->closed (enum stays 0, unlike other menus). Logs
-- every pooled Map host's signals to dumps/dump_map_state.txt on CHANGE, throttled.
-- Open the map, move the cursor a bit, then close it, and send me the file.
local MAP_DEBUG = true   -- RE-ARMED 2026-07-09: world map false-positived during COMBAT
local dbg_last, dbg_next = nil, 0
local function map_debug()
    if not MAP_DEBUG or tick < dbg_next then return end
    dbg_next = tick + 5   -- ~0.5 s between samples
    local lines = {}
    -- SAFETY: during a map switch (fast travel!) the widgets are being torn down and
    -- reading them is an uncatchable abort — log only the gate state, no widget reads.
    if Transition.active() then
        lines[1] = "[TRANSITION ACTIVE — widget reads skipped]"
    else
        -- The AUTHORITATIVE free-roam signal: the minimap radar is on-screen ONLY in
        -- normal gameplay (hidden in full-screen menus). If it is on-screen, the map is
        -- definitely CLOSED — this is the cross-check the map's own widgets can't give.
        local mm = Core.first_on_screen("AT_UIMiniMapRadar", tick)
        lines[1] = "MINIMAP(free-roam) onScreen=" .. tostring(mm ~= nil)
        local wl = {}
        for _, cls in ipairs({ "Map_M_C", "Map_World_C", "Map_World_Curs_C", "Map_World_Icon_C" }) do
            for _, o in ipairs(Core.cached_all(cls, tick)) do
                if Core.valid(o) then
                    local nm = "?"
                    pcall(function() nm = o:GetFullName() end)
                    if not nm:find("/Game/Art", 1, true) then   -- skip archetype/CDO copies
                        local vis, en, vp = "?", "?", "?"
                        pcall(function() vis = tostring(o:IsVisible()) end)
                        pcall(function() en = tostring(tonumber(o:GetVisibility())) end)
                        pcall(function() vp = tostring(o:IsInViewport()) end)   -- all are UserWidgets
                        wl[#wl + 1] = string.format("%s vis=%s enum=%s inVP=%s onScreen=%s",
                            nm, vis, en, vp, tostring(Core.on_screen(o)))
                    end
                end
            end
        end
        table.sort(wl)
        for _, l in ipairs(wl) do lines[#lines + 1] = l end
    end
    local sig = table.concat(lines, "|")
    if sig == dbg_last then return end
    dbg_last = sig
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_map_state.txt", "a")
        if not f then return end
        f:write(string.format("[%d] %d instances\n%s\n\n", os.time(), #lines, table.concat(lines, "\n")))
        f:close()
    end)
end

-- TEMP one-shot STRUCTURE dump (2026-07-08): to design the richer area reading (#2) and
-- find any snappable area index/list (#3), we need the world-map host's full property
-- tree (existing dumps only show text leaves). Fires ONCE when the world map is first
-- seen open, into dumps/dump_map_struct.txt. Reflection can abort UNCATCHABLY on this
-- game, so every hop is IsValid + pcall (mirrors discover.lua). Set STRUCT_DUMP=false
-- once captured. Just OPEN THE WORLD MAP and send me the file.
local STRUCT_DUMP = false   -- blind property walk is abort-prone here; use targeted node reads
local struct_done = false
local function dump_struct(host)
    if not STRUCT_DUMP or struct_done or not Core.valid(host) then return end
    struct_done = true
    local lines = {}
    pcall(function() lines[#lines + 1] = "HOST " .. host:GetFullName() end)
    -- Declared properties of the host class AND its native parents (the useful members —
    -- an area list / cursor index — live on the UAT_UIMap* native parent, not the _C).
    local cls = nil
    pcall(function() cls = host:GetClass() end)
    local depth = 0
    while Core.valid(cls) and depth < 6 do
        depth = depth + 1
        local cn = "?"
        pcall(function() cn = cls:GetFName():ToString() end)
        lines[#lines + 1] = "== class " .. cn .. " =="
        pcall(function()
            cls:ForEachProperty(function(prop)
                local pn, pt = "?", "?"
                pcall(function() pn = prop:GetFName():ToString() end)
                pcall(function() pt = prop:GetClass():GetFName():ToString() end)
                local extra = ""
                local child
                pcall(function() child = host[pn] end)
                if type(child) == "userdata" then
                    local ok_v = false
                    pcall(function() ok_v = child:IsValid() == true end)
                    if ok_v then
                        local ccls, txt = "?", nil
                        pcall(function() ccls = child:GetClass():GetFName():ToString() end)
                        pcall(function() txt = Core.read_text(child) end)
                        extra = " -> " .. ccls .. (txt and (" text='" .. txt .. "'") or "")
                    end
                elseif child ~= nil and type(child) ~= "table" then
                    extra = " = " .. tostring(child)
                end
                lines[#lines + 1] = string.format("  %s : %s%s", pn, pt, extra)
            end)
        end)
        local sup
        pcall(function() sup = cls:GetSuperStruct() end)
        cls = (Core.valid(sup)) and sup or nil
    end
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_map_struct.txt", "w")
        if not f then return end
        f:write(table.concat(lines, "\n") .. "\n")
        f:close()
    end)
end

function Map.is_active()
    tick = tick + 1
    map_debug()
    curs_debug()
    -- One-shot structure capture: dump whichever map host is on-screen (TEMP, see dump_struct).
    if not struct_done then
        local h = Core.first_on_screen("Map_World_C", tick) or Core.first_on_screen("Map_M_C", tick)
        if h then dump_struct(h) end
    end
    -- FREE-ROAM CROSS-CHECK (the real fix): the minimap radar is on-screen ONLY in normal
    -- gameplay, never under a full-screen map. The pooled map widgets keep reporting
    -- on-screen AFTER the map is dismissed (verified 2026-07-08: latched Map_World_C/Map_M
    -- onScreen=true WITH the minimap up), which is exactly what stuck the radar — including
    -- the load-save case. So if the minimap is up, the map is closed. Authoritative; a
    -- widget latch can't defeat it.
    if Core.first_on_screen("AT_UIMiniMapRadar", tick) then
        state = nil
        return false
    end
    -- WORLD map: gated on REAL rendered travel-point icons, not the always-on host (which
    -- false-positived during combat, where the minimap is also hidden). Host = text source.
    if world_icons_on_screen() then
        local wm = Core.first_on_screen("Map_World_C", tick)
        -- The hovered area (Txt_Area, else Txt_Name). Empty = the cursor sits on open
        -- terrain, spoken as "empty terrain" so LEAVING an area is heard too (what the
        -- terrain actually is — sea, mountains — isn't exposed by the game's UI).
        local area = wm and (node_text(wm.Txt_Area) or node_text(wm.Txt_Name))
        state = { screen = I18n.header(8), name = area or I18n.t("map_empty"), world = true }
        return true
    end
    -- AREA map (minimap hidden): host on-screen (enum 3 when open, collapses when closed).
    local am = Core.first_on_screen("Map_M_C", tick)
    if am then
        state = { screen = I18n.header(21),
                  name = node_text(am.Txt_Name) or node_text(am.Txt_Area) }
        return true
    end
    state = nil
    return false
end

-- The live world-map cursor position (screen px), or nil.
local function curs_pos()
    for _, o in ipairs(Core.cached_all("Map_World_Curs_C", tick)) do
        if Core.valid(o) then
            local fn = ""
            pcall(function() fn = o:GetFullName() end)
            if fn:find("BP_ATGameInstance", 1, true) then
                local x, y = render_xy(o)
                if x then return x, y end
            end
        end
    end
    return nil
end

-- Fast-travel points with screen positions { {name, x, y}, ... }, name-sorted for a stable
-- d-pad order. Positions come from the canvas Slot offset (the icons render at 0,0).
local function icons_pos()
    local out = {}
    for _, ic in ipairs(Core.cached_all("Map_World_Icon_C", tick)) do
        if Core.valid(ic) then
            local nm, x, y
            pcall(function() nm = clean(Core.read_text(ic.Txt_Name)) end)
            pcall(function()
                local off = ic.Slot.LayoutData.Offsets
                x, y = off.Left, off.Top
            end)
            if nm and nm ~= "" and x then out[#out + 1] = { name = nm, x = x, y = y } end
        end
    end
    table.sort(out, function(a, b) return a.name < b.name end)
    return out
end

-- Drive step (world map only): d-pad up/down picks a destination; we AUTO-MOVE the analog
-- cursor onto it by injecting the left stick, then say "on point" so the player presses
-- confirm. Injection auto-releases in the bridge if this stops running, and we release it
-- on every early exit + in reset(), so the stick can never stay stuck.
local function guidance()
    local snap = Input.read()
    if not snap then Input.inject_off(); return end
    local icons = icons_pos()
    if #icons == 0 then Input.inject_off(); return end
    local B = Input.BTN
    local function pressed(m) return (snap.buttons & m) ~= 0 and (guide_btn & m) == 0 end
    local changed = false
    if pressed(B.DPAD_DOWN) then
        guide_target = (guide_target or 0) % #icons + 1; changed = true
    elseif pressed(B.DPAD_UP) then
        guide_target = ((guide_target or 1) - 2) % #icons + 1; changed = true
    end
    guide_btn = snap.buttons
    if not guide_target then Input.inject_off(); return end   -- no destination picked yet
    if guide_target > #icons then guide_target = #icons end
    local t = icons[guide_target]
    if changed then
        guide_dir = nil
        Speech.say(t.name, true)   -- announce the newly selected destination
    end
    local cx, cy = curs_pos()
    if not cx then Input.inject_off(); return end
    local dx, dy = t.x - cx, t.y - cy   -- screen space: +y is DOWN
    local dist = math.sqrt(dx * dx + dy * dy)
    if dist <= ON_POINT_PX then
        Input.inject_off()
        if guide_dir ~= "on" then
            guide_dir = "on"
            Speech.say(string.format(I18n.t("map_on_point"), t.name), false)
        end
        return
    end
    -- Push the left stick toward the target (screen +y is DOWN -> stick -Y); ease near it.
    local mag = dist > DRIVE_NEAR_PX and DRIVE_FAR or DRIVE_NEAR
    Input.inject((dx / dist) * mag, -(dy / dist) * mag)
    guide_dir = "moving"
end

function Map.reset()
    ann:reset(); dests_said = false
    guide_target, guide_btn, guide_dir = nil, 0, nil
    Input.inject_off()
end

function Map.update()
    local s = state
    if not s then return end
    -- screen name on entry; the current/hovered area (or "empty terrain") as it
    -- changes; the game's help line as the tooltip.
    ann:focus(s.screen, nil, s.name or s.screen, nil, Keyhelp.helpmsg)
    if s.world then
        -- List the reachable fast-travel points once (so a blind player knows what's there),
        -- then run d-pad guidance toward the selected one.
        if not dests_said then
            dests_said = true
            local d = travel_points()
            if #d > 0 then
                Speech.say(string.format(I18n.t("map_travel_points"), #d, table.concat(d, ", ")), false)
            end
        end
        guidance()
    end
end

return Map
