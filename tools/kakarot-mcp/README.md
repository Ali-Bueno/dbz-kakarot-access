# kakarot-mcp — inspect the running game from the assistant

An MCP server that lets Claude Code query **the live game** instead of asking you to press F7 and
paste a file. Zero dependencies (Node ≥ 18); it speaks MCP's newline-delimited JSON-RPC directly.

It does **not** play the game. You still have to walk to the shop and open the menu — but from that
moment the assistant can ask eight questions about it without a single round-trip through you.

## What it talks to

Two independent file channels, both already the way these mods are driven. Nothing new touches the
`lua_State`, which is the whole reason it is files and not a socket — see the header of
`mod/KakarotAccess/Scripts/dev_channel.lua`.

| Channel | Mod | Files (next to `UE4SS.log`) | Answers |
|---|---|---|---|
| `mod` | KakarotAccess (`dev_channel.lua`) | `kakarot_cmd.txt` / `kakarot_out.txt` | the mod's own state: active adapter, speech history, census, reload, nav dumps |
| `inspector` | `tools/ue4ss-inspector` | `inspector_cmd.txt` / `inspector_out.txt` | generic engine reflection: `findall`, `dumpclass`, `props`, `probe`, `watch`, `dump` |

Each command is written as `#<seq> <command>` and answered between `<<<BEGIN seq>>>` /
`<<<END seq>>>`, so the server can find *its* answer in an append-only file and a repeated command
still runs twice.

## Tools

| Tool | Use it for |
|---|---|
| `kak_alive` | First thing when anything times out: game running? which channel answers? |
| `kak_screen` | *Which adapter owns the tick, and what did the mod last say.* The fastest answer to "why is this screen silent?" |
| `kak_class` | `dumpclass` parsed and filtered — `grep` matches member **names**, `only` picks props or funcs |
| `kak_cmd` | Any Inspector command verbatim |
| `kak_dev` | Any dev-channel command verbatim (`say`, `navdump`, `navlevels`, `memdiff`, …) |
| `kak_census` | Fire the F7 census **while the screen is open** and get the dump back, filtered |
| `kak_reload` | Ctrl+Shift+R without the keyboard, after editing any script except `main.lua` |
| `kak_log` | Tail `UE4SS.log` with a regex (`faulted in is_active`, `screen ->`) |

## Requirements

1. **The game running**, with KakarotAccess loaded and `build_flags.lua` at `debug = true`.
   The dev channel is compiled out of releases (`package.ps1` drops `dev_channel.lua` and rewrites
   the flag), so this only ever works in the dev tree.
2. **The Inspector mod installed** — needed by `kak_cmd` and `kak_class` only. It is not junctioned
   like KakarotAccess, so copy it in once:

   ```powershell
   $game = "D:\games\steam\steamapps\common\DRAGON BALL Z KAKAROT\AT\Binaries\Win64"
   Copy-Item "tools\ue4ss-inspector" (Join-Path $game "Mods\Inspector") -Recurse
   # then add   Inspector : 1   to $game\Mods\mods.txt, ABOVE the "; Built-in keybinds" line
   ```

   No ini change is needed: the file channel works with `ConsoleEnabled = 0`.

`kak_alive` tells you which of those two is missing.

## Configuration

`.mcp.json` at the repo root registers the server and sets the game directory. Override per machine
with the environment:

| Variable | Default |
|---|---|
| `KAKAROT_GAME_DIR` | the Steam path in `.mcp.json` |
| `KAKAROT_DUMPS_DIR` | `mod/KakarotAccess/Scripts/dumps` |
| `KAKAROT_LOG` | `<game dir>/UE4SS.log` |

## Latency and limits

- Both mods poll every **500 ms**, so a command costs ~0.3–1 s. `kak_reload` can take several
  seconds (it re-requires ~60 modules) and `kak_census` waits for the dump file to land.
- One command in flight per channel: overlapping calls are queued client-side, because the mods read
  only the first line of the command file and would drop the loser.
- `kak_cmd dump <kind>` reports completion **asynchronously** — the tool returns when the dumper is
  queued; confirm with `kak_log`.
- `probe` needs two calls: the first takes a baseline, the second diffs it after you move the cursor.

## Testing it without the game

```bash
printf '%s\n' \
  '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-06-18"}}' \
  '{"jsonrpc":"2.0","id":2,"method":"tools/list"}' | node tools/kakarot-mcp/server.js
```

Tool calls will report "no answer from the … channel", which is the correct answer when nothing is
running.
