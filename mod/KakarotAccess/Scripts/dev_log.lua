-- Dev-only: append probe results to a file the developer reads directly, so nothing
-- has to be relayed by voice. Absolute path into the repo (Scripts is a junction into
-- the game), same convention as discover.lua. Lines are timestamped and appended in
-- order, so the file reads as a chronological transcript of the F4 / field probes.

local DevLog = {}

local PATH = "D:\\code\\unreal\\dragon ball kakarot access\\mod\\KakarotAccess\\Scripts\\dev_probe.txt"

function DevLog.write(line)
    local stamp = 0
    pcall(function() stamp = os.time() end)
    local f = io.open(PATH, "a")
    if f then
        f:write(string.format("[%d] %s\n", stamp, tostring(line)))
        f:close()
    end
end

function DevLog.clear()
    local f = io.open(PATH, "w")
    if f then f:close() end
end

return DevLog
