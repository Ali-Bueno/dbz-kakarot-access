#!/usr/bin/env node
'use strict';
//
// server.js — MCP server for DRAGON BALL Z KAKAROT, so the assistant can inspect the running game
// directly instead of asking a human to press F7 and paste a file.
//
// Zero dependencies, on purpose: MCP over stdio is newline-delimited JSON-RPC 2.0, which is ~60
// lines to speak. Pulling the SDK in would put a node_modules tree in a repo whose rule is not to
// commit binaries, for no capability we need.
//
// Requires (see README.md): the game running with KakarotAccess loaded and build_flags.debug = true,
// and — for the reflection tools only — the Inspector mod installed. kak_alive reports which of the
// two is answering.

const fs = require('fs');
const path = require('path');
const C = require('./channel');

const SERVER = { name: 'kakarot', version: '1.0.0' };
const DEFAULT_PROTOCOL = '2025-06-18';

// --- tools --------------------------------------------------------------------------------------

const S = (props, required = []) => ({ type: 'object', properties: props, required });
const GREP = { type: 'string', description: 'Case-insensitive regex; only matching lines are returned. Use it — a class dump is hundreds of lines.' };
const MAXL = { type: 'number', description: 'Cap on returned lines (default 200).' };

const TOOLS = [
    {
        name: 'kak_alive',
        description: 'Health check: is the game running, is KakarotAccess answering, is the Inspector mod installed and answering, how stale is UE4SS.log. Run this first when anything else times out.',
        inputSchema: S({}),
    },
    {
        name: 'kak_screen',
        description: "Which screen adapter currently owns the tick, the registry state, and the last lines the mod actually spoke. This is the fastest answer to 'why is this screen silent?' — an adapter that never appears here has a gate problem in is_active, and one that appears while speaking nothing is holding the tick with nothing to say.",
        inputSchema: S({}),
    },
    {
        name: 'kak_class',
        description: 'Dump a UClass chain (properties with their types, and functions) from the live game, parsed and filterable. Accepts a short class name or a /Full/Object/Path. Needs the Inspector mod.',
        inputSchema: S({
            name: { type: 'string', description: 'Class or object path, e.g. "Xcmn_Pause_C" or "/Script/AT.AT_UIStartQuest".' },
            only: { type: 'string', enum: ['all', 'props', 'funcs'], description: 'Restrict the output (default all).' },
            grep: { type: 'string', description: 'Case-insensitive regex matched against property/function NAMES.' },
            max_lines: MAXL,
        }, ['name']),
    },
    {
        name: 'kak_cmd',
        description: 'Run any Inspector command against the live game: findall <Class>, find <substr>, dumpclass <Class>, props <Class> [n], funcs <Class>, outer <Class>, probe <Class>, watch <Class> <Prop> [ms], dump <objects|cxx|uht|usmap|actors|meshes|luatypes>, help. Note `probe` needs two calls (baseline, then diff after moving the cursor) and `dump` reports completion asynchronously — read it back with kak_log.',
        inputSchema: S({
            cmd: { type: 'string', description: 'The full command line, e.g. "findall Xcmn_Pause_C".' },
            grep: GREP,
            max_lines: MAXL,
            timeout_ms: { type: 'number', description: 'Default 10000. Raise for a full dump.' },
        }, ['cmd']),
    },
    {
        name: 'kak_dev',
        description: "Run a KakarotAccess dev-channel command: ping, screen, say <text>, reload, census, navdump, navlevels, memdiff, help. These reach the mod's own internals, unlike kak_cmd which is generic engine reflection.",
        inputSchema: S({
            cmd: { type: 'string', description: 'The full command line, e.g. "say testing" or "navdump".' },
            grep: GREP,
            max_lines: MAXL,
            timeout_ms: { type: 'number', description: 'Default 10000.' },
        }, ['cmd']),
    },
    {
        name: 'kak_census',
        description: "Trigger the F7 discovery census (every visible widget and its text, right now) and return the dump it wrote. Take this WHILE the screen in question is open — the playbook's rule is that a census settles in one pass what rounds of header-grepping get wrong, because it names the widget that is actually live.",
        inputSchema: S({
            grep: { type: 'string', description: 'Case-insensitive regex; only matching lines of the dump are returned. Without it you get the path and a short head, not the whole file.' },
            max_lines: MAXL,
        }),
    },
    {
        name: 'kak_reload',
        description: "Hot-reload the mod's Lua (the Ctrl+Shift+R path) without touching the keyboard. Use after editing any file under mod/KakarotAccess/Scripts except main.lua, which is only loaded at game start.",
        inputSchema: S({}),
    },
    {
        name: 'kak_log',
        description: "Tail UE4SS.log with a filter. Start here when an adapter goes quiet: grep 'faulted in is_active' for the pcall-isolated fault that names the culprit, or 'screen ->' for what has been claiming the tick.",
        inputSchema: S({
            grep: GREP,
            lines: { type: 'number', description: 'How many matching lines from the END of the log (default 80).' },
            bytes: { type: 'number', description: 'How much of the log tail to scan (default 512000).' },
        }),
    },
];

// --- implementations ----------------------------------------------------------------------------

const text = (t) => ({ content: [{ type: 'text', text: t }] });
const fail = (t) => ({ content: [{ type: 'text', text: t }], isError: true });

async function kakAlive() {
    const out = [];
    out.push(`game dir : ${C.GAME_DIR}${fs.existsSync(C.GAME_DIR) ? '' : '   *** NOT FOUND ***'}`);
    const logSize = C.fileSize(C.LOG_PATH);
    if (logSize) {
        const age = Math.round((Date.now() - fs.statSync(C.LOG_PATH).mtimeMs) / 1000);
        out.push(`UE4SS.log: ${(logSize / 1024).toFixed(0)} KB, last written ${age}s ago`);
    } else {
        out.push('UE4SS.log: absent — the game has not run here');
    }
    // Both channels at once: they are independent queues, and a dead one costs its whole timeout.
    const [mod, insp] = await Promise.all([
        C.send('mod', 'ping', { timeoutMs: 3500 }),
        C.send('inspector', 'help', { timeoutMs: 3500 }),
    ]);
    out.push('');
    out.push(`KakarotAccess dev channel : ${mod.ok ? 'ALIVE — ' + mod.lines.join(' | ') : 'no answer'}`);
    if (!mod.ok) {
        out.push('    -> game not running, or build_flags.debug is false, or the mod failed to load');
    }
    out.push(`Inspector mod             : ${insp.ok ? 'ALIVE' : 'no answer'}`);
    if (!insp.ok) {
        out.push(`    -> install it: copy tools/ue4ss-inspector to ${path.join(C.GAME_DIR, 'Mods', 'Inspector')}`);
        out.push('       and add "Inspector : 1" to Mods/mods.txt (kak_cmd and kak_class need it)');
    }
    return text(out.join('\n'));
}

async function kakClass(a) {
    const r = await C.send('inspector', `dumpclass ${a.name}`, { timeoutMs: a.timeout_ms || 20000 });
    if (!r.ok) return fail(r.reason);
    const classes = C.parseClassDump(r.lines);
    if (!classes.length) return text(r.lines.join('\n') || `nothing returned for ${a.name}`);

    const only = a.only || 'all';
    let re = null;
    if (a.grep) {
        try { re = new RegExp(a.grep, 'i'); } catch (e) { return fail(`bad grep regex: ${e.message}`); }
    }
    const lines = [];
    let hits = 0;
    for (const c of classes) {
        const props = only === 'funcs' ? [] : c.props.filter((p) => !re || re.test(p.name));
        const funcs = only === 'props' ? [] : c.funcs.filter((f) => !re || re.test(f));
        if (!props.length && !funcs.length) continue;
        hits += props.length + funcs.length;
        lines.push(`-- ${c.name}`);
        for (const p of props) lines.push(`   ${p.name.padEnd(44)} ${p.type}`);
        for (const f of funcs) lines.push(`   fn ${f}()`);
    }
    const total = classes.reduce((n, c) => n + c.props.length + c.funcs.length, 0);
    if (!lines.length) {
        return text(`${a.name}: ${classes.length} classes in the chain, ${total} members, `
                  + `none matching /${a.grep}/i.\nChain: ${classes.map((c) => c.name).join(' <- ')}`);
    }
    const head = `${a.name}: ${hits}/${total} members across ${classes.length} classes in the chain\n`;
    return text(head + C.shape(lines, { max_lines: a.max_lines }));
}

async function kakCensus(a) {
    const t0 = Date.now();
    const r = await C.send('mod', 'census', { timeoutMs: 10000 });
    if (!r.ok) return fail(r.reason);
    // discover.run() queues its own ExecuteInGameThread and reports no path, so the file lands a tick
    // or two after the command returns and is found by being the newest one written after we asked.
    let dump = null;
    for (let i = 0; i < 30 && !dump; i++) {
        await C.sleep(300);
        dump = C.newestDump(t0 - 1500, 'dump_');
    }
    if (!dump) {
        return fail(`census ran (${r.lines.join(' | ')}) but no new file appeared in ${C.DUMPS_DIR}`);
    }
    // Let it finish being written before reading it — discover.lua writes the whole file in one go,
    // but "newest mtime" can catch it mid-flush on a big screen.
    await C.sleep(400);
    const body = fs.readFileSync(dump.path, 'utf8').split(/\r?\n/);
    const header = `${dump.path}\n${body.length} lines, ${(dump.size / 1024).toFixed(1)} KB\n\n`;
    if (!a.grep) {
        return text(header + C.shape(body, { max_lines: a.max_lines || 60 })
                  + '\n\n(pass grep= to search the whole dump, or read the file directly)');
    }
    return text(header + C.shape(body, { grep: a.grep, max_lines: a.max_lines }));
}

async function kakLog(a) {
    const tail = C.tailFile(C.LOG_PATH, a.bytes || 512 * 1024);
    if (tail === null) return fail(`no log at ${C.LOG_PATH}`);
    let lines = tail.split(/\r?\n/).filter((l) => l.trim().length);
    if (a.grep) {
        let re;
        try { re = new RegExp(a.grep, 'i'); } catch (e) { return fail(`bad grep regex: ${e.message}`); }
        lines = lines.filter((l) => re.test(l));
    }
    const want = a.lines || 80;
    const shown = lines.slice(-want);            // the END of the log is the interesting end
    const note = lines.length > shown.length ? `(last ${shown.length} of ${lines.length} matching)\n\n` : '';
    return text(note + (shown.join('\n') || '(nothing matched)'));
}

async function callTool(name, a = {}) {
    switch (name) {
        case 'kak_alive':  return kakAlive();
        case 'kak_class':  return kakClass(a);
        case 'kak_census': return kakCensus(a);
        case 'kak_log':    return kakLog(a);
        case 'kak_screen': {
            const r = await C.send('mod', 'screen', { timeoutMs: 8000 });
            return r.ok ? text(C.shape(r.lines, {})) : fail(r.reason);
        }
        case 'kak_reload': {
            // Re-requires ~60 modules on the game thread; nothing else in the mod takes this long.
            const r = await C.send('mod', 'reload', { timeoutMs: 25000 });
            return r.ok ? text(r.lines.join('\n')) : fail(r.reason);
        }
        case 'kak_cmd': {
            const r = await C.send('inspector', a.cmd, { timeoutMs: a.timeout_ms || 10000 });
            return r.ok ? text(C.shape(r.lines, a)) : fail(r.reason);
        }
        case 'kak_dev': {
            const r = await C.send('mod', a.cmd, { timeoutMs: a.timeout_ms || 10000 });
            return r.ok ? text(C.shape(r.lines, a)) : fail(r.reason);
        }
        default: return fail(`unknown tool: ${name}`);
    }
}

// --- MCP stdio transport ------------------------------------------------------------------------

function write(msg) { process.stdout.write(JSON.stringify(msg) + '\n'); }
const reply = (id, result) => write({ jsonrpc: '2.0', id, result });
const replyErr = (id, code, message) => write({ jsonrpc: '2.0', id, error: { code, message } });

async function handle(msg) {
    if (msg.id === undefined || msg.id === null) return;      // a notification wants no answer
    try {
        switch (msg.method) {
            case 'initialize':
                return reply(msg.id, {
                    protocolVersion: (msg.params && msg.params.protocolVersion) || DEFAULT_PROTOCOL,
                    capabilities: { tools: {} },
                    serverInfo: SERVER,
                });
            case 'ping':        return reply(msg.id, {});
            case 'tools/list':  return reply(msg.id, { tools: TOOLS });
            case 'tools/call':  return reply(msg.id, await callTool(msg.params.name, msg.params.arguments || {}));
            default:            return replyErr(msg.id, -32601, `method not found: ${msg.method}`);
        }
    } catch (e) {
        // A thrown tool is a tool result, not a protocol error — the caller should see the message.
        if (msg.method === 'tools/call') return reply(msg.id, fail(String((e && e.message) || e)));
        return replyErr(msg.id, -32603, String((e && e.stack) || e));
    }
}

// Exit only once every in-flight call has answered. A live client holds stdin open so 'end' never
// fires, but a piped test (see README) closes it the instant the last line is written — and exiting
// there would kill a tool call that is still waiting on the game, which is exactly the call worth
// testing. Every request outlives its own stdin.
let inFlight = 0;
let stdinEnded = false;
const maybeExit = () => { if (stdinEnded && inFlight === 0) process.exit(0); };

let buf = '';
process.stdin.setEncoding('utf8');
process.stdin.on('data', (chunk) => {
    buf += chunk;
    let i;
    while ((i = buf.indexOf('\n')) >= 0) {
        const line = buf.slice(0, i).trim();
        buf = buf.slice(i + 1);
        if (!line) continue;
        let msg;
        try { msg = JSON.parse(line); } catch { continue; }
        // Deliberately not awaited: requests may overlap, and the per-channel queues in channel.js
        // are what actually serializes access to the game.
        inFlight++;
        handle(msg).catch(() => {}).finally(() => { inFlight--; maybeExit(); });
    }
});
process.stdin.on('end', () => { stdinEnded = true; maybeExit(); });

// stdout is the protocol — every diagnostic goes to stderr or it corrupts the stream.
process.stderr.write(`[kakarot-mcp] game dir: ${C.GAME_DIR}\n`);
