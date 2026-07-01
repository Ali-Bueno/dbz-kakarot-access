-- Discovery tooling (dev only) — one-shot "menu snapshot" so we rarely need to iterate.
--
-- main.lua's F7 re-requires this fresh and calls Discover.run() each press. The default
-- run() is a GENERAL snapshot that usually gives everything needed to write a
-- screen_<name>.lua adapter in a single press:
--   1) visible UserWidget classes (identify the screen's container),
--   2) all visible on-screen text (the readable content),
--   3) a GENERIC per-widget breakdown of every content/item widget: each child node's
--      kind + value — text nodes show their string (+ rich markup), images show
--      visibility + brush texture. The selection marker is simply the image visible on
--      ONLY the selected item; labels/descriptions are the text nodes. No per-screen
--      edits needed — it reflects each widget's real properties.
--
-- Not part of the shipping mod.

local Discover = {}

local OUT_DIR = "D:\\code\\unreal\\dragon ball kakarot access\\mod\\KakarotAccess\\Scripts\\dumps\\"

-- Content widgets get a full breakdown; these leaf/chrome classes are too numerous or
-- carry nothing structural, so they're only listed in the class summary.
local LEAF = {
    Xcmn_MultiLineText_C = true, CFUIMultiLineTextBox = true,
    CFExtendRichTextBlock = true, Xcmn_Fade_Black_C = true,
    Xcmn_Btn_Plat_C = true, Image = true, CanvasPanel = true,
}
local MAX_INSTANCES = 14   -- skip breakdown for classes with more live instances (noise)
local MAX_WIDGETS = 60     -- overall cap on widgets broken down

local function write_lines(path, lines)
    local f, err = io.open(path, "w")
    if not f then print("[Discover] could not open dump: " .. tostring(err) .. "\n") return end
    f:write(table.concat(lines, "\n")); f:write("\n"); f:close()
    print("[Discover] wrote " .. #lines .. " lines to " .. path .. "\n")
end

function Discover.run()
    ExecuteInGameThread(function()
        local out = {}
        _G.__dumpN = (_G.__dumpN or 0) + 1
        local stamp = 0; pcall(function() stamp = os.time() end)
        local outfile = string.format("%sdump_%d_%03d.txt", OUT_DIR, stamp, _G.__dumpN)

        local function valid(o) return o ~= nil and o:IsValid() == true end
        local function cname(o) return o:GetClass():GetFName():ToString() end
        local function short(fn) return fn:match("([^%.]+%.[^%.]+%.?[^%.]*)$") or fn end
        local function vis(o)
            if not valid(o) then return "inv" end
            local ok, v = pcall(function() return o:IsVisible() end)
            return ok and tostring(v) or "err"
        end
        -- text of an inner text box (CFUIMultiLineTextBox / CFExtendRichTextBlock)
        local function box_text(o)
            if not valid(o) then return nil end
            local ok, s = pcall(function() return o.Text:ToString() end)
            if ok and s and s ~= "" then return s end
            return nil
        end
        -- plain + rich text of an Xcmn_MultiLineText_C node
        local function node_plain(o) return valid(o) and box_text(o.mainTxt) or nil end
        local function node_markup(o) return valid(o) and box_text(o.ExMainTxt) or nil end
        local function texture_of(o)
            local res
            pcall(function()
                local ro = o.Brush.ResourceObject
                if ro and ro:IsValid() then res = ro:GetFullName() end
            end)
            return res
        end

        -- 1) visible UserWidget classes (identify containers).
        local byClass, order = {}, {}
        for _, o in pairs(FindAllOf("UserWidget") or {}) do
            if valid(o) and vis(o) == "true" then
                local c = cname(o)
                if not byClass[c] then byClass[c] = { n = 0, ex = o:GetFullName() } order[#order + 1] = c end
                byClass[c].n = byClass[c].n + 1
            end
        end
        out[#out + 1] = "==== visible UserWidget classes ===="
        write_lines(outfile, out)
        for _, c in ipairs(order) do
            out[#out + 1] = string.format("  x%-3d  %s\n         ex: %s", byClass[c].n, c, byClass[c].ex)
        end

        -- 2) all visible on-screen text (safe: reads only leaf text boxes).
        out[#out + 1] = "==== visible on-screen text ===="
        write_lines(outfile, out)
        for _, o in pairs(FindAllOf("CFUIMultiLineTextBox") or {}) do
            if valid(o) and vis(o) == "true" then
                local s = box_text(o)
                if s then out[#out + 1] = string.format("  %q  <- %s", s, short(o:GetFullName())) end
            end
        end
        write_lines(outfile, out)

        -- Reflect a class's own properties (SAFE: touches only reflection objects, never
        -- an instance child — so it can't hit the uncatchable child-access abort).
        local function reflect_class(label, obj)
            out[#out + 1] = "==== " .. label .. " ===="
            write_lines(outfile, out)
            if not valid(obj) then out[#out + 1] = "  (invalid)" return end
            out[#out + 1] = obj:GetFullName()
            pcall(function()
                obj:GetClass():ForEachProperty(function(prop)
                    local t = "?"; pcall(function() t = prop:GetClass():GetFName():ToString() end)
                    out[#out + 1] = string.format("  %s : %s", prop:GetFName():ToString(), t)
                end)
            end)
        end

        local function first_vis(cls)
            for _, o in pairs(FindAllOf(cls) or {}) do
                if valid(o) and vis(o) == "true" and o:GetFullName():find("/Engine/Transient", 1, true) then return o end
            end
            return nil
        end

        -- CONFIRMATION POPUP structure. Reflect the shared Yes/No choice widget and the
        -- Win00 window class, and list every visible choice's label — safe, scoped.
        reflect_class("Xcmn_Win00_Choice_C", first_vis("Xcmn_Win00_Choice_C"))
        reflect_class("Xcmn_Win00_C", first_vis("Xcmn_Win00_C"))

        out[#out + 1] = "==== visible Xcmn_Win00_Choice_C (label + image nodes) ===="
        write_lines(outfile, out)
        for _, ch in pairs(FindAllOf("Xcmn_Win00_Choice_C") or {}) do
            if valid(ch) and vis(ch) == "true" then
                out[#out + 1] = string.format("-- %s  label=%q", short(ch:GetFullName()), tostring(node_plain(ch.Txt_Choice)))
                write_lines(outfile, out)
                pcall(function()
                    ch:GetClass():ForEachProperty(function(prop)
                        local t = "?"; pcall(function() t = prop:GetClass():GetFName():ToString() end)
                        if t ~= "ObjectProperty" then return end
                        local nm = prop:GetFName():ToString()
                        local child = ch[nm]
                        if valid(child) and cname(child) == "Image" then
                            out[#out + 1] = string.format("    %-18s vis=%-5s tex=%s", nm, vis(child), texture_of(child) or "-")
                        end
                    end)
                end)
            end
        end
        write_lines(outfile, out)
    end)
end

return Discover
