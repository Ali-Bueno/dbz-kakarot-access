-- Catalog QA for the opening through the immediate post-Raditz scene.

local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local Cues = require("audio_description_cues")

local fails = 0
local function check(cond, what)
    if cond then
        print("ok   - " .. what)
    else
        fails = fails + 1
        print("FAIL - " .. what)
    end
end

-- Every cinematic source encountered before and immediately after the Raditz
-- battle is explicit here. An empty table is meaningful: that movie was reviewed
-- but has no safe dialogue-free description window.
local REQUIRED = {
    "C01_000_S010_MOV", "C01_000_S020_MOV", "C01_000_S040_MOV",
    "C01_010_S010_GDM", "C01_010_S020_GDM", "C01_010_S025_MOV",
    "C01_010_S030_MOV", "C01_010_S040_MOV", "C01_010_S070_GDM",
    "C01_010_S080_MOV", "C01_010_S120_GDM", "C01_020_S010_GDM",
    "C01_020_S030_GDM",
    "C01_020_S040_MOV", "C01_020_S060_MOV",
}

for _, id in ipairs(REQUIRED) do
    check(type(Cues[id]) == "table", "catalog covers " .. id)
end

local count = 0
for id, list in pairs(Cues) do
    check(id:match("^C%d%d_%d%d%d_S%d%d%d_[MG][OD][VM]$") ~= nil,
        "catalog key is a normalized demo id: " .. tostring(id))
    local previous = -1
    for i, cue in ipairs(list) do
        count = count + 1
        check(type(cue.at) == "number" and cue.at >= 0 and cue.at > previous,
            id .. " cue " .. i .. " has a strictly increasing timestamp")
        check(type(cue.text) == "string" and cue.text ~= "",
            id .. " cue " .. i .. " has description text")
        check(not cue.text:match("^Audio description:"),
            id .. " cue " .. i .. " does not waste time on a spoken prefix")
        if cue.before then
            local estimated_end = cue.at + 0.5 + #cue.text / 14
            check(estimated_end <= cue.before + 0.001,
                id .. " cue " .. i .. " fits its verified dialogue-free window")
        end
        previous = cue.at
    end
end

-- Compact Raditz-arrival pass: the returned ViddyScribe intervals were too tight
-- for the mod's speech estimate, and adjacent descriptions must not queue over
-- each other. These independent windows are the intersection of both installed
-- mono voice tracks in C01_010_S080_mov.usm (2026-09-05 FFmpeg analysis).
-- Include the scheduler's 0.35-second late-start allowance, not just ideal timing.
local ARRIVAL_VOICE_GAPS = {
    {0.000, 4.739}, {5.326, 14.658}, {19.659, 47.800},
    {56.614, 59.668}, {69.788, 76.637}, {77.196, 79.433},
    {82.811, 85.138}, {93.105, 107.847},
}
local arrival = Cues.C01_010_S080_MOV
for i, cue in ipairs(arrival) do
    local finish = cue.at + 0.5 + #cue.text / 14 + 0.35
    local fits = false
    for _, gap in ipairs(ARRIVAL_VOICE_GAPS) do
        if cue.at >= gap[1] and finish <= gap[2] + 0.001 then fits = true end
    end
    check(fits, "Raditz arrival cue " .. i .. " leaves late-start headroom before either voice track")
    local next_cue = arrival[i + 1]
    check(not next_cue or finish <= next_cue.at,
        "Raditz arrival cue " .. i .. " finishes before the following description")
end
check(count >= 45, "catalog contains at least 45 timed visual descriptions")

-- Every language file carries every cue under its derived key (the adapter speaks the
-- English catalog text only as a fallback). Parsed with the same line pattern as
-- i18n.lua's load_ext, so a line the mod could not read fails here too.
local LANGS = { "ar", "de", "es", "fr", "it", "ja", "ko", "pl", "pt", "ru", "th", "zh" }
for _, code in ipairs(LANGS) do
    local path = here .. "../../mod/KakarotAccess/Scripts/lang/" .. code .. ".txt"
    local f = io.open(path, "r")
    check(f ~= nil, "lang/" .. code .. ".txt exists")
    local entries = {}
    if f then
        for line in f:lines() do
            local key, val = line:match("^%s*([^#=%s][^=]-)%s*=%s*(.-)%s*$")
            if key then entries[key] = val end
        end
        f:close()
    end
    local missing = 0
    for _, list in pairs(Cues) do
        for _, cue in ipairs(list) do
            local v = entries[cue.key]
            if type(v) ~= "string" or v == "" then missing = missing + 1 end
        end
    end
    check(missing == 0, "lang/" .. code .. ".txt carries all " .. count .. " cue lines (missing " .. missing .. ")")
end

if fails > 0 then
    print(("%d check(s) FAILED"):format(fails))
    os.exit(1)
end
print("all checks passed")
