-- Regression: the spoken-duration estimate must count CHARACTERS, not bytes.
-- Byte counting inflated every 2-3 byte alphabetic script (Cyrillic, Thai, Arabic) and held
-- the queue/protection windows that much too long. Ideographs must NOT move: they were already
-- charged 3 units by the byte model and a syllable really does take about that long to say.
local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "../../mod/KakarotAccess/Scripts/?.lua;" .. package.path

local Speech = require("speech")
local est = Speech.speak_seconds

-- What the previous implementation returned, kept here as the comparison baseline.
local function old_byte_model(s) return math.min(6.0, 0.5 + #s / 14) end

local function rep(cp, n)
    local c = utf8.char(cp)
    local out = {}
    for _ = 1, n do out[#out + 1] = c end
    return table.concat(out)
end

local failures = 0
local function check(label, cond)
    if cond then print("ok   - " .. label)
    else failures = failures + 1 print("FAIL - " .. label) end
end
local function near(a, b) return math.abs(a - b) < 1e-9 end

-- Representative characters, by Unicode block.
local HAN, KANA, HANGUL = 0x4E00, 0x3042, 0xAC00     -- ideographic: weight 3
local CYRILLIC, THAI, ARABIC = 0x0412, 0x0E01, 0x0645 -- alphabetic, but 2-3 bytes each
local LATIN = 0x0061

-- 1. Ideographic scripts must be BYTE-IDENTICAL to the old model (no behaviour change).
for _, cp in ipairs({ HAN, KANA, HANGUL }) do
    local s = rep(cp, 8)
    check(string.format("U+%04X x8 keeps its old estimate (%.3fs)", cp, est(s)),
        near(est(s), old_byte_model(s)))
end

-- 2. The 2-3 byte alphabetic scripts must now match the SAME NUMBER of Latin letters,
--    and must be strictly shorter than the byte model gave them.
for _, cp in ipairs({ CYRILLIC, THAI, ARABIC }) do
    local s, latin = rep(cp, 8), rep(LATIN, 8)
    check(string.format("U+%04X x8 now costs the same as 8 letters", cp), near(est(s), est(latin)))
    check(string.format("U+%04X x8 is shorter than the old byte model", cp), est(s) < old_byte_model(s))
end

-- 3. ASCII is untouched.
check("ascii is unchanged", near(est("Bulma esta aqui"), old_byte_model("Bulma esta aqui")))

-- 4. The tail and the cap still hold.
check("empty text is just the tail", near(est(""), 0.5))
check("a very long line is capped", near(est(rep(LATIN, 5000)), 6.0))
check("a wall of hanzi is capped too", near(est(rep(HAN, 5000)), 6.0))

-- 5. Malformed UTF-8 must fall back, never raise: this runs on the speech path.
local ok, v = pcall(est, string.char(0xE5))
check("malformed utf-8 does not raise", ok and type(v) == "number")

if failures > 0 then print(failures .. " check(s) failed") os.exit(1) end
print("all checks passed")
