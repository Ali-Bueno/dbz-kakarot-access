-- Installs the mod's RegisterHook(s) for the overworld-menu section reader.
--
-- Kept in its OWN file on purpose — main.lua require()s it under pcall, so the safest rollback
-- if hooking ever destabilizes the game is simply DELETING this file and restarting.
--
-- DIAGNOSTIC MODE: SetFontType turned out never to fire via a hookable path (fired=0 — it's
-- called directly in C++, not via ProcessEvent). So we now probe several candidate navigation
-- UFunctions at once, counting how often each fires and the value of its first param, exposed
-- via _G.__KakarotHook for the F6 keybind. Once we learn which one fires on menu navigation and
-- what its param means, this collapses back to a single hook that announces the section.

local Hook = {}

-- DISABLED (2026-07-02): hooking is a confirmed DEAD END for the overworld menu on this game.
-- RegisterHook installs fine, but the menu's navigation is driven directly in C++ (not via
-- ProcessEvent), so none of the candidate UFunctions fire on cursor movement — tested live:
-- SetFontType fired 0; OnSelectItem 0; RootMenu/StartTop CheckHovered 0; In_Curs fired once
-- (menu open) only. And the labels are image-font with no readable text, AND even a targeted
-- read of the header's Image_HeaderFont aborts uncatchably. So the overworld Start menu is not
-- reachable from Lua at all (reflection can't see the non-UPROPERTY selection member, hooks
-- don't fire, and widget access crashes). It would need a native C++ path (Ghidra-found offsets
-- + a memory-read in the bridge DLL). install() is now a no-op so the mod stays hook-free and
-- stable; the enum→name map (i18n.header) + header_reader are kept, ready for a future native
-- reader. The candidate list below is retained only as a record of what was tried.
local TRIED = {
    "/Script/AT.AT_UIXCmnHeader:SetFontType",  -- 0 (C++-direct, not ProcessEvent)
    "/Script/AT.RootMenu:OnSelectItem",        -- 0
    "/Script/AT.RootMenu:CheckHovered",        -- 0
    "/Script/AT.AT_UIStartTop:CheckHovered",   -- 0
    "/Script/AT.AT_UIStartTop:In_Curs",        -- 1 (only on menu open)
}

-- No-op: install no hooks. See the note above. Returns false so main.lua prints nothing.
function Hook.install()
    _G.__KakarotHook = { installed = false, fired = {}, last = {} }
    return false
end

-- Kept so the F6 diagnostic key degrades gracefully.
function Hook.status() return "hooks disabled (dead end on this game)" end

-- Former installer, retained for reference / a future re-test. Not called.
function Hook._install_experimental()
    _G.__KakarotHook = { installed = false, fired = {}, last = {} }
    local H = _G.__KakarotHook
    if type(RegisterHook) ~= "function" then return false end
    for _, name in ipairs(CANDIDATES) do
        H.fired[name] = 0
        pcall(function()
            RegisterHook(name, function(ctx, p1)
                H.fired[name] = (H.fired[name] or 0) + 1
                local okg, v = pcall(function() return p1 and p1:get() end)
                if okg and v ~= nil then
                    H.last[name] = v
                    -- If this is the selection callback, route it to the (reloadable) reader so
                    -- we can also hear whether the value maps to a sensible section name.
                    pcall(function() require("app").header_font(v) end)
                end
            end)
        end)
    end
    H.installed = true
    return true
end

-- A spoken summary for the F6 diagnostic: which candidates fired and their last param value.
function Hook.status()
    local H = _G.__KakarotHook
    if not H then return "hook not installed" end
    local parts = {}
    for _, name in ipairs(CANDIDATES) do
        local short = name:match("([^%.]+:[%w_]+)$") or name
        local n = H.fired[name] or 0
        if n > 0 then
            local lv = H.last[name]
            parts[#parts + 1] = short .. " " .. n .. (lv ~= nil and (" last " .. tostring(lv)) or "")
        end
    end
    if #parts == 0 then return "installed " .. tostring(H.installed) .. ", none fired" end
    return table.concat(parts, ", ")
end

return Hook
