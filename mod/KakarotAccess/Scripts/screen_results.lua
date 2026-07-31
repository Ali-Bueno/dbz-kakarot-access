-- Screen adapter: story results (Quest_Main_Clear_C -> UAT_UIQuestMainClear).
--
-- The post-battle "Story Results" sequence: one bar per battle ("Goku vs. Raditz")
-- with a RANK LETTER shown as an IMAGE, then a TOTAL rank; "View details" (X) expands
-- per-battle detail rows (Completion time / Max combo / Damage taken), each with its
-- own rank image and — for counters like max combo — a DIGIT-IMAGE number
-- (Image_PercentageList). Verified from the F7 dumps (2026-07-06) + CXX headers:
--   host UIClearBar_List (native TArray of bars); per bar TextBox_Item + Image_Rank
--   (native) and the DETAIL rows as the blueprint's DIRECT children
--   Quest_Main_Clear_Detail00..05 (Quest_Main_Clear_Bar.hpp — the native
--   UIClearDetail_List array stayed empty live, which left the expanded details
--   unread); total UIQuestMainClearRank.Image_Rank.
--
-- Rank letters and digit counters come from brush TEXTURE names (keyhelp technique).
-- The reader is INCREMENTAL: each line is queued exactly once as it becomes readable,
-- following the game's reveal animation. Lines are KEYED by bar index + content so
-- two battles with the same title (Goku vs. Raditz twice) both read.

local Core = require("ui_core")
local Speech = require("speech")
local I18n = require("i18n")

local Results = {}

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Results.keyhelp_auto = false

local ann = Core.make_announcer()   -- unused for speech (incremental), kept for API symmetry
local host = nil
local tick = 0
local spoken = {}    -- line KEY -> true. Cleared when the screen closes, NOT on reset().
local queue = {}     -- { {key=, text=}, ... } computed in is_active

-- DIAGNOSIS (user bug 2026-07-15): every detail row reads a CONSTANT "222" for its
-- value while labels and ranks are right — the digit images likely all share ONE
-- atlas texture whose name ends in a digit (texture-name parsing can't work then; the
-- real value would live in the row's unreflected tail, AT.hpp: Detail 0x3C0..0x418).
-- ROUND 2 (2026-07-17): the user's dump confirmed it — every digit image carries the
-- SAME material (Ins_Num_Result02), some as the shared MaterialInstanceConstant and
-- some as a per-widget MaterialInstanceDynamic. A per-widget MID + shared atlas means
-- the digit is selected by a MATERIAL PARAMETER, so the dump now also captures each
-- brush material's Scalar/Vector parameter values (+ parent). The next real results
-- screen tells us the parameter name/encoding; the reader fix follows.
-- Turn OFF once the value source is fixed and re-verified.
-- GATED on the dev build (2026-07-25): this dump walks brush MATERIALS, which is the
-- uncatchable-abort family this project has been burned by more than once, and it writes a
-- file on every results screen. It has to stay available — the digit decoder still depends
-- on it — but it must never reach a player. package.ps1 rewrites build_flags to debug=false.
local DEBUG = require("build_flags").debug
local dumped = {}    -- detail KEY -> true (reset with `spoken` when the screen closes)

local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    return dir .. "\\dumps\\dump_results.txt"
end

-- One image's render state + every material parameter behind it. Extracted from the loop
-- below on 2026-07-31 so the RANK image can be dumped through the identical path: the digit
-- turned out to be a material parameter rather than a texture name, and the rank is read by
-- the same name-parsing technique that was wrong for the digits — the user hears "S" on every
-- row while the screenshot shows an "A" among them. This is the capture that settles it.
local function dump_one_image(f, tag, img)
    if not Core.valid(img) then
        f:write(string.format("  %s (absent)\n", tag))
        return
    end
    local vis, full, tex = "?", "?", "(none)"
    pcall(function() vis = tostring(Core.is_visible(img)) end)
    pcall(function() full = img:GetFullName() end)
    local params = ""
    -- Same Brush.ResourceObject struct hop as texture_token, hardened the same way.
    local ro = Core.member_path(img, "Brush", "ResourceObject")
    pcall(function()
        if Core.nonnull(ro) then                  -- never ro:IsValid(), see Core.nonnull
            tex = ro:GetFullName()
            local sv, sn = Core.array_of(ro, "ScalarParameterValues")
            if sv then
                for k = 1, sn do
                    local nm, val = "?", "?"
                    pcall(function() nm = sv[k].ParameterInfo.Name:ToString() end)
                    pcall(function() val = tostring(sv[k].ParameterValue) end)
                    params = params .. string.format(" S:%s=%s", nm, val)
                end
            end
            local vv, vn = Core.array_of(ro, "VectorParameterValues")
            if vv then
                for k = 1, vn do
                    local nm, val = "?", "?"
                    pcall(function() nm = vv[k].ParameterInfo.Name:ToString() end)
                    pcall(function()
                        local c = vv[k].ParameterValue
                        val = string.format("(%.3f,%.3f,%.3f,%.3f)", c.R, c.G, c.B, c.A)
                    end)
                    params = params .. string.format(" V:%s=%s", nm, val)
                end
            end
            pcall(function()
                local p = ro.Parent
                if Core.valid(p) then
                    params = params .. " parent=" .. p:GetFullName()
                end
            end)
        end
    end)
    f:write(string.format("  %s vis=%s tex=%s\n      params=%s\n      widget=%s\n",
        tag, vis, tex, params, full))
end

-- Each row is dumped TWICE, not once (2026-07-31). The first write lands on the first tick the
-- row becomes readable — which is mid reveal-animation — so a row whose digits are still the
-- undriven template there may simply not have been filled in yet. A second pass a few reads
-- later separates "this row shows no number" from "we looked too early", which is the open
-- question for the time and damage rows and cannot be answered from a single sample.
local DUMP_PASSES = 2

local function debug_dump_detail(d, key, label, bar)
    local seen = (dumped[key] or 0) + 1
    if seen > DUMP_PASSES then return end
    dumped[key] = seen
    local f = io.open(dump_path(), "a")
    if not f then return end
    f:write(string.format("== %s label=%s pass=%d\n", key, tostring(label), seen))
    local arr, n = Core.array_of(d, "Image_PercentageList")
    if arr then
        pcall(function()
            for i = 1, n do dump_one_image(f, "[" .. i .. "]", arr[i]) end
        end)
    else
        f:write("  (Image_PercentageList unreadable)\n")
    end
    -- The RANK glyph, through the SAME path (2026-07-31). If it is an atlas material with a
    -- selector parameter, like the digits turned out to be, this names that parameter; if its
    -- material really is per-rank, the asset name will differ row to row and the current parse
    -- is simply reading the wrong image. Either way ONE capture decides it, instead of another
    -- round of reasoning about a value we can just look at.
    dump_one_image(f, "[rank]", Core.member(d, "Image_Rank"))
    -- ...and the BAR's own rank glyph, which is a different image on a different class: the
    -- per-row rank and the per-battle rank are read by the same parse, so both have to be in
    -- the capture or a fix for one could silently be wrong for the other.
    if Core.valid(bar) then dump_one_image(f, "[bar-rank]", Core.member(bar, "Image_Rank")) end
    f:close()
end

local DETAIL_COUNT = 6   -- Quest_Main_Clear_Detail00..05 (Quest_Main_Clear_Bar.hpp)

-- (`texture_token` and the RANKS letter set lived here until 2026-07-31. Both existed only to
-- parse a rank out of an ASSET NAME, which turned out to be reading the icon's SIZE suffix —
-- see rank_letter below, now decoded from the material like the digits. Deleted rather than
-- left dead: nothing else in the file called them.)

-- Every scalar parameter behind a material instance: itself first, then up the Parent chain.
-- Both halves are needed and they live on different objects — the per-widget MID overrides
-- only `Num`, while the atlas geometry (`columns`/`rows`) sits on the shared MIC it parents
-- to. Guarded idiom throughout (Core.array_of for the TArray, Core.struct_member for the
-- struct hops, Core.name_str for the FName), the same one screen_community uses.
local MAT_PARENT_HOPS = 3    -- MID -> MIC -> Material is two; one spare, and it cannot loop

local function mat_params(mat)
    local out, hops = {}, 0
    while Core.nonnull(mat) and hops < MAT_PARENT_HOPS do
        local sv, n = Core.array_of(mat, "ScalarParameterValues")
        if sv then
            pcall(function()
                for k = 1, n do
                    local nm = Core.name_str(
                        Core.struct_member(Core.struct_member(sv[k], "ParameterInfo"), "Name"))
                    local v = Core.struct_member(sv[k], "ParameterValue")
                    -- SELF WINS: the first material to name a parameter owns its value, so the
                    -- MID's live `Num` is never overwritten by the template's baked 0.
                    if nm and out[nm] == nil and type(v) == "number" then out[nm] = v end
                end
            end)
        end
        mat = Core.member(mat, "Parent")
        hops = hops + 1
    end
    return out
end

-- The digit is a MATERIAL PARAMETER, not a texture name. Decoded 2026-07-31 from
-- `dumps/dump_results.txt` — the capture STATUS.md had been waiting for since 2026-07-15, and
-- which only became possible today because the screen finally reads at all.
--
-- Each digit is an Image whose brush material is a MaterialInstanceDynamic over one shared
-- atlas (`Ins_Num_Result02`, `columns=5 rows=2` — the ten digits), carrying a single scalar
-- parameter `Num` that selects the cell AS A TENTH: 0.4 -> 4, 0.7 -> 7. VERIFIED against the
-- user's own screenshot on two independent values: Gohan's max combo (`_01`=0.4, `_02`=0.0)
-- = 40, Vegeta's (`_01`=0.7, `_02`=0.6) = 76.
--
-- Why the old parse said "222": it read the shared atlas's ASSET NAME and took its last digit,
-- so every image answered "2" — three visible images, "222". And on the combo row, where the
-- material is a MID whose full name ends in the WIDGET index, it took that instead and said
-- "21". Both are exactly what the user heard, which is what confirms the mechanism.
--
-- ORDER comes from the WIDGET NAME, never the array: the TArray runs in reverse screen order
-- ([1]=`_02`, [2]=`_01`, [3]=`_00`), so concatenating by index would read 40 as "04".
--
-- An image whose material is NOT a MID has no per-instance value — it is the shared template,
-- which the dump shows sitting at Alpha=0, i.e. a transparent placeholder. Those are skipped,
-- and a row with none of them returns nil, so the line reads its label and rank with NO number
-- instead of a fabricated one.
local NUM_PARAM = "Num"     -- scalar parameter selecting the atlas cell (dump_results.txt:55)
-- A per-widget MaterialInstanceDynamic is what carries a REAL value. An image still pointing at
-- the shared MaterialInstanceConstant is the undriven template — the dump shows it baked at
-- Num=0 AND Alpha=0, i.e. a transparent placeholder — so counting it would invent a "0" digit
-- and turn an empty row into "000". The engine class name is the authoritative test for that.
local LIVE_MAT_CLASS = "MaterialInstanceDynamic"

local function image_number(arr_owner, member)
    local arr, n = Core.array_of(arr_owner, member)
    if not arr then return nil end
    local slots, ok = {}, true
    pcall(function()
        for i = 1, n do
            local img = arr[i]
            if Core.valid(img) and Core.is_visible(img) then
                local d, ord
                local mat = Core.member_path(img, "Brush", "ResourceObject")
                if Core.nonnull(mat) then
                    local cls
                    pcall(function() cls = mat:GetClass():GetFName():ToString() end)
                    if cls == LIVE_MAT_CLASS then
                        local p = mat_params(mat)
                        -- Atlas geometry read from the material, never assumed: 5 x 2 = the ten
                        -- digits, and Num is the cell as a fraction of that.
                        local cells = (p.columns and p.rows) and (p.columns * p.rows) or nil
                        if p[NUM_PARAM] and cells and cells >= 1 then
                            local v = math.floor(p[NUM_PARAM] * cells + 0.5)
                            if v >= 0 and v <= cells - 1 then d = tostring(v) end
                        end
                    end
                end
                pcall(function()
                    ord = tonumber(img:GetFName():ToString():match("_(%d+)$"))
                end)
                -- STRICT: one visible digit that will not decode makes the whole number
                -- untrustworthy, and a truncated number ("4" for 40) is worse than none.
                if d and ord then slots[#slots + 1] = { ord = ord, d = d } else ok = false end
            end
        end
    end)
    if not ok or #slots == 0 then return nil end
    table.sort(slots, function(a, b) return a.ord < b.ord end)
    local digits = {}
    for i = 1, #slots do digits[i] = slots[i].d end
    return table.concat(digits)
end

-- The RANK is the SAME atlas trick as the digits (dump_results.txt, 2026-07-31): the image's
-- material is a per-widget MID over `Ins_Rate_S` on a criterion row, `Ins_Rate_M` on the battle
-- bar, carrying the same scalar `Num` that picks the cell as a fraction of the atlas.
--
-- WHY THE OLD PARSE SAID "S" ON EVERY ROW OF EVERY BATTLE: it took the trailing uppercase
-- letters of the asset NAME, and `Ins_Rate_S` ends in "_S" — which is the icon's SIZE (small;
-- the bar's is `Ins_Rate_M`, medium), not a rank at all. The size suffix merely collided with a
-- valid rank letter. And on the bar it parsed "M", which is not a rank, so the PER-BATTLE rank
-- line was never spoken at all — a second symptom that nobody had connected to the first.
--
-- THE SCALE IS S, A, B: three levels, no Z, source-verified four independent ways.
-- `CrowdResultRank` (AT_enums.hpp:414) and `RankAnimType` (:11648) both run S=0, A=1, B=2;
-- `UAT_UIQuestMainClearBar` and `UAT_UIQuestMainClearRank` each declare exactly three
-- animations (`Anim_StartRankS/A/B`), never a fourth; and `FRankConditions` (AT.hpp:9691) is
-- 0x6 bytes — three uint8 pairs — so there is physically no room for another tier. The capture
-- agrees: Num=0.0 -> S everywhere, Num=1/3 -> A on the single row the screenshot shows an "A"
-- (Gohan's max combo). Z is a Dragon Ball convention elsewhere; this screen does not have it.
local RANK_LETTERS = { [0] = "S", [1] = "A", [2] = "B" }
local RANK_COUNT = 3   -- = RANK_MAX / RANK_ANIM_TYPE_NUM in the enums above, not a guess

local function rank_letter(img)
    if not Core.valid(img) or not Core.is_visible(img) then return nil end
    local mat = Core.member_path(img, "Brush", "ResourceObject")
    if not Core.nonnull(mat) then return nil end
    local cls
    pcall(function() cls = mat:GetClass():GetFName():ToString() end)
    if cls ~= LIVE_MAT_CLASS then return nil end
    local p = mat_params(mat)
    local v = p[NUM_PARAM]
    if type(v) ~= "number" then return nil end
    -- Atlas geometry off the material when it publishes it; otherwise the rank COUNT, which is
    -- the same number and is itself read from the game's enums rather than assumed. Falling
    -- back matters: losing the geometry would otherwise turn "always S" into "no rank at all",
    -- which is a worse regression than the bug being fixed.
    local cells = (p.columns and p.rows) and (p.columns * p.rows) or RANK_COUNT
    if cells < 1 then return nil end
    return RANK_LETTERS[math.floor(v * cells + 0.5)]
end

-- All currently-readable lines with stable keys, in on-screen order.
local function lines()
    local out = { { key = "screen", text = I18n.header(6) } }
    local bars, nbars = Core.array_of(host, "UIClearBar_List")
    if not bars then return out end
    pcall(function()
        for i = 1, nbars do
            local bar = bars[i]
            if Core.valid(bar) and Core.on_screen(bar) then
                -- NAME candidates (2026-07-31 census, dump_1785513949_001.txt:446/458): the
                -- battle title reads from the Blueprint node `Txt_List`
                -- (Quest_Main_Clear_C_4.Clear_Bar_Dummy00.Txt_List -> "Gohan contra Oficial
                -- del Ejército de Freezer"), NOT from the native spelling this asked for.
                -- Core.first_text is the shared strict multi-candidate helper — its own
                -- comment names this exact TextBox_*/Txt_List alternation — so an absent
                -- candidate is a quiet nil instead of the uncatchable abort.
                local name = Core.first_text(bar, "TextBox_Item", "Txt_List")
                -- Skip the lines already spoken (2026-07-29). `Results.update` drops them
                -- anyway, so every rank/digit texture resolved for them is thrown away —
                -- and that work is the expensive part: rank_letter and image_number each
                -- hop to a brush ResourceObject and GetFullName it, PER IMAGE, on every
                -- 100 ms poll, for as long as the results screen stays up. This is a pure
                -- short-circuit on the EXISTING `spoken` set, not a new cache: no extra
                -- lifetime to invalidate, and it clears with `spoken` when the screen closes.
                -- `name` is still read unconditionally — the first detail row is prefixed
                -- with it below, even once the bar's own line has been spoken.
                local barkey = "bar" .. i
                if not spoken[barkey] then
                    local r = rank_letter(Core.member(bar, "Image_Rank"))
                    if name and r then
                        out[#out + 1] = { key = barkey, text = name .. ", " .. r }
                    end
                end
                -- Expanded details (after "View details"): blueprint children; the
                -- first row is prefixed with its battle so groups stay apart.
                for j = 0, DETAIL_COUNT - 1 do
                    local key = string.format("d%d.%d", i, j)
                    if not spoken[key] then
                        local d
                        pcall(function()
                            d = Core.member(bar, "Quest_Main_Clear_Detail" .. string.format("%02d", j))
                        end)
                        if Core.valid(d) and Core.on_screen(d) then
                            -- Same alternation as the bar name above: the census shows the
                            -- criterion text on `Txt_List` (…Quest_Main_Clear_Detail00
                            -- .Txt_List -> "Tiempo de finalización", Detail01 -> "Combo
                            -- máximo", Detail02 -> "Daño recibido").
                            local dn = Core.first_text(d, "TextBox_Detail", "Txt_List")
                            local dr = rank_letter(Core.member(d, "Image_Rank"))
                            if dn and dr then
                                if DEBUG then debug_dump_detail(d, key, dn, bar) end
                                local text = Core.phrase(dn,
                                    image_number(d, "Image_PercentageList"), dr)
                                if j == 0 and name then text = name .. ": " .. text end
                                out[#out + 1] = { key = key, text = text }
                            end
                        end
                    end
                end
            end
        end
        -- Hardened 2026-07-29: both were raw UObject member fetches outside any gate.
        if not spoken["total"] then
            local tot = Core.member(host, "UIQuestMainClearRank")
            if Core.valid(tot) and Core.on_screen(tot) then
                local r = rank_letter(Core.member(tot, "Image_Rank"))
                if r then
                    out[#out + 1] = { key = "total", text = I18n.t("results_total") .. ", " .. r }
                end
            end
        end
    end)
    return out
end

-- Last logged gate-rejection reason, for the dedupe in is_active below.
local gate_last = nil

-- SPEAK IN SCREEN ORDER rather than reveal order (user request, 2026-07-31: *"prefiero oírlo en
-- el orden, no pasa nada si se tarda un segundo más"*).
--
-- `lines()` already builds the sheet in the game's own order — each battle, then its criterion
-- rows, then the total. The problem was never the order it BUILDS, it is that rows become
-- READABLE at different moments during the reveal animation, and saying each one the instant it
-- was ready produced "battle 1, battle 2, total, …then every detail row".
--
-- So hold the whole sheet until it STOPS GROWING and release it in one go. Deliberately NOT done
-- by withholding lines individually until the previous one is ready: `DETAIL_COUNT` is 6 while
-- this screen only uses 3, so a per-line rule has to tell "this row does not exist" apart from
-- "this row is not ready yet", and getting that wrong stalls the reader FOREVER — much worse
-- than the cosmetic defect being fixed.
--
-- Measured in WALL TIME, never in polls: the adapter is driven by a loop whose rate varies with
-- what the game thread is doing, so a settle counted in calls confirms a call count and nothing
-- else (the playbook's rule, reached the hard way on the objective line).
--
-- FAILS OPEN TWICE, because a results sheet that never speaks is far worse than one that speaks
-- a second late: a count that stops changing releases after SETTLE_S, and a sheet that somehow
-- never settles releases anyway at HOLD_MAX_S — degrading to the old reveal-order behaviour
-- rather than to silence.
local SETTLE_S = 0.6      -- quiet period after the last new line before releasing
local HOLD_MAX_S = 5.0    -- absolute ceiling on the hold, from the sheet first being claimed
local hold_n, hold_t0, hold_seen = nil, nil, nil

local function clear_hold() hold_n, hold_t0, hold_seen = nil, nil, nil end

function Results.is_active()
    tick = tick + 1
    host = Core.first_on_screen("Quest_Main_Clear_C", tick)   -- pooled, multi-instance
    -- A pooled results pane lingers on_screen (still rendered) after it closes, so gate on
    -- it being GENUINELY LIVE (pane_live: visibility + opacity) and cross-check free-roam
    -- (the minimap being up = back in the world, so any remaining pane is stale). Without
    -- this, a flicker off/on re-cleared `spoken` and re-announced old results in the middle
    -- of nowhere (user 2026-07-24; same pane-live rule cooking uses, CLAUDE.md §8).
    --
    -- WHICH of the three refused (2026-07-31). The user has reported this screen unread more
    -- than once, and `screen -> screen_results` has NEVER appeared in a log — which by the
    -- playbook's own shortcut puts the fault in these gates, not in lines(). But three
    -- conditions can refuse here and none of them said which, so every round was a guess
    -- between them. The census settles WHAT to read (Quest_Main_Clear_C_4 was on screen,
    -- fully populated, while the adapter stayed silent); this settles WHY it is refused.
    -- One line per CHANGE of reason: silent while nothing changes, and one battle names it.
    -- LIVENESS: pane_RENDERED, not pane_live (2026-07-31). This is the THIRD time the same
    -- defect has been found — screen_fishresult 2026-07-17, screen_questreward 2026-07-28,
    -- and this file, which the two earlier sweeps never reached (its gate dates from
    -- 2026-07-24 and has not been revisited since). pane_live demands ESlateVisibility
    -- Visible(0); a result sheet in this game renders HitTestInvisible/SelfHitTestInvisible,
    -- so that gate rejects the host on EVERY tick and the adapter is silent FOREVER. The
    -- fingerprint is identical to questreward's, quoted in its own header: `screen ->
    -- screen_results` appears in no log, not once, while the user's F7 census taken WITH the
    -- sheet up (dump_1785513949_001.txt:435-458) shows both battle bars and all six criterion
    -- rows populated. The visibility half exists to stop a pooled pane SHADOWING the adapters
    -- below it, which matters here (this is registered 4th of 44) — but the stale-pane job is
    -- already done by the two gates that remain: `on_screen` drops Collapsed/Hidden, the
    -- opacity check drops the close-animation ghost, and `free_roam` (the 2026-07-24 fix for
    -- re-announcing old results in the middle of nowhere) is untouched.
    local rej = (not host and "host-not-on-screen")
        or (not Core.pane_rendered(host) and "pane-not-rendered")
        or (Core.free_roam(tick) and "free-roam")
        or nil
    if rej ~= gate_last then
        gate_last = rej
        if rej then print("[KakarotAccess] results gate: " .. rej .. "\n") end
    end
    if rej then
        spoken = {}
        queue = {}
        dumped = {}
        clear_hold()
        return false
    end
    -- Build every tick (cheap once a line is in `spoken`), but only RELEASE it to update() once
    -- the sheet has settled — see the note above SETTLE_S.
    local built = lines()
    local now = os.clock()
    if hold_t0 == nil then hold_t0 = now end
    if #built ~= hold_n then hold_n, hold_seen = #built, now end
    if (now - (hold_seen or now)) >= SETTLE_S or (now - hold_t0) >= HOLD_MAX_S then
        queue = built
    else
        queue = {}
    end
    return true
end

function Results.reset()
    ann:reset()
    clear_hold()   -- reset() runs on every screen change: the free backstop for the hold state
end

function Results.update()
    for _, e in ipairs(queue) do
        if not spoken[e.key] then
            spoken[e.key] = true
            Speech.say(e.text, false)   -- queued: follows the game's reveal pacing
        end
    end
end

return Results
