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

        -- 4) all visible RICH text (CFExtendRichTextBlock) — where <inputicon> glyph markup
        --    lives. The control tutorials show a button glyph per line; this reveals whether
        --    it's inline markup (resolvable to a button name) and in which widget subtree.
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
        print("[Discover] wrote focused dump to " .. outfile .. "\n")
    end)
end

return Discover
