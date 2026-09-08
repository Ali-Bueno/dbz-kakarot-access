-- Regression: Dragon Balls register EMapIcon 28 directly with the minimap. They
-- need not have an ATMapIconComponent on their actor. Exercise the real radar
-- enumeration and manual selection; only the Unreal/native boundary is doubled.
local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local fails = 0
local function check(ok, message)
    if ok then print("ok   - " .. message)
    else fails = fails + 1; print("FAIL - " .. message) end
end
local function module(name, value)
    package.preload[name] = function() return value end
    return value
end
local next_addr = 1000
local function object(class, values)
    local o = values or {}
    next_addr = next_addr + 1
    o.addr, o.class, o.valid = next_addr, class, true
    function o:GetAddress() return self.addr end
    function o:GetClass()
        return { GetFName = function() return { ToString = function() return self.class end } end }
    end
    function o:K2_GetActorLocation() return { X = self.x or 0, Y = self.y or 0, Z = self.z or 0 } end
    function o:GetComponentByClass() return self.component end
    function o:IsA(cls)
        return cls.path == "/Script/AT.AT_UIMiniMapIcon"
            and (self.class == "AT_UIMiniMapIcon" or self.base == "AT_UIMiniMapIcon")
    end
    return o
end
local player = object("AT_Character", { x = 0, y = 0, z = 0 })
local mm = object("AT_UIMiniMapRadar", { PlayerIns = player, MapIconList = {}, visible = true })
local active_adapter, transition_active, native_unreadable = nil, false, false
local speech, scans = {}, {}
local pings, arrivals, stops, nav_tick = 0, 0, 0, nil
local clock_s = 10
os.clock = function() return clock_s end
local nav_errors, real_print = {}, print
print = function(message, ...)
    if tostring(message):find("nav .* error:") then nav_errors[#nav_errors + 1] = message end
    real_print(message, ...)
end
local native_offsets_read = {}
local pool = {}
module("ui_core", {
    valid = function(o) return type(o) == "table" and o.valid == true end,
    valid_ref = function(o) return type(o) == "table" end,
    member = function(o, key) return o and o[key] end,
    array_of = function(o, key)
        local a = o and o[key]
        if a then return a, #a end
    end,
    on_screen = function(o) return o and o.valid == true and o.visible == true or false end,
    pane_rendered = function(o) return o and o.valid == true and (o.opacity or 1) >= 0.05 or false end,
    cached_live = function(class) if class == "AT_UIMiniMapRadar" then return mm end end,
    cached_all = function() return {} end,
    peek_all = function() return {} end,
    take_scan_slot = function() return true end,
    scan_quiet = function() return false end,
    poll_world = function() end,
    findall = function(class)
        scans[class] = (scans[class] or 0) + 1
        return pool[class] or {}
    end,
})
module("mem", {
    mark = function() end,
    raw_addr = function(o) return o and o.addr end,
    u8 = function(o, off)
        native_offsets_read[off] = true
        if native_unreadable then return nil end
        return o and o.bytes and o.bytes[off]
    end,
})
module("speech", { say = function(s) speech[#speech + 1] = s end })
module("audio", {
    stop = function() stops = stops + 1 end,
    ping = function() pings = pings + 1 end,
    arrival = function() arrivals = arrivals + 1 end,
})
module("i18n", { t = function(key)
    if key == "nav_tracking" then return "Tracking %s, %d meters" end
    if key == "nav_meters" then return "%d meters" end
    if key == "cat_dragonball" then return "Dragon Ball" end
    if key == "nav_clock" then return "%d o'clock" end
    return key
end })
module("ui_registry", { active_adapter = function() return active_adapter end })
module("transition", { active = function() return transition_active end, on_begin = function() end })
module("raycast", {})
module("quest_objective", { item_requirement = function() return nil end })
module("pad_poll", { register_every = function(name, _, fn)
    if name == "nav" then nav_tick = fn end
end })
StaticFindObject = function(path) return object("Class", { path = path }) end
FindFirstOf = function() return nil end
FindAllOf = function(class) return pool[class] or {} end
ExecuteInGameThread = function(fn) fn() end

local Nav = require("nav_tracker")
local function marker(actor, kind)
    return object("AT_UIMiniMapIcon", {
        TargetActor = actor, WL_Owner = mm,
        WL_Icon_ImgSw = object("AT_UIMapIconSwitchImproved", { visible = true, opacity = 1 }),
        -- Independently recovered from native registration/type getter, not
        -- imported from production constants: active +0x88, EMapIcon +0x89.
        bytes = { [0x88] = 1, [0x89] = kind or 28 },
    })
end
local function group(key)
    local all = Nav.list_targets()
    Nav.sweep_partial()
    for _, g in ipairs(all) do if g.key == key then return g.items end end
    return {}
end
local ball = object("DragonBallStaticActor", { x = 3000, bHidden = false })
local icon = marker(ball)
mm.MapIconList = { icon }
local items = group("dragonball")
check(#items == 1 and items[1].actor == ball,
    "a displayed Dragon Ball without an actor map-icon component is listed")
check(#items == 1 and items[1].dist == 3000 and items[1].noun == "cat_dragonball",
    "the pickup's own position and Dragon Ball noun feed the ordinary picker")

local far_ball = object("DragonBallStaticActor", { x = 6000, bHidden = false })
mm.MapIconList = { marker(far_ball), icon, icon }
items = group("dragonball")
check(#items == 2 and items[1].actor == ball and items[2].actor == far_ball,
    "Dragon Balls sort nearest first and duplicate markers do not duplicate targets")

mm.MapIconList = { icon }
icon.bytes[0x88] = 0
check(#group("dragonball") == 0, "an inactive pooled marker with an old actor is not listed")
icon.bytes[0x88] = 1
icon.bytes[0x89] = 5
check(#group("dragonball") == 0, "a reused marker of another type is not a Dragon Ball")
icon.bytes[0x89] = 28
icon.WL_Icon_ImgSw.visible = false
check(#group("dragonball") == 0, "a game-hidden marker does not reveal a locked or unavailable ball")
icon.WL_Icon_ImgSw.visible = true
icon.WL_Icon_ImgSw.opacity = 0
check(#group("dragonball") == 0, "a fully faded marker is not listed")
icon.WL_Icon_ImgSw.opacity = 1
ball.bHidden = true
check(#group("dragonball") == 0, "a hidden pickup does not survive through its old marker")
ball.bHidden = false
ball.valid = false
check(#group("dragonball") == 0, "a destroyed pickup does not survive through its old marker")
ball.valid = true
icon.TargetActor = nil
check(#group("dragonball") == 0, "an empty marker does not create a target")
icon.TargetActor = ball
icon.WL_Owner = object("AT_UIMiniMapRadar", { visible = true })
check(#group("dragonball") == 0, "a marker belonging to another minimap is rejected")
icon.WL_Owner = mm
icon.class = "UnrelatedObject"
check(#group("dragonball") == 0, "native icon offsets are not interpreted on another class")
icon.class = "AT_UIMiniMapIcon"
icon.class, icon.base = "BlueprintMapIcon_C", "AT_UIMiniMapIcon"
check(#group("dragonball") == 1,
    "a derived minimap icon retains the native base layout and is not silently excluded")
icon.class, icon.base = "AT_UIMiniMapIcon", nil
native_unreadable = true
check(#group("dragonball") == 0, "unreadable native marker state does not invent a Dragon Ball")
native_unreadable = false

transition_active = true
check(#group("dragonball") == 0, "a world transition suppresses Dragon Ball reads")
transition_active = false
active_adapter = { nav_mute = true }
check(#group("dragonball") == 0, "a pausing menu suppresses Dragon Ball reads")
active_adapter = nil

local shop = object("ShopActor", { x = 2000 })
shop.component = object("ATMapIconComponent", { MapIconType = 1, bShowMapIcon = true, SearchRangeRadius = 10000 })
function shop.component:GetOwner() return shop end
pool.ATMapIconComponent = { shop.component }
mm.MapIconList = { icon, marker(shop, 1) }
check(#group("shops") == 1, "existing ordinary map-icon discovery and deduplication still work")
items = group("dragonball")
check(#items == 1, "ordinary map icons do not shadow component-free Dragon Balls")
check(scans.DragonBallStaticActor == nil and scans.DragonBallManager == nil,
    "Dragon Ball discovery adds no world-actor or manager scans")

check(Nav.set_manual_target(ball, tostring(ball.addr), "Dragon Ball", "dragonball", false) == true,
    "a current Dragon Ball can be selected through the existing manual tracking API")
check(speech[#speech] == "Tracking Dragon Ball, 30 meters",
    "selection announces the correct pickup distance through ordinary speech")
icon.bytes[0x88] = 0
local spoken_before = #speech
check(Nav.set_manual_target(ball, tostring(ball.addr), "Dragon Ball", "dragonball", false) == false,
    "a stale picker entry cannot start tracking a retired Dragon Ball")
check(#speech == spoken_before, "a rejected stale pick is not announced as tracking")
icon.bytes[0x88] = 1
check(#group("dragonball") == 1, "reactivation restores the ball without a restart")

ball.component = object("ATMapIconComponent", { MapIconType = 28, bShowMapIcon = true })
function ball.component:GetOwner() return ball end
pool.ATMapIconComponent = { shop.component, ball.component }
icon.WL_Icon_ImgSw.visible = false
check(#group("dragonball") == 0,
    "an actor component cannot bypass the displayed Dragon Ball marker requirement")
icon.WL_Icon_ImgSw.visible = true
check(#group("dragonball") == 1,
    "a displayed ball that also has a component is still listed exactly once")
ball.component, pool.ATMapIconComponent = nil, { shop.component }

native_unreadable = true
check(Nav._dragonball_available(ball) == nil,
    "an unreadable marker is unknown, not proof of collection")
native_unreadable = false
mm.MapIconList = {}
check(Nav._dragonball_available(ball) == false,
    "removal from a readable marker list retires a still-live pickup")
mm.MapIconList = { icon }

-- Exercise the real game-tick consumer too: a correct list alone must not leave
-- the beacon tracking a live actor after the game removes its marker.
Nav.start()
local function tick_once()
    clock_s = clock_s + 0.1
    nav_tick()
end
Nav.set_manual_target(ball, tostring(ball.addr), "Dragon Ball", "dragonball", false)
tick_once()
tick_once() -- 30 m uses a beacon interval longer than one 100 ms tick.
check(pings > 0, "a selected Dragon Ball uses the ordinary beacon tick")
local before_pings = pings
icon.bytes[0x88] = 0
tick_once()
check(pings == before_pings and speech[#speech] == "radar_chain_done",
    "retirement mid-approach stops the old beacon and completes an empty sweep")
Nav.where()
check(speech[#speech] == "nav_no_target", "the retired pickup is no longer an active target")

icon.bytes[0x88] = 1
local far_icon = marker(far_ball)
mm.MapIconList = { icon, far_icon }
Nav.set_manual_target(ball, tostring(ball.addr), "Dragon Ball", "dragonball", false)
mm.MapIconList = { far_icon }
tick_once()
check(speech[#speech] == "Tracking Dragon Ball, 60 meters",
    "marker removal mid-approach selects the next remaining ball")

mm.MapIconList = { icon, far_icon }
Nav.set_manual_target(ball, tostring(ball.addr), "Dragon Ball", "dragonball", false)
native_unreadable = true
tick_once()
Nav.where()
check(speech[#speech]:find("30 meters", 1, true) ~= nil,
    "a transient native read failure does not discard or advance the selected ball")
native_unreadable = false

player.x = ball.x
tick_once()
check(arrivals == 1 and speech[#speech] == "nav_arrived_pickup",
    "reaching the ball uses the existing arrival cue and waits for collection")
icon.WL_Icon_ImgSw.visible = false
tick_once()
check(speech[#speech] == "Tracking Dragon Ball, 30 meters",
    "retirement after arrival advances even when the collected actor stays alive")

-- A pausing menu is not collection. Test the arrival-wait lane, where the
-- existing world gate deliberately releases the actor and resumes by its key.
icon.WL_Icon_ImgSw.visible = true
Nav.set_manual_target(ball, tostring(ball.addr), "Dragon Ball", "dragonball", false)
tick_once()
active_adapter = { nav_mute = true }
local before_stop = stops
tick_once()
check(stops > before_stop, "a pausing menu stops the Dragon Ball beacon")
active_adapter = nil
local before_menu_resume = #speech
tick_once()
tick_once()
local resumed, falsely_done = false, false
for i = before_menu_resume + 1, #speech do
    if speech[i] == "Tracking Dragon Ball, 0 meters" then resumed = true end
    if speech[i] == "radar_chain_done" then falsely_done = true end
end
check(resumed and not falsely_done, "closing a menu resumes the reached ball without skipping it")

player.x = 0
Nav.set_manual_target(ball, tostring(ball.addr), "Dragon Ball", "dragonball", false)
active_adapter = { nav_mute = true }
tick_once()
active_adapter = nil
-- Let the actual deferred builder publish a healthy snapshot, then fail the
-- native read when the resume consumer attempts to accept that snapshot's pick.
for _ = 1, 4 do
    if Nav.targets_snap then break end
    tick_once()
end
check(Nav.targets_snap ~= nil, "menu resume builds a current picker snapshot")
native_unreadable = true
tick_once()
native_unreadable = false
for _ = 1, 80 do tick_once() end
Nav.where()
check(speech[#speech]:find("30 meters", 1, true) ~= nil,
    "a failed native read during resume is retried instead of losing the selected ball")
check(#nav_errors == 0, "the real navigation and explore ticks raise no harness errors")

if fails > 0 then print(fails .. " check(s) FAILED"); os.exit(1) end
print("all checks passed")
