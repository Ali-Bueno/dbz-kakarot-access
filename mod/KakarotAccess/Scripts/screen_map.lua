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
local PadPoll = require("pad_poll")

local Map = {}

local ann = Core.make_announcer()
local tick = 0
local state = nil    -- { screen, name, world } computed in is_active
local dests_said = false   -- world-map travel points announced once per opening
local OBJ_READ_DELAY = 3   -- polls after opening before the quest objective is re-read, so
                           -- it QUEUES after the map's own area/help/travel readout instead
                           -- of talking over it (user 2026-07-24)
local obj_read_at = nil    -- tick to fire the deferred objective re-read (nil = done/disarmed)

-- World-map fast travel via the game's OWN selection machine (UAT_UIMapWorld, Ghidra 2026-07-09,
-- native_offsets.mapWorld): d-pad up/down picks a destination by its native InfoIcon INDEX, we
-- WRITE that index to selIndex(0x514), and Confirm(A) fires the game's InputConfirm() → the
-- "Go to X?" YesNo. No stick injection, no cursor math — the selected index IS the source of
-- truth the confirm reads (verified in-game: write index + InputConfirm travels to that point).
local FT = OFF.mapWorld
-- The BP class the mapWorld offsets belong to, used BOTH to look the host up and to assert it
-- before writing selIndex. The bridge bounds the offset; only this stops a right-offset-wrong-
-- object write, which corrupts silently and crashes later with no trace.
local FT_HOST_CLASS = "Map_World_C"
local ft_points = nil      -- ordered { name } by InfoIcon index (list[i+1] = name for index i)
local ft_sel = nil         -- chosen index (0-based), or nil until the player d-pads
local ft_prevbtn = 0       -- previous pad bitmask (button edge detection)
-- Pending KEYBOARD command for the travel list (main.lua's arrow / Enter keybinds via
-- App.nav_key). Queued rather than acted on where the keybind fires: that callback runs on
-- UE4SS's own thread, and moving a point WRITES the game's selection index. Consumed in
-- ft_guidance, which runs on the game thread. One slot is enough — a human cannot outrun
-- the 20 ms poll.
local ft_kb_cmd = nil

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
    -- Hardened 2026-07-29: `o.RenderTransform.Translation` was a raw two-hop chain — the
    -- inner fetches are not protected by the surrounding pcall against an uncatchable abort
    -- (CLAUDE.md §8). RenderTransform is a UWidget struct property (gated UObject hop via
    -- Core.member), Translation/X/Y are struct-value hops (Core.struct_member).
    local t = Core.member_path(o, "RenderTransform", "Translation")
    if type(t) ~= "userdata" then return nil, nil end
    return Core.struct_member(t, "X"), Core.struct_member(t, "Y")
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
            local nm = clean(Core.read_text(Core.member(ic, "Txt_Name")))
            local rx, ry = render_xy(ic)
            -- Icons render at 0,0 → their screen position lives in the canvas Slot offsets.
            -- Hardened 2026-07-29: Slot is a gated UObject hop (Core.member); LayoutData/
            -- Offsets/Left/Top are struct-value hops (Core.struct_member) — the raw chain
            -- used to escape the wrapping pcall on an uncatchable abort (CLAUDE.md §8).
            local sl = "?"
            -- NOTE (2026-07-29 review): the hop into the SLOT is Core.member, not member_path.
            -- member_path's contract is "one UObject hop, then STRUCT hops only", and a slot is
            -- a UObject - so LayoutData through struct_member would be an UNGATED member fetch
            -- on a class that may not declare it (a non-canvas slot), i.e. the uncatchable abort.
            local slot = Core.member(ic, "Slot")
            local off = Core.struct_member(Core.member(slot, "LayoutData"), "Offsets")
            if off then
                local l, t2 = Core.struct_member(off, "Left"), Core.struct_member(off, "Top")
                if type(l) == "number" and type(t2) == "number" then
                    sl = string.format("%.1f,%.1f", l, t2)
                end
            end
            -- Absolute geometry as a second candidate (works even off a canvas panel).
            -- `g` is a struct returned fresh from a UFUNCTION call (no gate needed for the
            -- call itself), but AbsolutePosition/X/Y are still struct-value hops.
            local ab = "?"
            local ok, g = pcall(function() return ic:GetCachedGeometry() end)
            if ok and g then
                local p = Core.struct_member(g, "AbsolutePosition")
                if not p then pcall(function() p = g:GetAbsolutePosition() end) end
                if p then
                    local px, py = Core.struct_member(p, "X"), Core.struct_member(p, "Y")
                    if type(px) == "number" and type(py) == "number" then
                        ab = string.format("%.1f,%.1f", px, py)
                    end
                end
            end
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
                    -- Core.valid, not a pcall'd IsValid: this walks arbitrary members of a
                    -- pooled map widget, so `child` may well be a freed handle — and IsValid
                    -- dereferences before it checks, so it faults instead of answering false.
                    if Core.valid(child) then
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
                  name = node_text(Core.member(am, "Txt_Name")) or node_text(Core.member(am, "Txt_Area")),
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
        local area = wm and (node_text(Core.member(wm, "Txt_Area")) or node_text(Core.member(wm, "Txt_Name")))
        state = { screen = I18n.header(8), name = area or I18n.t("map_empty"), world = true }
        return true
    end
    state = nil
    return false
end

-- The live UAT_UIMapWorld host (BP Map_World_C) whose selection machine we drive, or nil.
--
-- TWO FIXES, 2026-07-28 (user: the map d-pad is slow, and dead from the SECOND time the map is
-- opened). Both were in this function.
--
-- 1. ON_SCREEN, not just valid. It used to take the first `Core.valid` instance whose path is
--    Transient. These hosts are POOLED and multi-instance (see the file header), and a closed
--    pooled widget stays valid forever — so on the second open this happily returned the previous,
--    off-screen instance and every native selIndex write went to a host the confirm core is not
--    reading. Works the first time, dead from the second: the exact intermittency CLAUDE.md's
--    "invalidate by on_screen, never by validity alone" rule describes.
-- 2. MEMOISED. This runs on the 20 ms pad loop, and the walk below calls `GetFullName()` on every
--    pooled instance — a reflection call per instance per tick, growing as instances accumulate
--    over a session. That is the sluggish d-pad. The resolved host is now reused; `Core.valid`
--    still runs on every use (it is the guard that must never be skipped), and the costlier
--    on_screen re-check is throttled. Cleared in reset(), so each map entry resolves afresh.
local FT_RECHECK_S = 0.25
local ft_cached, ft_checked_at = nil, 0

local function ft_host()
    if ft_cached and Core.valid(ft_cached) then
        local now = os.clock()
        if now - ft_checked_at < FT_RECHECK_S then return ft_cached end
        if Core.on_screen(ft_cached) then ft_checked_at = now return ft_cached end
    end
    ft_cached = nil
    -- peek_all, not cached_all: this runs on the 20 ms pad loop, which never calls
    -- `Core.begin_scan_tick` and so never REFILLS the shared scan budget. `cached_all` can fall to
    -- the scan path when the directory cannot answer, and doing that fifty times a second drains
    -- the budget five times faster than the 100 ms registry refills it — starving the very icon
    -- scan the destination list is waiting on. `peek_all` serves the directory list or the cached
    -- pool and never scans.
    for _, o in ipairs(Core.peek_all(FT_HOST_CLASS)) do
        if Core.valid(o) and Core.on_screen(o) then
            local fn = ""
            pcall(function() fn = o:GetFullName() end)
            if fn:find("/Engine/Transient", 1, true) then
                ft_cached, ft_checked_at = o, os.clock()
                return o
            end
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
    -- peek_all, not cached_all: ft_build is reachable from the 20 ms pad loop
    -- (ft_step -> ft_guidance -> here), where a scanning helper is forbidden for the reason
    -- spelled out in ft_host above. The pool is guaranteed populated by then — the registry
    -- side scans this class in `world_icons_on_screen`, and that gate has to pass before
    -- `state.world` is set and ft_guidance becomes reachable at all.
    local byaddr = {}
    for _, ic in ipairs(Core.peek_all("Map_World_Icon_C")) do
        if Core.valid(ic) then
            local a = Mem.addr(ic)
            local nm = clean(Core.read_text(Core.member(ic, "Txt_Name")))
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
-- The ONE place that writes the game's selection index, so the host-class assertion is stated
-- once instead of at all four call sites below.
local function ft_write_sel(host, idx)
    return Mem.write_i32(host, FT.selIndex, idx, FT_HOST_CLASS)
end

-- Fast-loop self-build throttle, and a direction pressed before the list existed.
local FT_BUILD_S = 0.25
local ft_built_at, ft_pending = 0, nil
-- The NAME of the point last selected. Survives reset(), which is the whole point: pressing A
-- opens the game's yes/no travel prompt, that prompt is a different adapter, the registry commits
-- it and calls Map.reset() — so declining used to drop you back at the start of the list and you
-- had to walk all the way down again (user, 2026-07-28). Kept by NAME rather than by index, the
-- same choice nav_tracker makes for `resume_pick`: the list is rebuilt from scratch on every map
-- open and an index would silently point at a different place, whereas a name either still exists
-- or it does not.
local ft_last_name = nil

-- D-PAD AUTO-REPEAT (user 2026-07-31: "navigating the map can feel a bit laggy"). Without it a
-- twenty-destination list is twenty separate presses, and the player compensates by tapping fast
-- — which is also what made presses get eaten. Holding a direction now keeps stepping.
-- Both delays are interaction constants (there is no game-side value to derive them from) and
-- are paced against the SCREEN READER, not the OS key-repeat rate: every step interrupts the
-- previous name, so anything faster than roughly a short name's worth just yields half-words.
-- The floor is deliberately not tied to Speech.speak_seconds — waiting for each name to finish
-- would make a long list slower to cross than tapping, which is the opposite of the fix.
local RPT_DELAY_S = 0.40   -- hold this long before the repeat starts (a deliberate single step
                           -- must never repeat)
local RPT_EVERY_S = 0.25   -- then one step per this long
local ft_hold_dir, ft_rpt_at = 0, 0

-- Leaving the travel list: a transition, the map closing, or another adapter owning the screen —
-- which includes the game's own "Go to X?" Yes/No, so this runs for the whole confirmation.
--
-- Keep ft_prevbtn tracking the pad's ACTUAL level instead of zeroing it. A zeroed level makes a
-- direction the player is STILL HOLDING read as a brand-new press the instant control comes back
-- (ft_pressed's level compare sees 1-from-0), stepping the list once by itself right where the
-- player expects nothing to happen. Only the auto-repeat is genuinely reset: it represents a
-- session of holding the d-pad, and that session ended when the list lost the screen.
local function ft_idle()
    local snap = Input.read()
    ft_prevbtn = snap and snap.buttons or 0
    ft_hold_dir, ft_rpt_at = 0, 0
end

-- Rising edge for `mask` = the NATIVE LATCH or the level compare against the previous tick.
-- The latch (input_bridge's take_edges, drained once per pad tick by pad_poll) is fed inside
-- the XInput hook at the game's frame rate, so it catches a tap whose whole down-up cycle fell
-- between two of our polls — which the level compare structurally cannot see, and which is what
-- "sometimes it will miss places when you use the DPad" was (user, 2026-07-31). The level
-- compare stays as the fallback for an input_bridge.dll without the latch. Either one saying
-- "pressed" is enough, and a boolean cannot double-fire.
-- `buttons` is this tick's level; the CALLER commits it to ft_prevbtn once all its edges are read.
local function ft_pressed(buttons, mask)
    return Input.pressed(mask) or ((buttons & mask) ~= 0 and (ft_prevbtn & mask) == 0)
end

-- Keep the game's own selection index pinned to ours, so a stray analog hover cannot retarget
-- its Confirm. This used to be an unconditional Mem.write_i32 on EVERY 20 ms tick, and that
-- write asserts the host class first (Mem.class_ok -> class_chain walks the whole super chain
-- through reflection: ~30 pcall'd reflection calls, fifty times a second, on the game thread).
-- Reading the index back is a plain guarded native read with no reflection at all, so correct it
-- only when it has actually drifted. Identical guarantee, a fraction of the cost.
local function ft_pin_sel(host, idx)
    if Mem.i32(host, FT.selIndex) == idx then return true end
    return ft_write_sel(host, idx)
end

-- X on the world map: describe the selection on demand.
--
-- The player pressed X expecting an info key, got a list of place names, and reasonably
-- expected more (user, 2026-07-31). X was never a mod bind: it is one of ui_registry's
-- BOOST_BTNS, so pressing any face button lifts the scan quiet window, the travel-icon pool
-- finally scans, and this screen's once-per-opening "N travel points: …" line fires right
-- then — an info key by accident, saying the one thing it already said on opening. Now it is
-- a real one, and answers the questions the list read-out cannot: WHICH destination is armed,
-- WHERE in the list it sits, and what the free analog cursor is currently over (that is a
-- different thing from the selection — the cursor roams, the d-pad selection does not).
local function ft_describe()
    local n = ft_points and #ft_points or 0
    local parts = {}
    if ft_sel and ft_points and ft_points[ft_sel + 1] then
        parts[#parts + 1] = string.format(I18n.t("map_info_sel"),
            ft_points[ft_sel + 1], ft_sel + 1, n)
    elseif n > 0 then
        parts[#parts + 1] = string.format(I18n.t("map_info_none"), n)
    else
        parts[#parts + 1] = I18n.t("map_info_empty")
    end
    -- Where the game's own free cursor sits, straight from the host text the adapter already
    -- reads each poll (state.name is "empty terrain" when the cursor is off every area).
    local s = state
    if s and s.name then
        parts[#parts + 1] = string.format(I18n.t("map_info_cursor"), s.name)
    end
    Speech.say(table.concat(parts, ". "), true)
end

local function ft_guidance(host)
    -- EDGE BOOKKEEPING FIRST (2026-07-28 — "a veces funciona, a veces no"). `ft_prevbtn` used to
    -- be updated only at the very END of this function, i.e. AFTER the "no list yet" early return.
    -- Every press made while the list was still being built was therefore swallowed without even
    -- being remembered, and the player had to press again — which is exactly what intermittent
    -- feels like. Read the pad once, up front, decide the edges, and commit `ft_prevbtn`
    -- immediately so no path can skip it.
    local snap = Input.read()
    local B = Input.BTN
    local buttons = snap and snap.buttons or 0
    local function pressed(m) return ft_pressed(buttons, m) end
    local hit_down, hit_up, hit_ok = pressed(B.DPAD_DOWN), pressed(B.DPAD_UP), pressed(B.A)
    local hit_info = pressed(B.X)
    ft_prevbtn = buttons

    -- Keyboard: the arrows drive this exactly like the d-pad. Queued by main.lua's keybinds and
    -- consumed HERE, on the game thread, because moving a point WRITES the game's selection index.
    local kcmd = ft_kb_cmd
    ft_kb_cmd = nil

    -- SELF-SUFFICIENT BUILD. This used to wait for Map.update() (100 ms) to produce the list,
    -- which made the d-pad hostage to the SLOW loop's scan luck: the icon pool the build needs
    -- sits on the budgeted scan path, is deferred by quiet mode, and its backoff after a re-open
    -- can be seconds. The old comment was right that a 20 ms rebuild would re-walk the pool fifty
    -- times a second on a map with genuinely no travel points — so the rebuild is throttled by
    -- WALL CLOCK instead of being refused outright.
    local n = ft_points and #ft_points or 0
    if n == 0 then
        local now = os.clock()
        if now - ft_built_at >= FT_BUILD_S then
            ft_built_at = now
            ft_points = ft_build(host)
        end
        n = ft_points and #ft_points or 0
    end
    if n == 0 then
        -- No list yet: REMEMBER the direction instead of dropping it, and replay it on the first
        -- tick there is one.
        if hit_down then ft_pending = 1 elseif hit_up then ft_pending = -1 end
        return
    end

    -- Restore the previous selection by NAME, silently: the player asked to "keep moving from
    -- where I was", not to be told again where that is — the next press announces the new point.
    -- A name that no longer exists simply leaves the selection unset, which is the old behaviour.
    if ft_sel == nil and ft_last_name then
        for i = 1, n do
            if ft_points[i] == ft_last_name then
                ft_sel = i - 1
                ft_write_sel(host, ft_sel)
                break
            end
        end
    end

    local function move(delta)
        ft_sel = ((ft_sel or (delta > 0 and -1 or 0)) + delta) % n
        ft_write_sel(host, ft_sel)
        ft_last_name = ft_points[ft_sel + 1]
        Speech.say(string.format(I18n.t("map_on_point"), ft_points[ft_sel + 1]), true)
    end
    local function confirm()
        if not ft_sel then return end
        ft_write_sel(host, ft_sel)
        pcall(function() host:InputConfirm() end)
    end
    -- A direction pressed before the list existed fires now, once.
    if ft_pending then
        local d = ft_pending
        ft_pending = nil
        move(d)
    end
    if kcmd == "next" then move(1)
    elseif kcmd == "prev" then move(-1)
    elseif kcmd == "select" then confirm() end

    if hit_down then
        move(1)
    elseif hit_up then
        move(-1)
    end
    -- AUTO-REPEAT: a held direction keeps stepping after RPT_DELAY_S. Computed from the LEVEL
    -- (what is held right now), which is the one thing the level read is actually right for —
    -- edges say a press happened, they say nothing about it still being down.
    local now = os.clock()
    local held = (((buttons & B.DPAD_DOWN) ~= 0) and 1)
              or (((buttons & B.DPAD_UP) ~= 0) and -1) or 0
    if held ~= ft_hold_dir then
        ft_hold_dir, ft_rpt_at = held, now + RPT_DELAY_S
    elseif held ~= 0 and now >= ft_rpt_at then
        ft_rpt_at = now + RPT_EVERY_S
        move(held)
    end
    -- Confirm: pin the chosen index and fire the game's own confirm for it (validated in-game).
    if hit_ok then confirm() end
    -- X describes the selection in full (see ft_describe).
    if hit_info then ft_describe() end
    -- keep the chosen index pinned so the game's own Confirm (A) also targets it.
    if ft_sel then ft_pin_sel(host, ft_sel) end
end

-- Area map: announce the POI the cursor is focused on (FocusTarget), each time it changes.
-- Type = the radar's EMapIcon noun of the focused icon's Target actor; name = the game's own
-- detail text (WL_MapDetailTxt.WL_Detail_Txt), which it fills for the focused icon. Silent
-- when the cursor sits on no icon (FocusTarget nil), so sweeping open map area isn't spammy.
local function area_poi(host)
    -- Hardened 2026-07-29: both hops used to be raw (`host.FocusTarget`, `ft.Target`), each
    -- wrapped only in its own pcall — which does not catch an uncatchable member-fetch abort
    -- (CLAUDE.md §8). Both FocusTarget and Target are UObject members, so both go through
    -- Core.member, which existence-gates them.
    -- The explicit Core.valid on each RESULT stays (restored 2026-07-29 review): Core.member
    -- checks its own result ONLY when the property set is available, and when it is not the
    -- gate falls open and hands back whatever the raw fetch returned - for a NULL FocusTarget it
    -- is an INVALID RemoteObject, which is not nil. Dropping the check meant (a) Nav.icon_noun
    -- could be handed a dead handle, and (b) the documented "silent when the cursor sits on no
    -- icon" behaviour inverted into announcing the stale detail text over empty map.
    local ft = Core.member(host, "FocusTarget")
    if not Core.valid(ft) then area_focus_key = nil; return end
    local key = Mem.addr(ft)
    if key and key == area_focus_key then return end   -- same focus -> already announced
    area_focus_key = key
    local ta = Core.member(ft, "Target")
    local noun = Core.valid(ta) and Nav.icon_noun(ta) or nil
    -- the detail panel's text (the POI's own name/description), if the game is showing it.
    -- Both hops guarded: a chained `host.A.B` evaluates the inner fetch at the call
    -- site, outside every pcall (the uncatchable-AV class, CLAUDE.md §8).
    local name = clean(Core.read_text(
        Core.member(Core.member(host, "WL_MapDetailTxt"), "WL_Detail_Txt")))
    local msg
    if noun and name and name ~= "" then msg = noun .. ", " .. name
    else msg = noun or (name ~= "" and name) or nil end
    if msg then Speech.say(msg, true) end
end

function Map.reset()
    ann:reset(); dests_said = false
    ft_points, ft_sel = nil, nil
    ft_idle()   -- button level + auto-repeat: a direction held on the way out must not resume
    -- Drop the memoised fast-travel host: the map is rebuilt on every open, so a handle from the
    -- previous visit is exactly the stale instance that killed the d-pad on re-entry (ft_host).
    ft_cached, ft_checked_at = nil, 0
    ft_kb_cmd = nil   -- a keyboard command left unconsumed from the previous visit must not fire
    ft_built_at, ft_pending = 0, nil   -- and neither must a direction pressed on the way out
    -- `ft_last_name` is DELIBERATELY NOT cleared here. reset() runs whenever another adapter takes
    -- the screen, and the travel yes/no prompt is exactly that — so clearing it here is what made
    -- declining a trip send you back to the top of the list. It self-expires: a name that is not
    -- in the rebuilt list is simply never restored.
    -- The other half of the same rule: ft_build matches InfoIcon entries to icon widgets BY
    -- ADDRESS, and the game recreates `Map_World_Icon_C` every time the map opens. A pool cache
    -- from the previous visit therefore matches nothing and every point degrades to "map point
    -- N". The class is on the scan path (it cannot be directory-mapped), so ask for the re-scan
    -- explicitly on entry instead of waiting out its backoff. Budget-gated inside Core.
    Core.refresh_all("Map_World_Icon_C")
    area_focus_key = nil
    wake_disarm()
    -- Opening the map re-reads the current quest objective on demand: the HUD reader only
    -- speaks it on change / first sight (user 2026-07-24), so the map is where the player
    -- reviews the goal again. DEFERRED (not spoken here): armed for OBJ_READ_DELAY polls so
    -- update() reads it AFTER the map's own entry readout, queued, not talking over it.
    obj_read_at = tick + OBJ_READ_DELAY
end

-- Wall-clock floor between two on-demand objective re-reads (see Map.update).
local OBJ_REPEAT_S = 20
local obj_said_at = nil

function Map.update()
    local s = state
    if not s then return end
    -- screen name on entry; the current/hovered area (or "empty terrain") as it
    -- changes; the game's help line as the tooltip.
    ann:focus(s.screen, nil, s.name or s.screen, nil, Keyhelp.helpmsg)
    -- Deferred quest-objective re-read: fire once, a few polls after opening, QUEUED
    -- (interrupt=false) so it follows the readout above instead of overlapping it.
    -- AREA MAP ONLY, and rate-limited (user 2026-07-28: "los objetivos se están anunciando un
    -- montón de veces, hasta cuando abro el world map y lo cierro"). Two things were wrong:
    -- `reset()` arms this, and reset runs on EVERY screen change — so opening the world map,
    -- closing it, and every dispatcher flip in between each re-armed a re-read. And the world map
    -- is not where anyone reviews the current objective; the area map is.
    -- So: never on the world map, and never twice within OBJ_REPEAT_S, which bounds the damage if
    -- some other path starts re-arming it again.
    if obj_read_at and tick >= obj_read_at and not s.world
        and (obj_said_at == nil or os.clock() - obj_said_at >= OBJ_REPEAT_S) then
        obj_read_at = nil
        obj_said_at = os.clock()
        pcall(function() require("quest_objective").reannounce(false) end)
    end
    if s.world then
        local host = ft_host()
        if not host then return end
        -- List the reachable fast-travel points once (so a blind player knows what's
        -- there). The d-pad selection itself runs on the FAST loop below, not here.
        -- NEVER LATCH AN EMPTY BUILD (user: "the d-pad on the world map works sometimes,
        -- especially not when I open the map several times", 2026-07-26). `dests_said` used to
        -- be set BEFORE knowing whether the build produced anything, and `ft_guidance` only
        -- rebuilds when `ft_points` is nil — an empty TABLE is not nil. So one unlucky first
        -- tick (the travel icons not materialised yet, or the native InfoIcon block not yet
        -- populated) latched an empty list for the WHOLE visit: no destinations announced and a
        -- dead d-pad, with everything else about the map working normally. Re-opening the map is
        -- exactly what re-rolls that dice, which is why it looked random and got worse the more
        -- the map was opened.
        --
        -- This is the items-menu rule again (2026-07-15): an adapter that collects a REBUILT
        -- screen's children must never cache an empty collection as final. The latch now closes
        -- only on success, so the build simply retries on the next 100 ms poll until the game has
        -- the points ready — and stops for good once it does.
        if not dests_said then
            ft_points = ft_build(host)
            if #ft_points > 0 then
                dests_said = true
                -- …and name the info key here, once, where the player is already being told
                -- what the list holds. X used to *look* like an info key by accident (see
                -- ft_describe) — saying so out loud is what turns that into a feature.
                Speech.say(string.format(I18n.t("map_travel_points"), #ft_points,
                    table.concat(ft_points, ", "))
                    .. ". " .. string.format(I18n.t("map_info_hint"), I18n.button("X")), false)
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

local travel_running = false

local function ft_step()
    local s = state
    -- The dispatch grid this screen needs, declared BEFORE every early return so a screen change
    -- can never leave the fast grid pinned (pad_poll.lua: slow by default, fast only on demand).
    -- The travel list's d-pad and its hold-to-repeat are the reason the fast grid exists at all.
    -- (The two early returns this replaces both did exactly `ft_idle(); return`.)
    local mine = not Transition.active() and s ~= nil and Registry.active_adapter() == Map
    PadPoll.demand_fast("map_travel", mine)
    if not mine then ft_idle() return end
    if not s.world then
        -- AREA map: no travel list to drive here, but the info key answers on this screen too —
        -- X re-reads the POI under the cursor, which area_poi() otherwise speaks only when the
        -- focus CHANGES. Same button, same meaning on both map screens.
        local snap = Input.read()
        local buttons = snap and snap.buttons or 0
        local hit_info = ft_pressed(buttons, Input.BTN.X)
        ft_prevbtn = buttons
        ft_hold_dir, ft_rpt_at = 0, 0   -- the auto-repeat belongs to the world-map list only
        if hit_info and s.host and Core.valid(s.host) then
            area_focus_key = nil   -- drop the diff-gate so the SAME focus speaks again
            area_poi(s.host)
        end
        return
    end
    local host = ft_host()
    if host then ft_guidance(host) end
end

-- Keyboard command for the world-map travel list: "next" / "prev" move through the points
-- (the arrows, mirroring the d-pad) and "select" travels (Enter, mirroring A). Ignored
-- unless the travel list is actually up — ft_guidance only runs there — so the same keys
-- stay free everywhere else.
function Map.key(cmd) ft_kb_cmd = cmd end

function Map.start()
    if travel_running then return end
    if not Input.is_loaded() then
        print("[KakarotAccess] screen_map: input bridge not loaded, travel d-pad disabled\n")
        return
    end
    travel_running = true
    -- Shared 20ms scheduler (pad_poll.lua): ft_step early-outs off-map, so the idle cost
    -- is one gate check inside the shared dispatch instead of a whole loop of our own.
    PadPoll.register("map_travel", ft_step)
end

function Map.stop()
    travel_running = false
    PadPoll.unregister("map_travel")
end

return Map
