-- Build flags for KakarotAccess.
--
-- In the DEV tree (this file) debug = true, so main.lua registers the developer
-- keybinds (F4/F6/F7/F9/F10/Ctrl+F5/Ctrl+Shift+F5 — memory diff, screen dumps,
-- diagnostics). When packaging a release, package.ps1 REWRITES this file with
-- debug = false so none of those ship to players.
return {
    debug = true,
}
