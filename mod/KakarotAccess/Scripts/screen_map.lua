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
local Mem = require("mem")
local OFF = require("native_offsets")

local Map = {}

local ann = Core.make_announcer()
local tick = 0
local state = nil    -- { screen, name, world } computed in is_active
local dests_said = false   -- world-map travel points announced once per opening

-- World-map fast travel via the game's OWN selection machine (UAT_UIMapWorld, Ghidra 2026-07-09,
-- native_offsets.mapWorld): d-pad up/down picks a destination by its native InfoIcon INDEX, we
-- WRITE that index to selIndex(0x514), and Confirm(A) fires the game's InputConfirm() → the
-- "Go to X?" YesNo. No stick injection, no cursor math — the selected index IS the source of
-- truth the confirm reads (verified in-game: write index + InputConfirm travels to that point).
local FT = OFF.mapWorld
local ft_points = nil      -- ordered { name } by InfoIcon index (list[i+1] = name for index i)
local ft_sel = nil         -- chosen index (0-based), or nil until the player d-pads
local ft_prevbtn = 0       -- previous pad bitmask (button edge detection)

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

-- The live UAT_UIMapWorld host (BP Map_World_C) whose selection machine we drive, or nil.
-- Uses the SAME pick as the validated in-game test (first transient instance), so the address
-- we read/write is the one the confirm core actually reads.
local function ft_host()
    for _, o in ipairs(Core.cached_all("Map_World_C", tick)) do
        if Core.valid(o) then
            local fn = ""
            pcall(function() fn = o:GetFullName() end)
            if fn:find("/Engine/Transient", 1, true) then return o end
        end
    end
    return nil
end

-- Build the ordered fast-travel point names, INDEXED BY THE GAME'S native InfoIcon order
-- (list[i+1] = name for index i), so the index we write to selIndex matches the announced
-- name. Each InfoIcon entry's icon widget (entry+0x08) is matched by address to a live
-- Map_World_Icon_C to read its Txt_Name. Only points the game actually put on the map appear
-- (undiscovered destinations are not rendered), so the list stays faithful to the gameplay.
local function ft_build(host)
    -- address -> name for every live world-map icon (the names the game shows).
    local byaddr = {}
    for _, ic in ipairs(Core.cached_all("Map_World_Icon_C", tick)) do
        if Core.valid(ic) then
            local a = Mem.addr(ic)
            local nm
            pcall(function() nm = clean(Core.read_text(ic.Txt_Name)) end)
            if a and nm and nm ~= "" then byaddr[a] = nm end
        end
    end
    local data = Mem.ptr(host, FT.infoIconData)
    local count = Mem.i32(host, FT.infoIconCount) or 0
    local out = {}
    if data and data ~= 0 and count > 0 and count < 256 then
        for i = 0, count - 1 do
            -- read the entry's icon widget pointer (8 bytes LE) and match it by address.
            local iconptr
            local b = Mem.at_bytes(data, i * FT.infoIconStride + FT.entryIcon, 8)
            if b and #b == 8 then iconptr = string.unpack("<I8", b) end
            out[i + 1] = (iconptr and byaddr[iconptr]) or (I18n.t("map_point") .. " " .. (i + 1))
        end
    end
    return out
end

-- World-map fast travel: d-pad up/down selects a point (native InfoIcon index); we write the
-- index and announce the name. Confirm (A) writes the index again and calls InputConfirm() so
-- the game opens its own "Go to X?" YesNo for the CHOSEN point — regardless of where the analog
-- cursor sits. We re-assert the chosen index each tick so a stray hover can't retarget Confirm.
local function ft_guidance(host)
    if not ft_points then ft_points = ft_build(host) end
    local n = #ft_points
    if n == 0 then return end
    local snap = Input.read()
    if not snap then return end
    local B = Input.BTN
    local function pressed(m) return (snap.buttons & m) ~= 0 and (ft_prevbtn & m) == 0 end
    if pressed(B.DPAD_DOWN) then
        ft_sel = ((ft_sel or -1) + 1) % n
        Mem.write_i32(host, FT.selIndex, ft_sel)
        Speech.say(string.format(I18n.t("map_on_point"), ft_points[ft_sel + 1]), true)
    elseif pressed(B.DPAD_UP) then
        ft_sel = ((ft_sel or 0) - 1) % n
        Mem.write_i32(host, FT.selIndex, ft_sel)
        Speech.say(string.format(I18n.t("map_on_point"), ft_points[ft_sel + 1]), true)
    end
    -- Confirm: pin the chosen index and fire the game's own confirm for it (validated in-game).
    if pressed(B.A) and ft_sel then
        Mem.write_i32(host, FT.selIndex, ft_sel)
        pcall(function() host:InputConfirm() end)
    end
    ft_prevbtn = snap.buttons
    -- keep the chosen index pinned so the game's own Confirm (A) also targets it.
    if ft_sel then Mem.write_i32(host, FT.selIndex, ft_sel) end
end

function Map.reset()
    ann:reset(); dests_said = false
    ft_points, ft_sel, ft_prevbtn = nil, nil, 0
end

function Map.update()
    local s = state
    if not s then return end
    -- screen name on entry; the current/hovered area (or "empty terrain") as it
    -- changes; the game's help line as the tooltip.
    ann:focus(s.screen, nil, s.name or s.screen, nil, Keyhelp.helpmsg)
    if s.world then
        local host = ft_host()
        if not host then return end
        -- List the reachable fast-travel points once (so a blind player knows what's there),
        -- then run d-pad selection over the game's native index.
        if not dests_said then
            dests_said = true
            ft_points = ft_build(host)
            if #ft_points > 0 then
                Speech.say(string.format(I18n.t("map_travel_points"), #ft_points, table.concat(ft_points, ", ")), false)
            end
        end
        ft_guidance(host)
    end
end

return Map
