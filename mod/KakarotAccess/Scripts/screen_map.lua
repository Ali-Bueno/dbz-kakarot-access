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
local Nav = require("nav_tracker")
local Registry = require("ui_registry")

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

-- Area map (Map_M_C / AT_UIMapM): the POI the cursor is focused on lives in FocusTarget
-- (reflected). We announce it (type via the radar's EMapIcon vocabulary + the game's detail
-- text) each time the focus changes, so a blind player can hear what the cursor is over.
local area_focus_key = nil   -- diff-gate: address of the last-announced focused icon

-- World-map ICON WAKE (dump_map_state 2026-07-09): on opening the world map the game
-- creates the Map_World_Icon_C widgets LAZILY — only once it sees a cursor move — so the
-- icons gate in is_active stays closed and the d-pad is dead until the player nudges the
-- stick ("tengo que mover un milímetro el stick"). The mod performs that nudge itself:
-- the ONLY route to the world map is Triangle from the area map, and closing the area map
-- back to gameplay always brings the minimap back — so "area map just closed AND the
-- minimap did not return" means the world map is opening. While that window is armed and
-- no icons exist yet, a micro left-stick pulse is injected (input_bridge inject — the
-- self-expiring TTL guarantees release) until the icons materialize. Seeing the minimap
-- cancels the window at once, so combat — where the minimap is also hidden — can't be
-- reached inside an armed window (the open map pauses the world).
local WAKE_TICKS = 20        -- window length in UI polls (~2 s at 100 ms)
local WAKE_PULSE = 0.5       -- stick deflection of the nudge (small but above deadzone)
local area_open = false      -- Map_M_C was on-screen last poll
local wake_until = nil       -- tick the wake window expires at (nil = disarmed)
local wake_pulsing = false   -- an inject pulse is live (must inject_off when done)

local function wake_disarm()
    if wake_pulsing then Input.inject_off() end
    wake_until, wake_pulsing = nil, false
end

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
local MAP_DEBUG = false  -- OFF 2026-07-11 (no automatic dumps); re-arm only to diagnose
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
    -- AREA map FIRST — its own authoritative signal, checked BEFORE the minimap gate. Unlike the
    -- WORLD map, the AREA map (Mapa del Área) does NOT hide the minimap radar: it stays on-screen
    -- WITH Map_M_C enum 3 (verified dump_map_state 2026-07-09), so the free-roam gate below would
    -- wrongly preempt it. Map_M_C collapses to enum 1 (on_screen false) on close, so this is clean.
    local am = Core.first_on_screen("Map_M_C", tick)
    if am then
        area_open = true
        wake_disarm()
        state = { screen = I18n.header(21),
                  name = node_text(am.Txt_Name) or node_text(am.Txt_Area),
                  host = am }   -- the AT_UIMapM whose FocusTarget we read for POI-under-cursor
        return true
    end
    if area_open then
        -- The area map JUST closed: back to gameplay (minimap returns, window cancelled
        -- below) or forward to the world map (minimap stays hidden) — arm the icon wake.
        area_open = false
        wake_until = tick + WAKE_TICKS
    end
    -- FREE-ROAM CROSS-CHECK (WORLD map only): the minimap radar is hidden under the full-screen
    -- WORLD map but stays up in free-roam AND during the AREA map (handled above). The pooled
    -- Map_World_C widgets keep reporting on-screen AFTER the world map is dismissed (verified
    -- 2026-07-08: latched onScreen=true WITH the minimap up), which stuck the radar — including
    -- the load-save case. So here (area map already ruled out) minimap up => world map closed.
    if Core.first_on_screen("AT_UIMiniMapRadar", tick) then
        wake_disarm()   -- gameplay confirmed: never nudge the stick here
        state = nil
        return false
    end
    -- ICON WAKE: world map opening (window armed, minimap gone) but the game hasn't
    -- materialized the travel-point icons yet — keep the micro pulse alive until it does
    -- (the gate right below then confirms the map and disarms).
    if wake_until and not world_icons_on_screen() then
        if tick >= wake_until then
            wake_disarm()
        elseif Core.first_on_screen("Map_World_C", tick) then
            wake_pulsing = Input.inject(WAKE_PULSE, 0) or wake_pulsing
        end
    end
    -- WORLD map: gated on REAL rendered travel-point icons, not the always-on host (which
    -- false-positived during combat, where the minimap is also hidden). Host = text source.
    if world_icons_on_screen() then
        wake_disarm()   -- icons live: the wake (if any) did its job
        local wm = Core.first_on_screen("Map_World_C", tick)
        -- The hovered area (Txt_Area, else Txt_Name). Empty = the cursor sits on open
        -- terrain, spoken as "empty terrain" so LEAVING an area is heard too (what the
        -- terrain actually is — sea, mountains — isn't exposed by the game's UI).
        local area = wm and (node_text(wm.Txt_Area) or node_text(wm.Txt_Name))
        state = { screen = I18n.header(8), name = area or I18n.t("map_empty"), world = true }
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

-- Area map: announce the POI the cursor is focused on (FocusTarget), each time it changes.
-- Type = the radar's EMapIcon noun of the focused icon's Target actor; name = the game's own
-- detail text (WL_MapDetailTxt.WL_Detail_Txt), which it fills for the focused icon. Silent
-- when the cursor sits on no icon (FocusTarget nil), so sweeping open map area isn't spammy.
local function area_poi(host)
    local ft
    pcall(function() ft = host.FocusTarget end)
    if not (ft and Core.valid(ft)) then area_focus_key = nil; return end
    local key = Mem.addr(ft)
    if key and key == area_focus_key then return end   -- same focus -> already announced
    area_focus_key = key
    local ta
    pcall(function() ta = ft.Target end)
    local noun = (ta and Core.valid(ta)) and Nav.icon_noun(ta) or nil
    -- the detail panel's text (the POI's own name/description), if the game is showing it.
    local name
    pcall(function() name = clean(Core.read_text(host.WL_MapDetailTxt.WL_Detail_Txt)) end)
    local msg
    if noun and name and name ~= "" then msg = noun .. ", " .. name
    else msg = noun or (name ~= "" and name) or nil end
    if msg then Speech.say(msg, true) end
end

function Map.reset()
    ann:reset(); dests_said = false
    ft_points, ft_sel, ft_prevbtn = nil, nil, 0
    area_focus_key = nil
    wake_disarm()
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
        -- List the reachable fast-travel points once (so a blind player knows what's
        -- there). The d-pad selection itself runs on the FAST loop below, not here.
        if not dests_said then
            dests_said = true
            ft_points = ft_build(host)
            if #ft_points > 0 then
                Speech.say(string.format(I18n.t("map_travel_points"), #ft_points, table.concat(ft_points, ", ")), false)
            end
        end
    elseif s.host then
        area_poi(s.host)   -- read the POI under the cursor on the area map
    end
end

-- ---- fast travel-selection loop -------------------------------------------------------
-- The d-pad runs on its own 20 ms loop (radar_menu pattern): the 100 ms UI poll missed
-- quick taps and added up to a poll of lag per press ("map responsiveness", 2026-07-09).
-- The loop only acts while the MAP adapter owns the screen — when the travel YesNo (or any
-- other dialog) is on top, the registry stops polling Map.is_active, `state` goes stale,
-- and without the ownership guard the d-pad would keep retargeting under the dialog.

local TRAVEL_TICK_MS = 20
local travel_running = false

local function ft_step()
    if Transition.active() then ft_prevbtn = 0 return end
    local s = state
    if not (s and s.world) or Registry.active_adapter() ~= Map then
        ft_prevbtn = 0
        return
    end
    local host = ft_host()
    if host then ft_guidance(host) end
end

function Map.start()
    if travel_running then return end
    if not Input.is_loaded() then
        print("[KakarotAccess] screen_map: input bridge not loaded, travel d-pad disabled\n")
        return
    end
    travel_running = true
    _G.__KakarotMapTravelGen = (_G.__KakarotMapTravelGen or 0) + 1
    local myGen = _G.__KakarotMapTravelGen
    local busy = false
    LoopAsync(TRAVEL_TICK_MS, function()
        if _G.__KakarotMapTravelGen ~= myGen then return true end
        if not busy then
            busy = true
            ExecuteInGameThread(function()
                -- busy cleared on ENTRY, like radar_menu: an uncatchable C++ abort in the
                -- callback must not leave the guard stuck and the d-pad dead for the session.
                busy = false
                local ok, err = pcall(ft_step)
                if not ok then
                    print("[KakarotAccess] screen_map travel step error: " .. tostring(err) .. "\n")
                end
            end)
        end
        return false
    end)
end

function Map.stop()
    travel_running = false
    _G.__KakarotMapTravelGen = (_G.__KakarotMapTravelGen or 0) + 1
end

return Map
