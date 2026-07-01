-- Discovery tooling (dev only) — used to learn the game's real widget structure
-- before writing any menu-reading logic. Instead of printing to the UE4SS console
-- (painful to read with a screen reader), it writes a plain-text dump to a fixed
-- absolute path that the developer/assistant can open directly.
--
-- Not part of the shipping mod: once the main menu is mapped, this can be removed.

local Discover = {}

-- Fixed output path in the source repo, so it can be read regardless of where the
-- mod is deployed. Absolute on purpose (UE4SS's relative cwd is unpredictable).
local OUT = "D:\\code\\unreal\\dragon ball kakarot access\\mod\\KakarotAccess\\Scripts\\dump.txt"
local OUT_PROBE = "D:\\code\\unreal\\dragon ball kakarot access\\mod\\KakarotAccess\\Scripts\\probe.txt"
-- Each run() writes a NEW numbered file here, so several F7 dumps can pile up and
-- be compared. Counter lives in _G so it keeps incrementing across F7 presses
-- (require cache is cleared per press, but globals persist).
local OUT_DIR = "D:\\code\\unreal\\dragon ball kakarot access\\mod\\KakarotAccess\\Scripts\\dumps\\"

local function write_lines(path, lines)
    local f, err = io.open(path, "w")
    if not f then
        print("[Discover] could not open dump file: " .. tostring(err) .. "\n")
        return
    end
    f:write(table.concat(lines, "\n"))
    f:write("\n")
    f:close()
    print("[Discover] wrote " .. #lines .. " lines to " .. path .. "\n")
end

-- Dump every live UserWidget (class + full name + visibility) AND every non-empty
-- TextBlock (class + text) in one file, so a single keypress captures the screen.
function Discover.dump()
    ExecuteInGameThread(function()
        local out = {}

        table.insert(out, "==== UserWidgets ====")
        local widgets = FindAllOf("UserWidget")
        if widgets then
            for _, w in pairs(widgets) do
                if w:IsValid() then
                    local cls = w:GetClass():GetFName():ToString()
                    local vis = "?"
                    local okv, v = pcall(function() return w:IsVisible() end)
                    if okv then vis = tostring(v) end
                    table.insert(out, string.format("  %s  (visible=%s)  %s",
                        cls, vis, w:GetFullName()))
                end
            end
        else
            table.insert(out, "  (none)")
        end

        table.insert(out, "")
        table.insert(out, "==== TextBlocks (non-empty) ====")
        local tbs = FindAllOf("TextBlock")
        if tbs then
            for _, tb in pairs(tbs) do
                if tb:IsValid() then
                    local ok, txt = pcall(function() return tb.Text:ToString() end)
                    if ok and txt and txt ~= "" then
                        table.insert(out, string.format("  [%s] \"%s\"",
                            tb:GetClass():GetFName():ToString(), txt))
                    end
                end
            end
        else
            table.insert(out, "  (none)")
        end

        write_lines(OUT, out)
    end)
end

-- Probe: figure out how to read the string out of Xcmn_MultiLineText_C. Walks the
-- live Gametitle menu's text nodes and, for each, tries a list of candidate
-- properties/getters, logging which one yields a non-empty string.
function Discover.probe()
    ExecuteInGameThread(function()
        -- The UE4SS "member function on nullptr" error is a C++ abort that pcall
        -- CANNOT catch: it kills this whole callback. So we can't just wrap and
        -- retry. Instead we leave a breadcrumb file BEFORE every risky call and
        -- stop at the first property that yields text. If a call aborts, the file
        -- tells us exactly which property/node was to blame.
        local out = { "==== Xcmn_MultiLineText_C probe (Gametitle_C_0) ====" }

        -- Only the live, selected item: List00's Txt_Select (visible=true earlier).
        -- Fewer objects touched = smaller crash surface.
        local target = "Gametitle_List00.WidgetTree_0.Txt_Select"

        local node = nil
        local nodes = FindAllOf("Xcmn_MultiLineText_C") or {}
        for _, n in pairs(nodes) do
            if n:IsValid() and n:GetFullName():find(target, 1, true)
               and n:GetFullName():find("Gametitle_C_0", 1, true) then
                node = n
                break
            end
        end

        if not node then
            table.insert(out, "target node not found: " .. target)
            write_lines(OUT_PROBE, out)
            return
        end

        table.insert(out, "node: " .. node:GetFullName())

        -- Reflect a class: dump its properties (name : type) and functions. Safe:
        -- only touches reflection objects, never a null instance.
        local function reflect(label, obj)
            out[#out + 1] = "==== " .. label .. " ===="
            write_lines(OUT_PROBE, out) -- breadcrumb before touching obj
            if obj == nil then table.insert(out, "  (nil)") return end
            local cls = obj:GetClass()
            table.insert(out, "class: " .. cls:GetFName():ToString())
            table.insert(out, "---- properties ----")
            pcall(function()
                cls:ForEachProperty(function(prop)
                    local pname = prop:GetFName():ToString()
                    local ptype = "?"
                    pcall(function() ptype = prop:GetClass():GetFName():ToString() end)
                    table.insert(out, string.format("  %s : %s", pname, ptype))
                end)
            end)
            table.insert(out, "---- functions ----")
            pcall(function()
                cls:ForEachFunction(function(fn)
                    table.insert(out, "  " .. fn:GetFName():ToString())
                end)
            end)
        end

        -- The text lives in the sub-widgets; reflect the primary one (mainTxt).
        reflect("mainTxt", node.mainTxt)

        write_lines(OUT_PROBE, out)
    end)
end

-- Reflect the menu container + one list item so we learn the property names to
-- navigate directly (avoids iterating the huge Xcmn_MultiLineText_C list, which
-- crashes the game). Touches only a couple of objects.
function Discover.structure()
    ExecuteInGameThread(function()
        local out = {}

        local function first_live(cls_name)
            local all = FindAllOf(cls_name) or {}
            for _, o in pairs(all) do
                if o:IsValid() and o:GetFullName():find("Gametitle_C_0", 1, true) then
                    return o
                end
            end
            return nil
        end

        local function reflect(label, obj)
            out[#out + 1] = "==== " .. label .. " ===="
            write_lines(OUT_PROBE, out)
            if obj == nil then table.insert(out, "  (nil)") return end
            table.insert(out, obj:GetFullName())
            local cls = obj:GetClass()
            table.insert(out, "-- properties --")
            pcall(function()
                cls:ForEachProperty(function(prop)
                    local t = "?"
                    pcall(function() t = prop:GetClass():GetFName():ToString() end)
                    table.insert(out, string.format("  %s : %s", prop:GetFName():ToString(), t))
                end)
            end)
            table.insert(out, "-- functions --")
            pcall(function()
                cls:ForEachFunction(function(fn)
                    table.insert(out, "  " .. fn:GetFName():ToString())
                end)
            end)
        end

        reflect("Gametitle_C", first_live("Gametitle_C"))
        reflect("Gametitle_List_C", first_live("Gametitle_List_C"))
        write_lines(OUT_PROBE, out)
    end)
end

-- Confirm the menu model: for every live Gametitle list text node, read the label
-- (mainTxt.Text) and whether the node is visible. The selected option is the one
-- whose Txt_Select node is visible.
function Discover.menu()
    ExecuteInGameThread(function()
        local out = { "==== Gametitle menu contents ====" }
        local nodes = FindAllOf("Xcmn_MultiLineText_C") or {}
        for _, n in pairs(nodes) do
            if n:IsValid() then
                local full = n:GetFullName()
                if full:find("Gametitle_C_0", 1, true)
                   and full:find("Gametitle_List0", 1, true)
                   and (full:find("Txt_Select", 1, true) or full:find("Txt_List", 1, true)) then
                    -- Short label: List0N + Txt_kind
                    local short = full:match("Gametitle_List0%d+.-Txt_%a+") or full
                    local vis = "?"
                    pcall(function() vis = tostring(n:IsVisible()) end)
                    local label = "?"
                    pcall(function() label = n.mainTxt.Text:ToString() end)
                    out[#out + 1] = string.format("  %s  visible=%s  \"%s\"", short, vis, label)
                    write_lines(OUT_PROBE, out) -- flush incrementally
                end
            end
        end
        write_lines(OUT_PROBE, out)
    end)
end

-- run(): the current hot-editable discovery step. main.lua's dev key re-requires
-- this file and calls run() each press, so we can iterate without a restart.
-- Options menu (Start_Option_C): reflect the container + one row class to find how
-- the SELECTED row is marked (cursor image? bool? Txt_Select?) and confirm the
-- name/value/tab property names.
function Discover.run()
    ExecuteInGameThread(function()
        local out = {}

        -- New numbered file per press: dumps/dump_001.txt, dump_002.txt, ...
        _G.__dumpN = (_G.__dumpN or 0) + 1
        local outfile = string.format("%sdump_%03d.txt", OUT_DIR, _G.__dumpN)

        local function valid(o) return o ~= nil and o:IsValid() == true end

        local function first_live(cls_name)
            local all = FindAllOf(cls_name) or {}
            for _, o in pairs(all) do
                if valid(o) and o:GetFullName():find("/Engine/Transient", 1, true) then
                    return o
                end
            end
            return nil
        end

        local function reflect(label, obj)
            out[#out + 1] = "==== " .. label .. " ===="
            write_lines(outfile, out) -- breadcrumb before touching obj
            if obj == nil then out[#out + 1] = "  (nil)" return end
            out[#out + 1] = obj:GetFullName()
            local cls = obj:GetClass()
            out[#out + 1] = "-- properties --"
            pcall(function()
                cls:ForEachProperty(function(prop)
                    local t = "?"
                    pcall(function() t = prop:GetClass():GetFName():ToString() end)
                    out[#out + 1] = string.format("  %s : %s", prop:GetFName():ToString(), t)
                end)
            end)
            out[#out + 1] = "-- functions --"
            pcall(function()
                cls:ForEachFunction(function(fn)
                    out[#out + 1] = "  " .. fn:GetFName():ToString()
                end)
            end)
        end

        local function vis(o)
            if not valid(o) then return "invalid" end
            local ok, v = pcall(function() return o:IsVisible() end)
            return ok and tostring(v) or "err"
        end
        local function txt(o)
            if not valid(o) then return nil end
            local m = o.mainTxt
            if not valid(m) then return nil end
            local ok, s = pcall(function() return m.Text:ToString() end)
            return ok and s or nil
        end

        -- STATE snapshot of the options menu (reproduce the bug, then F7). Shows the
        -- keyhelp tooltip and, per row, its name + whether Ins_Cursor_Fad is visible
        -- + its Txt_Enter, so we can see exactly what marks focus when it goes mute.
        local opt = first_live("Start_Option_C")
        out[#out + 1] = "tab title: " .. tostring(valid(opt) and (txt(opt.Txt_Title_Steam) or txt(opt.Txt_Title)))
        local kh = first_live("Xcmn_Keyhelp_C")
        out[#out + 1] = "keyhelp tooltip: " .. tostring(valid(kh) and txt(kh.Txt_Helpmsg_Main))
        out[#out + 1] = "==== rows ===="
        write_lines(outfile, out)

        local fadCount = 0
        local rows = FindAllOf("Xlist_Bar03_C") or {}
        -- Sort-ish by index via name for readability.
        for i = 0, 20 do
            for _, r in pairs(rows) do
                if valid(r) and r:GetFullName():find("Xlist_Bar03_" .. string.format("%02d", i), 1, true)
                   and r:GetFullName():find("Start_Option_C", 1, true) then
                    local f = vis(r.Ins_Cursor_Fad)
                    if f == "true" then fadCount = fadCount + 1 end
                    out[#out + 1] = string.format(
                        "  %02d  fad=%-5s  list=%q (vis=%s)  enter=%q (vis=%s)",
                        i, f, tostring(txt(r.Txt_List)), vis(r.Txt_List),
                        tostring(txt(r.Txt_Enter)), vis(r.Txt_Enter))
                end
            end
        end
        out[#out + 1] = "fad count: " .. fadCount
        write_lines(outfile, out)
    end)
end

return Discover
