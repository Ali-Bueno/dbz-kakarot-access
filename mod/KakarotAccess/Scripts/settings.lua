-- User-editable mod configuration, persisted to Scripts/config.txt.
--
-- A single settings store other modules read (audio-cue master switch + volume,
-- radar auto-activation, language override). It is loaded ONCE from main.lua, before
-- the protected snapshot, so it survives a Ctrl+Shift+R reload with its in-memory
-- values intact (the config menu writes changes straight to disk as they happen).
--
-- The file is a plain `key = value` text file (comments with '#') so a user can edit
-- it by hand; the config menu (config_menu.lua) writes the same format. Unknown keys
-- are preserved on load only in spirit — the writer rewrites from the known set, so
-- editing values is safe but adding arbitrary keys is not persisted. Every value falls
-- back to DEFAULTS when the file is missing or a line is malformed.

local Settings = {}

-- Declaration order also drives the rewrite order in config.txt (kept stable so a
-- hand-edited file stays readable). `kind` coerces the on-disk string.
local SCHEMA = {
    { key = "audio_cues",      kind = "bool", def = true,  comment = "Audio navigation cues on/off (on|off)" },
    { key = "cue_volume",      kind = "pct",  def = 100,   comment = "Audio cue volume, 0-100" },
    { key = "radar_autotrack", kind = "bool", def = true,  comment = "Radar auto-tracks a new objective by itself (on|off)" },
    { key = "language",        kind = "lang", def = "auto", comment = "Mod language: auto (follow the game) or a code: en es fr de it pt ru pl ja ko zh ar th" },
}

local DEFAULTS = {}
for _, e in ipairs(SCHEMA) do DEFAULTS[e.key] = e.def end

local values = {}
local loaded = false

-- ---- paths -----------------------------------------------------------------

local function script_dir()
    local src = debug.getinfo(1, "S").source:sub(2)
    return src:match("^(.*)[/\\]") or "."
end

local function config_path()
    return script_dir() .. "\\config.txt"
end

-- ---- coercion --------------------------------------------------------------

local function to_bool(raw, def)
    if raw == nil then return def end
    raw = tostring(raw):lower():gsub("%s+", "")
    if raw == "on" or raw == "true" or raw == "1" or raw == "yes" then return true end
    if raw == "off" or raw == "false" or raw == "0" or raw == "no" then return false end
    return def
end

local function to_pct(raw, def)
    local n = tonumber(raw)
    if not n then return def end
    n = math.floor(n + 0.5)
    if n < 0 then n = 0 elseif n > 100 then n = 100 end
    return n
end

local VALID_LANG = {
    auto = true, en = true, es = true, fr = true, de = true, it = true, pt = true,
    ru = true, pl = true, ja = true, ko = true, zh = true, ar = true, th = true,
}

local function to_lang(raw, def)
    if not raw then return def end
    raw = tostring(raw):lower():gsub("%s+", "")
    return VALID_LANG[raw] and raw or def
end

local function coerce(entry, raw)
    if entry.kind == "bool" then return to_bool(raw, entry.def) end
    if entry.kind == "pct" then return to_pct(raw, entry.def) end
    if entry.kind == "lang" then return to_lang(raw, entry.def) end
    return raw ~= nil and raw or entry.def
end

-- ---- serialization to disk -------------------------------------------------

local function serialize(key, val)
    if val == true then return "on" end
    if val == false then return "off" end
    return tostring(val)
end

local function save()
    local ok = pcall(function()
        local f = io.open(config_path(), "w")
        if not f then return end
        f:write("# KakarotAccess configuration.\n")
        f:write("# Edit values after '='; keep the key names. The in-game config menu\n")
        f:write("# (L3 + R3 in the overworld) writes this file too.\n\n")
        for _, e in ipairs(SCHEMA) do
            f:write("# " .. e.comment .. "\n")
            f:write(e.key .. " = " .. serialize(e.key, values[e.key]) .. "\n\n")
        end
        f:close()
    end)
    return ok
end

-- ---- load ------------------------------------------------------------------

local function read_raw()
    local raw = {}
    local ok = pcall(function()
        local f = io.open(config_path(), "r")
        if not f then return end
        for line in f:lines() do
            -- strip a UTF-8 BOM on the first line, then parse `key = value` (skip # / blank).
            line = line:gsub("^\239\187\191", "")
            local k, v = line:match("^%s*([%w_]+)%s*=%s*(.-)%s*$")
            if k then raw[k] = v end
        end
        f:close()
    end)
    return ok and raw or {}
end

local function load()
    local raw = read_raw()
    local had_file = next(raw) ~= nil
    for _, e in ipairs(SCHEMA) do
        values[e.key] = coerce(e, raw[e.key])
    end
    -- Write a fresh, fully-commented file the first time (so the user has a template
    -- to edit) or when it was missing/empty.
    if not had_file then save() end
end

-- ---- public API ------------------------------------------------------------

function Settings.init()
    if loaded then return Settings end
    load()
    loaded = true
    _G.__KakarotSettings = Settings   -- read cross-module without adding locals (nav_tracker cap)
    return Settings
end

function Settings.get(key)
    return values[key]
end

-- Set + persist. Returns the stored (coerced) value.
function Settings.set(key, val)
    for _, e in ipairs(SCHEMA) do
        if e.key == key then
            values[key] = coerce(e, val)
            save()
            return values[key]
        end
    end
    return nil
end

-- Convenience accessors used by the audio/radar layers.
function Settings.cues_enabled()   return values.audio_cues ~= false end
function Settings.cue_gain()       return (values.cue_volume or 100) / 100 end
function Settings.autotrack_enabled() return values.radar_autotrack ~= false end
function Settings.language()       return values.language or "auto" end

return Settings
