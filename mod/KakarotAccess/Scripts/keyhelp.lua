-- Keyhelp bar reader: the on-screen contextual button prompts ("X: use, Y: skill tree").
--
-- Xcmn_Keyhelp_C is the shared help bar shown in almost every menu (and in-game):
-- a row of (button glyph + action label) pairs, exposed as index-aligned
-- Txt_Keyhelp_NN / Xcmn_Btn_Plat_NN (NN = 01..09).
--
-- Naming the glyph takes TWO sources, because the bar's face buttons are drawn with
-- device-INDEXED textures (Btn00..03) that carry no meaning on their own:
--   1. the brush textures of its Dmy_Btn image nodes — the only source that names the
--      DIRECTION glyphs (Btn_Key_Ud/Lr/All) and combos (L1 + R1). I18n.keyhelp maps them;
--   2. failing that, the widget's own semantic ids via A.platbtn_name (the same resolver the
--      item/skill palettes use) — this is what names the FACE buttons (A/B/X/Y), which the
--      texture path deliberately leaves unmapped rather than fabricate a physical button.
--
-- Entries are classified nav (move / change tab — a direction glyph) vs action (a real
-- button press), so the auto-reader (keyhelp_watch.lua) can speak only the choices the
-- player actually has to make while F1/F2 still read the bar whole.

local Core = require("ui_core")
local Speech = require("speech")
local I18n = require("i18n")
local A = require("ui_archetypes")

local Keyhelp = {}

local MAX_ENTRIES = 9   -- Xcmn_Keyhelp_C exposes Txt_Keyhelp_01..09 / Xcmn_Btn_Plat_01..09
local MAX_GLYPH_IMAGES = 4   -- Xcmn_Btn_Plat_C exposes Dmy_Btn_00..03 (combo glyphs)

-- A glyph that means "move the cursor / the camera" rather than "press this to do X", by
-- the two ways it can be named: the bar's own direction textures, and the semantic d-pad
-- tokens A.platbtn_token resolves (EATPlatBtnId DpadLeft..DpadDown). Both come from the
-- game's data, not from guessed strings. The STICK textures (Stk_Nut_R = the character
-- menu's "Rotar") are navigation too — a stick sweep is not a choice the player makes;
-- a stick CLICK is, and it comes through as Btn_R3/Btn_L3, not as a Stk_ glyph.
local NAV_TEXTURE = { Btn_Key_Ud = true, Btn_Key_Lr = true, Btn_Key_All = true }
local NAV_TEXTURE_PREFIX = "Stk_"
local NAV_TOKEN = { Up = true, Down = true, Left = true, Right = true }

local function is_nav_texture(t)
    return NAV_TEXTURE[t] or t:sub(1, #NAV_TEXTURE_PREFIX) == NAV_TEXTURE_PREFIX
end

-- Button glyphs are named from the real texture the game shows in
-- /Art/UI/Xcmn/<platform>/; only confident semantic tokens have a spoken name (the
-- device-dependent indexed face-button glyphs are intentionally unnamed → the action
-- label alone is spoken). Spoken words come from the i18n layer (I18n.keyhelp).

-- The live, visible keyhelp bar (there may be stale hidden instances from other
-- screens, so prefer one that is actually on screen). Exposed so other adapters
-- (e.g. screen_options' tooltip) reuse the one bar-finding path instead of
-- re-scanning Xcmn_Keyhelp_C themselves.
--
-- visible_only: return nil rather than a hidden instance. The on-demand readers accept
-- the hidden fallback (better a stale bar than silence when the user asks), but the
-- auto-reader must NOT: a hidden bar left over from the previous screen would make it
-- announce actions that don't exist here.
--
-- tick: the caller's poll counter. Anything called from the reader loop MUST pass it —
-- Core.cached_all is the budgeted, event-fed cache, whereas a raw FindAllOf scans every
-- UObject in the game and stalls the game thread (it lagged the item/palette menus to a
-- standstill when this ran per-poll, 2026-07-14). The on-demand F2 read passes none: a
-- single scan on a keypress is free, and it must never miss the bar.
function Keyhelp.bar(visible_only, tick)
    local fallback
    for _, k in pairs(Core.cached_all("Xcmn_Keyhelp_C", tick)) do
        if Core.valid(k) and k:GetFullName():find("/Engine/Transient", 1, true) then
            fallback = fallback or k
            if Core.is_visible(k) then return k end
        end
    end
    if visible_only then return nil end
    return fallback
end

-- The keyhelp bar's contextual help message (Txt_Helpmsg_Main) — the one-line tooltip
-- shown for the focused control. nil if no bar / no message. Reused as the Options
-- menu's per-item tooltip.
function Keyhelp.helpmsg()
    local kh = Keyhelp.bar()
    return kh and Core.text_of(kh.Txt_Helpmsg_Main) or nil
end

-- The last path segment of a widget's brush texture: ".../Btn_L1.Btn_L1" -> "Btn_L1".
local function texture_token(img)
    if not Core.valid(img) then return nil end
    local res
    pcall(function()
        local ro = img.Brush.ResourceObject
        if ro and ro:IsValid() then res = ro:GetFullName() end
    end)
    return res and res:match("([%w_]+)%.[%w_]+$") or nil
end

-- Horizontal position of an entry on screen. The bar is a CanvasPanel whose row the game
-- lays out ITSELF (UAT_UIKeyHelp: KeyHelpRightMargin / KeyHelpItemGapX), so Txt_Keyhelp_NN's
-- number is a SLOT ID, not a place in the row — reading in index order announced "Atrás"
-- first and "Paleta de Súper Ataque" last, the reverse of what the player sees. The real
-- place is the canvas slot's own offset. nil if it can't be read (then order falls back to
-- the slot ids, which is at least stable).
-- Guarded on the slot's real class: calling a member a UObject does NOT have is an
-- UNCATCHABLE abort on this game, so GetPosition() is only ever called on a CanvasPanelSlot.
local function slot_of(w)
    if not Core.valid(w) then return nil, nil end
    local s, cn
    pcall(function()
        s = w.Slot
        if s and s:IsValid() then cn = s:GetClass():GetFName():ToString() end
    end)
    if not cn then return nil, nil end
    return s, cn
end

-- X of a widget from every source the engine exposes, most authoritative first. The canvas
-- slot's LayoutData offsets read back as 0.0 on EVERY entry of this bar (dump_keyhelp
-- 2026-07-14) — the row is not laid out through them — so GetPosition() and the render
-- transform are tried too, and finally the ancestors: on this UI a leaf often sits at 0
-- inside a per-entry container that carries the real placement.
local function widget_x(w)
    local s, cn = slot_of(w)
    if s and cn == "CanvasPanelSlot" then
        local x
        pcall(function()
            local p = s:GetPosition()
            if p then x = p.X end
        end)
        if type(x) == "number" and x ~= 0 then return x end
        x = nil
        pcall(function() x = s.LayoutData.Offsets.Left end)
        if type(x) == "number" and x ~= 0 then return x end
    end
    local rt
    pcall(function() rt = w.RenderTransform.Translation.X end)
    if type(rt) == "number" and rt ~= 0 then return rt end
    return nil
end

-- Walk up until some ancestor knows where it is (bounded; the bar is shallow).
local function slot_x(w)
    local cur, depth = w, 0
    while Core.valid(cur) and depth < 4 do
        local x = widget_x(cur)
        if x then return x end
        local p
        pcall(function() p = cur:GetParent() end)
        cur, depth = p, depth + 1
    end
    return nil
end

-- The texture tokens of a glyph's visible Dmy_Btn images (a combo glyph shows several).
local function glyph_tokens(plat)
    local toks = {}
    if not Core.valid(plat) then return toks end
    for i = 0, MAX_GLYPH_IMAGES - 1 do
        local img = plat["Dmy_Btn_" .. string.format("%02d", i)]
        if Core.is_visible(img) then
            local t = texture_token(img)
            if t then toks[#toks + 1] = t end
        end
    end
    return toks
end

-- Spoken name + nav flag for one glyph widget. The descriptive textures (directions,
-- shoulders, combos) name themselves; the indexed face-button textures don't, so those
-- fall through to the widget's semantic ids (A.platbtn_name). nil name if neither knows it
-- — then the caller speaks the action label alone rather than fabricate a button.
local function glyph_of(plat)
    if not Core.valid(plat) then return nil, false end
    local toks = glyph_tokens(plat)
    local parts, nav = {}, #toks > 0
    for _, t in ipairs(toks) do
        if not is_nav_texture(t) then nav = false end
        local nm = I18n.keyhelp(t)
        if nm then parts[#parts + 1] = nm end
    end
    if #parts > 0 then return table.concat(parts, I18n.t("combo_join")), nav end
    -- No spoken name from the textures. The nav VERDICT still stands, though: the stick
    -- glyphs (Stk_Nut_R = "Rotar") are navigation and have no name in any table, so a
    -- `return nil, false` here would silently promote them back to actions — which is how
    -- "Rotar" leaked into the announcement and, sitting right before "botón B: Atrás",
    -- sounded like Rotar WAS the B button (2026-07-14).
    local tok = A.platbtn_token(plat)
    if not tok then return nil, nav end
    return I18n.button(tok), nav or NAV_TOKEN[tok] or false
end

-- DIAGNOSTIC (2026-07-14, the A/B inversion in the item menu): on an F2 read, log the RAW
-- semantic sources behind every glyph — which of A.platbtn_token's paths answers, and with
-- what. The bar names A/B/X/Y through those ids, and if the game's ids follow the Japanese
-- face-button convention (A = right, B = bottom) while it DRAWS Xbox glyphs, every face
-- button we speak is mirrored. This tells us which, instead of guessing. Turn OFF after.
-- ANSWERED (dump_keyhelp 2026-07-14): none of the semantic id paths answer on this bar
-- (ctrl/act are empty), the pad-index path does — and the game's index order is the
-- Japanese one, which is why A/B came out mirrored. See FACE_TOKEN in ui_archetypes.lua.
-- It ALSO answered the on-screen ORDER question: the canvas slot's LayoutData offsets read
-- back 0.0 on every entry, so the row isn't placed through them — asking the slot itself
-- (GetPosition) is what returns the real place. That is why widget_x is a chain.
-- FILE-SAFE: the first cut wrote straight to the handle and a probe threw mid-loop, so the
-- handle never closed and the buffered lines were LOST — the dump came back EMPTY, which
-- reads exactly like "no data" and isn't. Collect first, then write once, and record the
-- error itself if one is thrown.
local DEBUG = false
local function dbg_dump(kh, entries)
    if not DEBUG or not kh then return end
    local lines = { string.format("[%d] ---- keyhelp bar ----", os.time()) }
    local ok, err = pcall(function()
        -- Every X source, on the widget AND up its ancestors: which one actually knows
        -- where this entry sits on screen?
        local function probe(w)
            local out, cur, depth = {}, w, 0
            while Core.valid(cur) and depth < 4 do
                local nm, gp, off, rt
                pcall(function() nm = cur:GetFName():ToString() end)
                local s, cn = slot_of(cur)
                if s then
                    pcall(function() local p = s:GetPosition() if p then gp = p.X end end)
                    pcall(function() off = s.LayoutData.Offsets.Left end)
                end
                pcall(function() rt = cur.RenderTransform.Translation.X end)
                out[#out + 1] = string.format("%s[%s getpos=%s off=%s rt=%s]",
                    tostring(nm), tostring(cn), tostring(gp), tostring(off), tostring(rt))
                local p
                pcall(function() p = cur:GetParent() end)
                cur, depth = p, depth + 1
            end
            return table.concat(out, " > ")
        end
        for n = 1, MAX_ENTRIES do
            local suffix = string.format("%02d", n)
            local txt = kh["Txt_Keyhelp_" .. suffix]
            if Core.is_visible(txt) then
                local plat = kh["Xcmn_Btn_Plat_" .. suffix]
                lines[#lines + 1] = string.format("  %s label=%q tex=%s -> token=%s x=%s",
                    suffix, tostring(Core.text_of(txt)),
                    table.concat(glyph_tokens(plat), "+"),
                    tostring(A.platbtn_token(plat)), tostring(slot_x(plat)))
                lines[#lines + 1] = "      plat: " .. probe(plat)
                lines[#lines + 1] = "      txt : " .. probe(txt)
            end
        end
        lines[#lines + 1] = "  spoken: " .. tostring(Keyhelp.phrase(entries))
    end)
    if not ok then lines[#lines + 1] = "  ERROR: " .. tostring(err) end
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_keyhelp.txt", "a")
        if not f then return end
        f:write(table.concat(lines, "\n"), "\n")
        f:close()
    end)
end

-- Public glyph naming for OTHER adapters showing Xcmn_Btn_Plat widgets (e.g. the item
-- palette's per-slot d-pad button): same texture-token -> word path as the help bar.
-- Guarded on the exact blueprint class: the Dmy_Btn_NN images glyph_name reads are
-- declared on Xcmn_Btn_Plat_C ONLY (Xcmn_Btn_Plat.hpp), not on the native
-- UAT_UIXcmnPlatBtn — probing them on a bare native instance would be the abort.
function Keyhelp.glyph(plat)
    if not Core.valid(plat) then return nil end
    local cn
    pcall(function() cn = plat:GetClass():GetFName():ToString() end)
    if cn ~= "Xcmn_Btn_Plat_C" then return nil end
    return (glyph_of(plat))
end

-- The current keyhelp entries, LEFT TO RIGHT as they sit on screen (the order a sighted
-- player reads them in): { {label=, button=, nav=}, ... }. visible_only / tick: see Keyhelp.bar.
function Keyhelp.read(visible_only, tick)
    local kh = Keyhelp.bar(visible_only, tick)
    if not kh then return {} end
    local entries, placed = {}, true
    for n = 1, MAX_ENTRIES do
        local suffix = string.format("%02d", n)
        local txt = kh["Txt_Keyhelp_" .. suffix]
        if Core.is_visible(txt) then
            local label = Core.text_of(txt)
            if label then
                local plat = kh["Xcmn_Btn_Plat_" .. suffix]
                local button, nav = glyph_of(plat)
                local x = slot_x(plat) or slot_x(txt)
                if not x then placed = false end
                entries[#entries + 1] = { label = label, button = button, nav = nav, x = x, slot = n }
            end
        end
    end
    -- Sort by on-screen position, and only if EVERY entry knows where it is — a partial
    -- sort would interleave the placed and the unplaced into an order matching nothing.
    if placed then
        table.sort(entries, function(a, b)
            if a.x == b.x then return a.slot < b.slot end
            return a.x < b.x
        end)
    end
    return entries
end

-- The bar's ACTION entries only (a real button press), dropping the cursor-movement ones.
-- What the auto-reader speaks: "move / change tab" is not a choice the player has to make.
function Keyhelp.actions(tick)
    local out = {}
    for _, e in ipairs(Keyhelp.read(true, tick)) do
        if not e.nav then out[#out + 1] = e end
    end
    return out
end

-- Identity of the bar from its LABELS alone — the cheap probe the auto-reader polls with.
-- Resolving the glyphs (Keyhelp.read) costs a pile of reflected reads per entry, far too
-- much to pay on every poll; the labels are plain text nodes and they change exactly when
-- the offered actions do. So: poll this, and only read the glyphs once it moves.
-- "" when no bar is on screen.
function Keyhelp.label_sig(tick)
    local kh = Keyhelp.bar(true, tick)
    if not kh then return "" end
    local parts = {}
    for n = 1, MAX_ENTRIES do
        local txt = kh["Txt_Keyhelp_" .. string.format("%02d", n)]
        if Core.is_visible(txt) then
            local label = Core.text_of(txt)
            if label then parts[#parts + 1] = label end
        end
    end
    return table.concat(parts, "|")
end

-- "button: action, ..." — the bare action label for entries whose glyph we can't name.
function Keyhelp.phrase(entries)
    local parts = {}
    for _, e in ipairs(entries) do
        parts[#parts + 1] = e.button and (e.button .. ": " .. e.label) or e.label
    end
    return #parts > 0 and table.concat(parts, ", ") or nil
end

-- Speak the whole bar on demand (F2) — nav entries included: the user asked for it, so
-- report the bar as it is on screen.
function Keyhelp.announce()
    local entries = Keyhelp.read()
    dbg_dump(Keyhelp.bar(), entries)
    Speech.say(Keyhelp.phrase(entries) or I18n.t("no_keyhelp"), true)
end

return Keyhelp
