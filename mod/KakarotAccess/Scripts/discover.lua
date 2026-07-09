-- Discovery tooling (dev only) — a FOCUSED on-screen dump for the screens we still
-- can't read (loading dialogs, pause menu, tutorials).
--
-- Output convention: every dump is written to mod/KakarotAccess/Scripts/dumps/ (next to
-- the source), as dump_<time>_<NNN>.txt. Dumps live with the source, NOT in the game
-- folder — only the mod itself runs from the game (via the Scripts junction).
--
-- Two jobs:
--   1) container on-screen signals — per live instance, IsVisible / GetVisibility enum /
--      RenderOpacity / IsInViewport / parent-chain visibility side by side, so we can see
--      which one marks a REALLY-shown window vs a stale pooled one (IsVisible lies here).
--   2) pause-menu selection — the earlier dump showed every List_Bar identical, so the
--      selected-row signal is NOT on the bar. This dump goes wider: per bar it prints all
--      scalars + render transform (position/scale) + label colour + every child image,
--      and it ENUMERATES every widget under the pause instance to catch a separate cursor
--      widget that moves/positions onto the selected row.
--
-- Perf: FindAllOf is expensive, so the big object lists are fetched ONCE and reused (an
-- earlier version called FindAllOf per-container and hung the game). Not shipped.

local Discover = {}

local OUT_DIR = "D:\\code\\unreal\\dragon ball kakarot access\\mod\\KakarotAccess\\Scripts\\dumps\\"

local CONTAINERS = {
    "Xcmn_Win00_C", "Xcmn_Win01_C",
    "Xcmn_Win00_Choice_C", "Xcmn_Win01_List_C",
    "Xcmn_Pause_C",
    -- field / overworld pause (Start menu) — a different widget from the battle pause.
    "Start_Top_C", "Start_Menu_C", "Start_Sub_C",
    "Loading_C", "Tips_C",
    "Battle_Tips_Tutorial_C",
    -- dialogue surfaces: check whether a faded battle voice line stays on_screen with stale
    -- text and shadows the pause menu (screen_dialogue is registered above the menus).
    "Xcmn_Subtitles_C", "Field_Talk_Win_C",
    "Gametitle_C", "Start_Option_C",
    -- transition/save-overlay suspects: to find a signal that suppresses the title menu
    -- while a save/fade is happening over it (when the title is enum 0 but not "the menu").
    "Xcmn_System_Icon_C", "Xcmn_Fade_Black_C", "Xcmn_Fade_C", "Xcmn_Fade_Blur_C",
}

local ESLATE = { [0]="Visible", [1]="Collapsed", [2]="Hidden", [3]="HitTestInvisible", [4]="SelfHitTestInvisible" }
local SCALAR = {
    BoolProperty=true, ByteProperty=true, EnumProperty=true,
    IntProperty=true, Int64Property=true, NameProperty=true, FloatProperty=true,
}

local function write_lines(path, lines)
    local f, err = io.open(path, "w")
    if not f then print("[Discover] could not open dump: " .. tostring(err) .. "\n") return end
    f:write(table.concat(lines, "\n")); f:write("\n"); f:close()
end

-- =====================================================================================
-- TEMP MAP CONFIRM TEST (2026-07-09) — on the WORLD MAP (Mapa del Mundo / UAT_UIMapWorld), F7
-- performs the INSTANT-TRAVEL recipe on a CYCLING target point, to validate it WITHOUT moving
-- the cursor: read state/sel/count, pick target = (prev+1) % count, WRITE selIndex(0x514)=target,
-- then reflection-call InputConfirm(). If it works the game should open the "Go to X?" YesNo for
-- the TARGET point (index 0,1,...) regardless of where the cursor is — tell me which point's
-- dialog appears (validates the mechanism AND the index->point mapping). Answer No to cancel.
-- state 6 and 0xc are both accepted by the confirm core. Set MAP_INPUT_TEST=false to restore
-- the normal dump. (Confirmed 2026-07-09: write selIndex + InputConfirm travels to that point;
-- index 0=Casa de Goku, 1=Ciénaga Blake. Real feature now lives in screen_map.lua.)
local MAP_INPUT_TEST = false

function Discover.run()
    ExecuteInGameThread(function()
        -- --- world-map confirm-test branch (returns early; no dump on the map) ---
        if MAP_INPUT_TEST then
            local ok_test, handled = pcall(function()
                local Speech = require("speech")
                local Mem = require("mem")
                local OFF = require("native_offsets").mapWorld
                local function tvalid(o) return o ~= nil and o:IsValid() == true end
                local function tlive(o) return tvalid(o) and o:GetFullName():find("/Engine/Transient", 1, true) ~= nil end
                local host = nil
                for _, o in pairs(FindAllOf("Map_World_C") or {}) do
                    if tlive(o) then host = o break end
                end
                if not tvalid(host) then return false end   -- not on the world map -> normal dump
                local state = Mem.i32(host, OFF.state)
                local sel   = Mem.i32(host, OFF.selIndex)
                local ready = Mem.u8(host, OFF.ready)
                local count = Mem.i32(host, OFF.infoIconCount) or 0
                if count < 1 then
                    Speech.say(string.format("no points. state %s, sel %s, ready %s",
                        tostring(state), tostring(sel), tostring(ready)), true)
                    return true
                end
                -- pick the next target index (cycles 0..count-1), write it, then confirm.
                local target = ((_G.__fttgt or -1) + 1) % count
                _G.__fttgt = target
                local wrote = Mem.write_i32(host, OFF.selIndex, target)
                local sel_after = Mem.i32(host, OFF.selIndex)
                local called = pcall(function() host:InputConfirm() end)
                Speech.say(string.format("target %d of %d, wrote %s, sel now %s, confirm %s",
                    target, count, tostring(wrote), tostring(sel_after), called and "called" or "FAILED"), true)
                print(string.format("[MapConfirm] state=%s ready=%s count=%s target=%d wrote=%s selAfter=%s called=%s\n",
                    tostring(state), tostring(ready), tostring(count), target,
                    tostring(wrote), tostring(sel_after), tostring(called)))
                return true
            end)
            if ok_test and handled then return end   -- handled on the world map; skip the normal dump
        end

        local out = {}
        _G.__dumpN = (_G.__dumpN or 0) + 1
        local stamp = 0; pcall(function() stamp = os.time() end)
        local outfile = string.format("%sdump_%d_%03d.txt", OUT_DIR, stamp, _G.__dumpN)
        local function flush() write_lines(outfile, out) end

        -- fetched ONCE (see perf note)
        local ALLTEXT = FindAllOf("CFUIMultiLineTextBox") or {}

        local function valid(o) return o ~= nil and o:IsValid() == true end
        local function live(o) return valid(o) and o:GetFullName():find("/Engine/Transient", 1, true) ~= nil end
        local function cname(o) return valid(o) and o:GetClass():GetFName():ToString() or "?" end
        local function short(fn) return fn:match("([^%.]+%.?[^%.]*)$") or fn end

        local function isvis(o)
            local ok, v = pcall(function() return o:IsVisible() end)
            return ok and tostring(v) or "err"
        end
        local function vis_enum(o)
            local ok, v = pcall(function() return o:GetVisibility() end)
            if not ok then return "err" end
            local n = tonumber(v)
            if n and ESLATE[n] then return n .. ":" .. ESLATE[n] end
            return tostring(v)
        end
        local function opacity(o)
            local ok, v = pcall(function() return o.RenderOpacity end)
            return ok and v ~= nil and string.format("%.2f", v) or "-"
        end
        local function color_a(o)
            local ok, v = pcall(function() return o.ColorAndOpacity.A end)
            return ok and v ~= nil and string.format("%.2f", v) or "-"
        end
        -- brush resource: "MaterialInstanceDynamic .../Foo" — the game often swaps the
        -- selected item's highlight to a DYNAMIC material (to animate it) while inactive
        -- items keep a shared MaterialInstanceConstant, so the brush TYPE can be the
        -- selection signal even when opacity/alpha are identical.
        local function brush_of(o)
            local res
            pcall(function()
                local ro = o.Brush.ResourceObject
                if ro and ro:IsValid() then res = cname(ro) .. " " .. (ro:GetName() or "?") end
            end)
            return res or "-"
        end
        local function in_viewport(o)
            local ok, v = pcall(function() return o:IsInViewport() end)
            return ok and tostring(v) or "-"
        end
        -- render transform: position + scale of the widget (a moving cursor / a scaled-up
        -- selected row shows here even when every other property is identical).
        local function xform(o)
            local ok, s = pcall(function()
                local t = o.RenderTransform
                return string.format("pos=%.0f,%.0f scale=%.2f,%.2f",
                    t.Translation.X, t.Translation.Y, t.Scale.X, t.Scale.Y)
            end)
            return ok and s or "-"
        end
        -- NOTE: guard EACH deref with valid() — a dangling-but-non-nil child pointer makes
        -- a method call abort UNCATCHABLY (pcall can't save it), which truncates the dump.
        local function label_color(node)
            if not valid(node) or not valid(node.mainTxt) then return "-" end
            local ok, c = pcall(function()
                local col = node.mainTxt.ColorAndOpacity
                return string.format("%.2f,%.2f,%.2f,%.2f", col.R, col.G, col.B, col.A)
            end)
            return ok and c or "-"
        end
        local function node_text(node)
            if not valid(node) or not valid(node.mainTxt) then return "" end
            local ok, s = pcall(function() return node.mainTxt.Text:ToString() end)
            return ok and s or ""
        end
        local function anim_playing(w, name)
            local ok, v = pcall(function() return w:IsAnimationPlaying(w[name]) end)
            return ok and tostring(v) or "-"
        end
        local function parent_chain(o)
            local parts, cur, depth = {}, o, 0
            while depth < 5 do
                local ok, p = pcall(function() return cur:GetParent() end)
                if not ok or not valid(p) then break end
                parts[#parts + 1] = cname(p) .. "(" .. vis_enum(p) .. ")"
                cur, depth = p, depth + 1
            end
            return #parts > 0 and table.concat(parts, " < ") or "(root/none)"
        end
        local function subtree_text(prefix, limit)
            local parts = {}
            for _, t in pairs(ALLTEXT) do
                if valid(t) and t:GetFullName():find(prefix, 1, true) then
                    local okv, s = pcall(function() return t.Text:ToString() end)
                    if okv and s and s ~= "" then
                        parts[#parts + 1] = s:gsub("%s+", " ")
                        if #parts >= limit then break end
                    end
                end
            end
            return #parts > 0 and table.concat(parts, " | ") or "(no text)"
        end

        -- 0) ALL VISIBLE ON-SCREEN TEXT — runs FIRST and flushes, because the later
        -- Start_Top ring reflection can abort UNCATCHABLY and truncate the dump. Each
        -- line shows the text and the OWNING widget path (…OwnerWidget.mainTxt), which is
        -- how we identify an unread screen (e.g. the cooking "Make a dish" course-select).
        -- Owning widget of a text node: walk up to the nearest ancestor whose class name
        -- ends in "_C" (a blueprint widget) or starts with "UAT_"/"AT_" (native UI), so an
        -- unread screen is named by its real widget class, not the generic WidgetTree path.
        local function owner_widget(o)
            local cur, depth = o, 0
            while depth < 12 do
                local ok, p = pcall(function() return cur:GetParent() end)
                if not ok or not valid(p) then break end
                local c = cname(p)
                if c:match("_C$") or c:match("^UAT_") or c:match("^AT_") then
                    return c .. "  [" .. short(p:GetFullName()) .. "]"
                end
                cur, depth = p, depth + 1
            end
            return short(o:GetFullName())
        end

        -- =================================================================================
        -- FAST TRAVEL probe (2026-07-09) — runs FIRST and flushes so a later abort can't
        -- truncate it. Open the WORLD MAP and press F7. To learn whether the game sets the
        -- manager's CurrentTargetComponent from the HOVERED destination (the key to a
        -- reflection-only instant travel: write it + confirm), take TWO dumps with the
        -- cursor on different points and compare "CURRENT target PlatformId".
        -- Reflection can abort on this game -> every hop is valid()+pcall.
        -- =================================================================================
        local function fname_of(obj, field)   -- read an FName UPROPERTY -> string, guarded
            local ok, v = pcall(function()
                local n = obj[field]
                if n == nil then return nil end
                if type(n) == "userdata" then return n:ToString() end
                return tostring(n)
            end)
            return ok and v or nil
        end
        out[#out + 1] = "==== FAST TRAVEL v2 (map widget / icons+brush / manager) ===="
        flush()
        do
            local function real(o) return valid(o) and not o:GetFullName():find("Default__", 1, true) end
            -- class chain "A < B < C" up to (and including) the first native UI base.
            local function chain(o)
                local parts, cls, d = {}, nil, 0
                pcall(function() cls = o:GetClass() end)
                while valid(cls) and d < 8 do
                    local cn = "?"; pcall(function() cn = cls:GetFName():ToString() end)
                    parts[#parts + 1] = cn
                    if cn:find("UserWidget") or cn == "Object" then break end
                    local sup; pcall(function() sup = cls:GetSuperStruct() end)
                    cls = valid(sup) and sup or nil; d = d + 1
                end
                return table.concat(parts, " < ")
            end
            -- read a text widget's text via GetText() then .Text (guarded).
            local function widget_text(node)
                if not valid(node) then return nil end
                local ok, s = pcall(function() return node:GetText():ToString() end)
                if ok and s and s ~= "" then return s end
                ok, s = pcall(function() return node.Text:ToString() end)
                return (ok and s ~= "") and s or nil
            end
            local function vec2(o, field)
                local ok, s = pcall(function() local v = o[field]; return string.format("%.1f,%.1f", v.X, v.Y) end)
                return ok and s or "-"
            end

            -- 1) MANAGER — FindAllOf (native gotcha: may be empty) + FindFirstOf fallback.
            local mgr = nil
            for _, m in pairs(FindAllOf("FastTravelManager") or {}) do if real(m) then mgr = m break end end
            if not mgr then pcall(function() local m = FindFirstOf("FastTravelManager"); if real(m) then mgr = m end end) end
            if valid(mgr) then
                out[#out + 1] = "-- MANAGER " .. short(mgr:GetFullName())
                local tgt; pcall(function() tgt = mgr.CurrentTargetComponent end)
                out[#out + 1] = valid(tgt)
                    and string.format("     CURRENT target=%s PlatformId=%s", cname(tgt), tostring(fname_of(tgt, "PlatformId")))
                    or  "     CURRENT target = (none/nil)"
                local um; pcall(function() um = mgr.UiMap end)
                out[#out + 1] = "     UiMap = " .. (valid(um) and (cname(um) .. " [" .. short(um:GetFullName()) .. "]") or "nil")
            else
                out[#out + 1] = "-- MANAGER: not found via FindAllOf/FindFirstOf(FastTravelManager)"
            end
            flush()

            -- 2) MAP WIDGET — the BP host is Map_World_C; its native parent is AT_UIFastTravelMap,
            -- so native members are readable by name. Read the hovered destination name, the area
            -- name, snap threshold, and the ordered DestinationIconList (the authoritative list).
            local host = nil
            for _, o in pairs(FindAllOf("Map_World_C") or {}) do if valid(o) and live(o) then host = o break end end
            if valid(host) then
                out[#out + 1] = "-- MAP HOST " .. chain(host)
                out[#out + 1] = "     DestinationName=" .. tostring(widget_text(host.WL_Text_DestinationName))
                    .. "  AreaName=" .. tostring(widget_text(host.WL_Text_AreaName))
                out[#out + 1] = "     CursorSnapThreshold=" .. vec2(host, "CursorSnapThreshold")
                    .. "  CursorMoveSpeed=" .. tostring((function() local ok,v=pcall(function() return host.CursorMoveSpeed end); return ok and v end)())
                -- DestinationIconList (TArray<FDestinationIconInfo{Icon,Slot}>) — ordered truth.
                local n = -1; pcall(function() n = #host.DestinationIconList end)
                out[#out + 1] = "     DestinationIconList count=" .. tostring(n)
                if n and n > 0 and n < 200 then
                    pcall(function()
                        for i = 1, n do
                            local e = host.DestinationIconList[i]
                            local ic = e and e.Icon
                            if valid(ic) then
                                out[#out + 1] = string.format("       [%d] %s", i, tostring(widget_text(ic.Txt_Name) or "?"))
                            end
                        end
                    end)
                end
                flush()
            else
                out[#out + 1] = "-- MAP HOST Map_World_C: not present"; flush()
            end

            -- 3) ICONS — each visible Map_World_Icon_C: class chain (confirm it derives from
            -- AT_UIFastTravelMapDestinationIcon) + name + EVERY Image/child ObjectProperty with its
            -- brush (the greyed-out texture is the per-icon LOCKED signal — the unlock filter).
            out[#out + 1] = "  -- Map_World_Icon_C children (find the icon image field + greyout brush):"
            flush()
            local shown = 0
            for _, ic in pairs(FindAllOf("Map_World_Icon_C") or {}) do
                if valid(ic) and live(ic) and shown < 4 then
                    shown = shown + 1
                    local nm; pcall(function() nm = widget_text(ic.Txt_Name) end)
                    out[#out + 1] = string.format("   ICON '%s'  %s", tostring(nm), chain(ic))
                    flush()
                    local cls; pcall(function() cls = ic:GetClass() end)
                    local d = 0
                    while valid(cls) and d < 6 do
                        pcall(function()
                            cls:ForEachProperty(function(prop)
                                local pn, pt = "?", "?"
                                pcall(function() pn = prop:GetFName():ToString() end)
                                pcall(function() pt = prop:GetClass():GetFName():ToString() end)
                                if pt == "ObjectProperty" then
                                    local ch; pcall(function() ch = ic[pn] end)
                                    if valid(ch) then
                                        local cc = "?"; pcall(function() cc = ch:GetClass():GetFName():ToString() end)
                                        local extra = ""
                                        if cc == "Image" then extra = " brush=" .. brush_of(ch) .. " vis=" .. isvis(ch) end
                                        out[#out + 1] = string.format("       .%s (%s)%s", pn, cc, extra)
                                    end
                                end
                            end)
                        end)
                        local cn = "?"; pcall(function() cn = cls:GetFName():ToString() end)
                        if cn:find("UserWidget") or cn == "Object" then break end
                        local sup; pcall(function() sup = cls:GetSuperStruct() end)
                        cls = valid(sup) and sup or nil; d = d + 1
                    end
                    flush()
                end
            end
        end
        out[#out + 1] = ""

        out[#out + 1] = "==== all visible on-screen text (plain) ===="
        flush()
        for _, o in pairs(ALLTEXT) do
            if valid(o) then
                local okv = pcall(function() return o:IsVisible() end)
                if okv and o:IsVisible() == true then
                    local ok2, s = pcall(function() return o.Text:ToString() end)
                    if ok2 and s and s ~= "" then
                        out[#out + 1] = string.format("  %q\n        <- %s\n        <- %s",
                            s, owner_widget(o), o:GetFullName())
                    end
                end
            end
        end
        flush()
        out[#out + 1] = ""
        out[#out + 1] = "==== all visible RICH text (CFExtendRichTextBlock, raw markup) ===="
        flush()
        for _, o in pairs(FindAllOf("CFExtendRichTextBlock") or {}) do
            if valid(o) then
                local okv = pcall(function() return o:IsVisible() end)
                if okv and o:IsVisible() == true then
                    local ok2, s = pcall(function() return o.Text:ToString() end)
                    if ok2 and s and s ~= "" then
                        out[#out + 1] = string.format("  %q  <- %s", s, short(o:GetFullName()))
                    end
                end
            end
        end
        flush()
        out[#out + 1] = ""

        -- 1) CONTAINER DIAGNOSTICS ---------------------------------------------------
        out[#out + 1] = "==== container on-screen signals ===="
        out[#out + 1] = "  IsVisible | GetVisibility | RenderOpacity | ColorA | InViewport | parents"
        flush()
        for _, cls in ipairs(CONTAINERS) do
            local any = false
            for _, o in pairs(FindAllOf(cls) or {}) do
                if live(o) then
                    any = true
                    out[#out + 1] = string.format("-- %s  [%s]", cls, short(o:GetFullName()))
                    out[#out + 1] = string.format("     vis=%s  enum=%s  op=%s  colA=%s  inVP=%s",
                        isvis(o), vis_enum(o), opacity(o), color_a(o), in_viewport(o))
                    out[#out + 1] = "     parents: " .. parent_chain(o)
                    out[#out + 1] = "     text: " .. subtree_text(o:GetFullName():match("%s(.+)$") or o:GetFullName(), 4)
                    flush()
                end
            end
            if not any then out[#out + 1] = "-- " .. cls .. "  (not present)" ; flush() end
        end

        -- 1b) ITEM PALETTE (Start_Item_Customize_C) — the register-palette popup. Runs
        -- EARLY: the field-pause ring section below dies on an uncatchable image read
        -- and silently truncates every section after it (seen in every dump today).
        -- Neither the host nor the bar blueprint is in the CXX dump, so this enumerates
        -- the live tree from the widget POOLS by path needle: per bar every Image/Border
        -- (name + vis + enum + brush + slot offsets — the cursor must differ between two
        -- dumps taken on different rows), and each bar's Xcmn_Btn_Plat state
        -- (controller-id/key arrays + Dmy_Btn brushes) so the slot's face button can be
        -- named.
        out[#out + 1] = ""
        out[#out + 1] = "==== item palette Start_Item_Customize_C (bars + cursor hunt) ===="
        flush()
        do
            local pal = nil
            for _, o in pairs(FindAllOf("Start_Item_Customize_C") or {}) do
                if live(o) then pal = o break end
            end
            if not valid(pal) then
                out[#out + 1] = "  (Start_Item_Customize_C not present)"
                flush()
            else
                out[#out + 1] = string.format("  host enum=%s isvis=%s", vis_enum(pal), isvis(pal))
                flush()
                local prefix = pal:GetFullName():match("%s(.+)$") or pal:GetFullName()
                -- NOTE: generic Image/Border pool sweeps ABORT uncatchably on this game
                -- (two truncated runs), so instead: recover each live object from the
                -- KNOWN-SAFE text pool by OUTER-walking, then CLASS-REFLECT its declared
                -- property names (safe) and read exactly those (declared -> safe reads).
                local widget_cls
                pcall(function() widget_cls = StaticFindObject("/Script/UMG.Widget") end)
                local function is_widget(o)
                    if not (widget_cls and widget_cls:IsValid()) then return false end
                    local ok, r = pcall(function() return o:IsA(widget_cls) end)
                    return ok and r == true
                end
                -- Dump every ObjectProperty child of `obj` (own class + supers up to
                -- UserWidget): name, class, and — for widgets — vis/enum/op (+ brush).
                local function dump_children(label, obj)
                    out[#out + 1] = string.format("  %s cls=%s", label, obj:GetClass():GetFullName())
                    flush()
                    local cls = obj:GetClass()
                    while cls and cls:IsValid() do
                        cls:ForEachProperty(function(prop)
                            local pn, pt = "?", "?"
                            pcall(function() pn = prop:GetFName():ToString() end)
                            pcall(function() pt = prop:GetClass():GetFName():ToString() end)
                            if pt == "ObjectProperty" then
                                local child
                                pcall(function() child = obj[pn] end)
                                if child and child:IsValid() then
                                    local ccls = "?"
                                    pcall(function() ccls = child:GetClass():GetFName():ToString() end)
                                    if is_widget(child) then
                                        local extra = (ccls == "Image") and (" brush=" .. brush_of(child)) or ""
                                        out[#out + 1] = string.format(
                                            "    .%s (%s) vis=%s enum=%s op=%s%s",
                                            pn, ccls, isvis(child), vis_enum(child), opacity(child), extra)
                                    else
                                        out[#out + 1] = string.format("    .%s (%s) [non-widget]", pn, ccls)
                                    end
                                    flush()
                                end
                            end
                        end)
                        local oks, sup = pcall(function() return cls:GetSuperStruct() end)
                        cls = (oks and sup) or nil
                        if cls and cls:IsValid() then
                            local cn = "?"
                            pcall(function() cn = cls:GetFName():ToString() end)
                            if cn == "UserWidget" or cn == "Object" or cn == "CFUIUserWidget" then break end
                        end
                    end
                end
                -- The host's own declared children (a single MOVING cursor would be here).
                dump_children("HOST", pal)
                -- One bar per row, recovered by outer-walking from its Txt_List text box
                -- (mainTxt -> tree -> Txt_List -> tree -> Start_Item_Bar0N).
                local bars = {}
                for _, o in pairs(FindAllOf("CFUIMultiLineTextBox") or {}) do
                    if valid(o) then
                        local fn = o:GetFullName()
                        if fn:find(prefix, 1, true) and fn:find(".Txt_List.", 1, true) then
                            local bi = fn:match("%.Start_Item_Bar0(%d)%.")
                            if bi then
                                local cur = o
                                for _ = 1, 6 do
                                    local oko, outer = pcall(function() return cur:GetOuter() end)
                                    if not oko or not outer or not outer:IsValid() then break end
                                    cur = outer
                                    -- The bar WIDGET's full name ENDS at its own name.
                                    if cur:GetFullName():match("%.Start_Item_Bar0" .. bi .. "$") then
                                        bars[tonumber(bi) + 1] = cur
                                        break
                                    end
                                end
                            end
                        end
                    end
                end
                for i = 1, 4 do
                    if bars[i] and bars[i]:IsValid() then
                        dump_children(string.format("BAR%02d", i - 1), bars[i])
                    else
                        out[#out + 1] = string.format("  BAR%02d not recovered", i - 1)
                        flush()
                    end
                end
                -- Each bar's plat-button state (native UAT_UIXcmnPlatBtn members).
                for _, o in pairs(FindAllOf("Xcmn_Btn_Plat_C") or {}) do
                    if valid(o) then
                        local fn = o:GetFullName()
                        if fn:find(prefix, 1, true) then
                            local ids = "-"
                            pcall(function()
                                local a, parts = o.CurrentDynamicAssignInputControllerId, {}
                                for i = 1, #a do parts[#parts + 1] = a[i]:ToString() end
                                ids = table.concat(parts, "|")
                            end)
                            local keys = "-"
                            pcall(function()
                                local a, parts = o.CurrentKeyIds, {}
                                for i = 1, #a do parts[#parts + 1] = tostring(tonumber(a[i])) end
                                keys = table.concat(parts, "|")
                            end)
                            out[#out + 1] = string.format(
                                "  PLAT %s vis=%s ctrlIds=%s keyIds=%s",
                                fn:sub(#prefix + 1):match("^[^%s]*") or "?", isvis(o), ids, keys)
                            for i = 0, 3 do
                                local img
                                pcall(function() img = o["Dmy_Btn_" .. string.format("%02d", i)] end)
                                if valid(img) then
                                    out[#out + 1] = string.format("    Dmy_Btn_%02d vis=%s brush=%s",
                                        i, isvis(img), brush_of(img))
                                end
                            end
                            flush()
                        end
                    end
                end
            end
        end

        -- 2c) TUTORIAL rows: the raw button-glyph markup (Txt_Btn plain + rich + reflected
        --     GetText) + action, so we can see where the glyph lives and why it isn't
        --     resolving to a button name. Runs BEFORE the pause section, which can truncate.
        out[#out + 1] = ""
        out[#out + 1] = "==== tutorial rows (Txt_Btn plain/rich/gettext + Txt_Operat) ===="
        flush()
        local tut = nil
        for _, o in pairs(FindAllOf("Battle_Tips_Tutorial_C") or {}) do if live(o) then tut = o break end end
        if not valid(tut) then
            out[#out + 1] = "  (Battle_Tips_Tutorial_C not present)"
            out[#out + 1] = "  tutorial container enum=" .. (valid(tut) and vis_enum(tut) or "n/a")
        else
            out[#out + 1] = "  tutorial container enum=" .. vis_enum(tut) .. " isvis=" .. isvis(tut)
            local function rich_of(node)
                if not (valid(node) and valid(node.ExMainTxt)) then return "" end
                local ok, s = pcall(function() return node.ExMainTxt.Text:ToString() end)
                return ok and s or ""
            end
            local function gettext_of(node)
                if not valid(node) then return "" end
                local ok, s = pcall(function() return node:GetText():ToString() end)
                return ok and s or ""
            end
            for i = 0, 8 do
                local row = tut["Battle_Tips_List_0" .. i]
                if valid(row) then
                    out[#out + 1] = string.format("  row %d  vis=%s enum=%s  operat=%q",
                        i, isvis(row), vis_enum(row), node_text(row.Txt_Operat))
                    out[#out + 1] = string.format("        btn_plain=%q  btn_rich=%q  btn_gettext=%q",
                        node_text(row.Txt_Btn), rich_of(row.Txt_Btn), gettext_of(row.Txt_Btn))
                    flush()
                end
            end
        end

        -- 2) PAUSE MENU — container-level only. The old per-row deep dive (List_Bar props +
        -- all-widgets enumeration) HALTS the dump on this game (a List_Bar00 read aborts), and
        -- pause selection is already handled via the cursor-glow alpha, so only the safe
        -- container/enum + per-bar glow alpha is reported now.
        out[#out + 1] = ""
        out[#out + 1] = "==== pause menu (container + per-bar glow alpha) ===="
        flush()
        do
            local pause = nil
            for _, o in pairs(FindAllOf("Xcmn_Pause_C") or {}) do if live(o) then pause = o break end end
            if not valid(pause) then
                out[#out + 1] = "  (Xcmn_Pause_C not present)"
            else
                -- Probe several candidate "selected row" signals side by side so one dump
                -- reveals which (if any) diverges on the highlighted row: the two cursor-glow
                -- images' alpha + render opacity, and the PnlCursEff border content alpha.
                local function content_a(o)
                    local ok, v = pcall(function() return o.ContentColorAndOpacity.A end)
                    return ok and v ~= nil and string.format("%.2f", v) or "-"
                end
                local function brush_a(o)  -- UBorder.BrushColor alpha
                    local ok, v = pcall(function() return o.BrushColor.A end)
                    return ok and v ~= nil and string.format("%.2f", v) or "-"
                end
                out[#out + 1] = string.format("  Xcmn_Pause_C enum=%s isvis=%s", vis_enum(pause), isvis(pause))
                for i = 0, 4 do
                    local bar = pause["List_Bar0" .. i]
                    if valid(bar) then
                        -- PnlCursEff (the cursor Border) is the prime suspect: cA=content alpha,
                        -- bA=brush alpha, op=render opacity. Plus the two glow images' alpha.
                        out[#out + 1] = string.format(
                            "  Bar%d enum=%s | eff00 A=%s eff01 A=%s | pnl cA=%s bA=%s op=%s | barOp=%s",
                            i, vis_enum(bar),
                            color_a(bar.Ins_Curs_Hexa_Eff00), color_a(bar.Ins_Curs_Hexa_Eff01),
                            content_a(bar.PnlCursEff), brush_a(bar.PnlCursEff), opacity(bar.PnlCursEff),
                            opacity(bar))
                    end
                end
            end
            flush()
        end

        -- 2c) FIELD PAUSE (Start_Top_C) — the overworld ring menu (different widget from the
        -- battle pause). Labels are image-font (no text); the icon brush texture identifies each
        -- option and Ins_List_On / the cursor images mark the selected one. Also dump native
        -- header text in case the current option's name appears there.
        out[#out + 1] = ""
        out[#out + 1] = "==== field pause Start_Top_C (ring) ===="
        flush()
        do
            local st = nil
            for _, o in pairs(FindAllOf("Start_Top_C") or {}) do if live(o) then st = o break end end
            if not valid(st) then
                out[#out + 1] = "  (Start_Top_C not present)"
                flush()
            else
                out[#out + 1] = string.format("  Start_Top_C enum=%s isvis=%s", vis_enum(st), isvis(st))
                flush()
                -- Every access wrapped in pcall that CAPTURES the error string (the game does NOT
                -- crash — a failed read is a catchable Lua error), so the dump never stops and we
                -- learn WHY each read fails (usually a wrong property name).
                local function tryget(obj, field)
                    local ok, v = pcall(function() return obj[field] end)
                    if not ok then return nil, tostring(v) end
                    return v, nil
                end
                local function readimg(obj, field)
                    local child, err = tryget(obj, field)
                    if err then return field .. " ERR(" .. err .. ")" end
                    if child == nil then return field .. "=nil" end
                    return string.format("%s vis=%s brush=%s", field, isvis(child), brush_of(child))
                end
                local hdr, herr = tryget(st, "UIXCmnHeader")
                out[#out + 1] = "  UIXCmnHeader = " .. (herr and ("ERR(" .. herr .. ")") or tostring(hdr and valid(hdr)))
                flush()
                if valid(hdr) then
                    -- Named reads (from AT.hpp UAT_UIXCmnHeader) — each pcall'd via readimg, so a
                    -- wrong name prints ERR(...) and the dump keeps going instead of stopping.
                    for _, nm in ipairs({ "Image_HeaderFont", "Image_IconFont", "Image_Icon", "Image_IconCaption" }) do
                        out[#out + 1] = "    hdr." .. readimg(hdr, nm)
                        flush()
                    end
                end
                -- Ring items: reflect each item's Image children + brushes (real names, pcall'd).
                for i = 0, 6 do
                    local it, ierr = tryget(st, "Start_Top_List0" .. i)
                    out[#out + 1] = string.format("  List0%d = %s", i, ierr and ("ERR(" .. ierr .. ")") or tostring(it and valid(it)))
                    flush()
                    if valid(it) then
                        out[#out + 1] = "    FontOn: " .. readimg(it, "Image_FontOn") .. " | Font: " .. readimg(it, "Image_Font")
                        flush()
                    end
                end
            end
        end

        -- 2d) TIPS_C nodes — the tutorial tip pages read by screen_loading. Show each text
        -- node's PLAIN (mainTxt) + RICH (ExMainTxt) so we can see whether the control glyphs
        -- are inline <inputicon> markup (resolvable) or separate images.
        out[#out + 1] = ""
        out[#out + 1] = "==== Tips_C per-window structure (validate screen_tips) ===="
        flush()
        do
            local function gettext(n)
                if not valid(n) then return "" end
                local ok, s = pcall(function() return n:GetText():ToString() end)
                return ok and s or ""
            end
            -- Slot offsets (a buffered/off-screen window should sit far off the visible area,
            -- the current one centred) — the likeliest "which window is current" signal.
            local function slot_pos(w)
                local ok, s = pcall(function()
                    local sl = w.Slot
                    local o = sl.LayoutData.Offsets
                    local z = "?"; pcall(function() z = tostring(sl.ZOrder) end)
                    return string.format("L=%.0f T=%.0f Z=%s", o.Left, o.Top, z)
                end)
                return ok and s or "-"
            end
            local tips = nil
            for _, o in pairs(FindAllOf("Tips_C") or {}) do if live(o) then tips = o break end end
            if not valid(tips) then
                out[#out + 1] = "  (Tips_C not present)"
            else
                -- The window-pointer array order (index 0 may track the current page).
                pcall(function()
                    local arr = tips.UITipsWin_List
                    for i = 1, #arr do
                        out[#out + 1] = string.format("  UITipsWin_List[%d] page=%q", i, gettext(arr[i].Txt_Page))
                    end
                end)
                for _, wn in ipairs({ "Tips_Win00", "Tips_Win01" }) do
                    local w = tips[wn]
                    if valid(w) then
                        out[#out + 1] = string.format("-- %s vis=%s enum=%s op=%s slot=[%s] anIn=%s anOut=%s",
                            wn, isvis(w), vis_enum(w), opacity(w), slot_pos(w),
                            anim_playing(w, "Anim_In"), anim_playing(w, "Anim_Out"))
                        out[#out + 1] = string.format("   title=%q sub=%q page=%q",
                            gettext(w.Txt_Title), gettext(w.Txt_Subtitle), gettext(w.Txt_Page))
                        for _, cv in ipairs({ "Canvas_TextOnly", "Canvas_TextAndImage", "Canvas_KeyTips", "Canvas_BigImage" }) do
                            out[#out + 1] = string.format("   %s vis=%s enum=%s", cv, isvis(w[cv]), vis_enum(w[cv]))
                        end
                        for _, d in ipairs({ "Txt_Detail00", "Txt_Detail01", "Txt_Detail03", "Txt_Detail04" }) do
                            local t = gettext(w[d])
                            if t ~= "" then out[#out + 1] = string.format("   %s vis=%s = %q", d, isvis(w[d]), t) end
                        end
                        for _, it in ipairs({ "Tips_List00_L00_00", "Tips_List00_L00_01", "Tips_List00_R00_00" }) do
                            local item = w[it]
                            if valid(item) then
                                out[#out + 1] = string.format("   %s vis=%s name=%q btn=%q",
                                    it, isvis(item), gettext(item.Txt_Detail), gettext(item.Txt_Btn))
                            end
                        end
                        flush()
                    end
                end
            end
            flush()
        end

        print("[Discover] wrote focused dump to " .. outfile .. "\n")
    end)
end

return Discover
