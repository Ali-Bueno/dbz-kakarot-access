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
    "Loading_C", "Tips_C",
    "Battle_Tips_Tutorial_C",
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

function Discover.run()
    ExecuteInGameThread(function()
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

        -- 2) PAUSE MENU — wide dump to find the selected-row discriminator ------------
        out[#out + 1] = ""
        out[#out + 1] = "==== pause menu (find the SELECTED-row discriminator) ===="
        flush()
        local pause = nil
        for _, o in pairs(FindAllOf("Xcmn_Pause_C") or {}) do if live(o) then pause = o break end end
        if not valid(pause) then
            out[#out + 1] = "  (Xcmn_Pause_C not present)"
        else
            -- 2a) the container: all props + animation states
            out[#out + 1] = "-- Xcmn_Pause_C props/anims:"
            for _, a in ipairs({ "In", "Out", "Loop" }) do
                out[#out + 1] = string.format("     anim %-6s playing=%s", a, anim_playing(pause, a))
            end
            pcall(function()
                pause:GetClass():ForEachProperty(function(prop)
                    local t = "?"; pcall(function() t = prop:GetClass():GetFName():ToString() end)
                    if SCALAR[t] then
                        local pn = prop:GetFName():ToString()
                        local okv, v = pcall(function() return pause[pn] end)
                        if okv and v ~= nil then
                            out[#out + 1] = string.format("     %s = %s (%s)", pn, tostring(v), t)
                        end
                    end
                end)
            end)
            flush()
            -- 2b) each List_Bar: label + xform + colour + all scalars + all image children
            for i = 0, 4 do
                local bar = pause["List_Bar0" .. i]
                if valid(bar) then
                    out[#out + 1] = string.format("-- List_Bar0%d", i)
                    flush()   -- breadcrumb BEFORE the risky node reads, so a crash names the bar
                    -- NOTE: RenderTransform (xform) aborts uncatchably on these rows → omitted.
                    out[#out + 1] = string.format("   txt=%q  vis=%s  enum=%s  op=%s  color=%s",
                        node_text(bar.Txt_List), isvis(bar), vis_enum(bar), opacity(bar),
                        label_color(bar.Txt_List))
                    pcall(function()
                        bar:GetClass():ForEachProperty(function(prop)
                            local t = "?"; pcall(function() t = prop:GetClass():GetFName():ToString() end)
                            local pn = prop:GetFName():ToString()
                            if SCALAR[t] then
                                local okv, v = pcall(function() return bar[pn] end)
                                if okv and v ~= nil then
                                    out[#out + 1] = string.format("      %-22s %s = %s", pn, t, tostring(v))
                                end
                            elseif t == "ObjectProperty" then
                                local okc, child = pcall(function() return bar[pn] end)
                                if okc and valid(child) and cname(child) == "Image" then
                                    out[#out + 1] = string.format("      %-22s img vis=%-5s op=%s colA=%s brush=%s",
                                        pn, isvis(child), opacity(child), color_a(child), brush_of(child))
                                end
                            end
                        end)
                    end)
                    flush()
                end
            end
            -- 2c) EVERY widget under the pause instance (catch a separate moving cursor).
            -- Needle is the object PATH (GetFullName minus the leading "ClassName "), since
            -- a child's own full name starts with ITS class, not the pause's.
            out[#out + 1] = "-- all widgets under pause (class | short | vis | enum | xform | brush):"
            local full = pause:GetFullName()
            local pfx = full:match("%s(.+)$") or full
            for _, cls in ipairs({ "UserWidget", "Image" }) do
                for _, w in pairs(FindAllOf(cls) or {}) do
                    if valid(w) and w:GetFullName():find(pfx, 1, true) then
                        out[#out + 1] = string.format("      %-24s %-30s vis=%-5s enum=%-20s brush=%s",
                            cname(w), short(w:GetFullName()), isvis(w), vis_enum(w),
                            cname(w) == "Image" and brush_of(w) or "-")
                    end
                end
            end
            flush()
        end

        -- 3) all visible on-screen text ----------------------------------------------
        out[#out + 1] = ""
        out[#out + 1] = "==== all visible on-screen text ===="
        flush()
        for _, o in pairs(ALLTEXT) do
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
        print("[Discover] wrote focused dump to " .. outfile .. "\n")
    end)
end

return Discover
