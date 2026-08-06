'use strict';
//
// channel.js — everything that touches the running game (or its files).
//
// The transport is the pair of text files the game already polls: we write one tagged line into
// <game>/…_cmd.txt and read the bracketed answer back out of <game>/…_out.txt. That is deliberately
// the dumbest possible channel — see the header of mod/KakarotAccess/Scripts/dev_channel.lua for why
// a socket would be a bad trade on this engine.
//
// Two mods answer, on two independent channels:
//   inspector — tools/ue4ss-inspector, generic engine reflection (findall/dumpclass/probe/…)
//   mod       — KakarotAccess's own dev_channel.lua, the mod's internals (screen/census/reload/…)

const fs = require('fs');
const path = require('path');

const REPO_DIR = path.resolve(__dirname, '..', '..');

const GAME_DIR = process.env.KAKAROT_GAME_DIR
    || 'D:\\games\\steam\\steamapps\\common\\DRAGON BALL Z KAKAROT\\AT\\Binaries\\Win64';
const DUMPS_DIR = process.env.KAKAROT_DUMPS_DIR
    || path.join(REPO_DIR, 'mod', 'KakarotAccess', 'Scripts', 'dumps');
const LOG_PATH = process.env.KAKAROT_LOG || path.join(GAME_DIR, 'UE4SS.log');

const CHANNELS = {
    inspector: { cmd: 'inspector_cmd.txt', out: 'inspector_out.txt', label: 'Inspector mod' },
    mod:       { cmd: 'kakarot_cmd.txt',   out: 'kakarot_out.txt',   label: 'KakarotAccess dev channel' },
};

const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

function fileSize(p) {
    try { return fs.statSync(p).size; } catch { return 0; }
}

// Read a byte range without slurping the whole file — UE4SS.log and inspector_out.txt both grow
// without bound across a session, and the answer we want is always at the end.
function readFrom(p, start) {
    const size = fileSize(p);
    if (size <= start) return '';
    const fd = fs.openSync(p, 'r');
    try {
        const buf = Buffer.alloc(size - start);
        fs.readSync(fd, buf, 0, buf.length, start);
        return buf.toString('utf8');
    } finally {
        fs.closeSync(fd);
    }
}

function tailFile(p, maxBytes = 512 * 1024) {
    const size = fileSize(p);
    if (!size) return null;
    return readFrom(p, Math.max(0, size - maxBytes));
}

// One command in flight per channel. Both mods drop a command that arrives while they are busy (the
// dedup compares whole lines and the poll only reads the FIRST line), so two overlapping tool calls
// would silently lose one. Concurrency here buys nothing anyway: the game answers on its own thread.
const queues = { inspector: Promise.resolve(), mod: Promise.resolve() };
let seqCounter = 0;

function send(channelName, command, opts = {}) {
    const run = () => sendNow(channelName, command, opts);
    const q = queues[channelName].then(run, run);
    // Keep the chain alive whatever happens, or one rejection wedges the channel for the session.
    queues[channelName] = q.catch(() => {});
    return q;
}

async function sendNow(channelName, command, { timeoutMs = 10000, pollMs = 120 } = {}) {
    const ch = CHANNELS[channelName];
    if (!ch) throw new Error(`unknown channel: ${channelName}`);
    if (!fs.existsSync(GAME_DIR)) {
        return { ok: false, reason: `game dir not found: ${GAME_DIR}`, lines: [] };
    }
    const cmdPath = path.join(GAME_DIR, ch.cmd);
    const outPath = path.join(GAME_DIR, ch.out);

    // Unique per call, so the answer is findable in an append-only file AND so the mods' "same line
    // as last time = ignore" dedup can never swallow a legitimate repeat (`probe X` twice is the
    // whole point of probe).
    const seq = `q${Date.now().toString(36)}${(seqCounter++).toString(36)}`;
    let startAt = fileSize(outPath);

    try {
        fs.writeFileSync(cmdPath, `#${seq} ${command}\n`, 'utf8');
    } catch (e) {
        return { ok: false, reason: `cannot write ${cmdPath}: ${e.message}`, lines: [] };
    }

    const begin = `<<<BEGIN ${seq}>>>`;
    const end = `<<<END ${seq}>>>`;
    const deadline = Date.now() + timeoutMs;
    while (Date.now() < deadline) {
        await sleep(pollMs);
        // A truncated/rotated out file would otherwise park us past EOF forever.
        if (fileSize(outPath) < startAt) startAt = 0;
        const tail = readFrom(outPath, startAt);
        const i = tail.indexOf(begin);
        const j = tail.indexOf(end);
        if (i >= 0 && j > i) {
            const body = tail.slice(i + begin.length, j);
            return { ok: true, seq, lines: body.split(/\r?\n/).filter((l) => l.trim().length) };
        }
    }
    return {
        ok: false,
        reason: `no answer from the ${ch.label} in ${timeoutMs} ms — is the game running with that `
              + `mod loaded? (kak_alive tells you which channel is dead)`,
        lines: [],
    };
}

// --- presentation -------------------------------------------------------------------------------
// The point of routing through a server instead of reading the file directly: a dumpclass on a big
// UMG class is several hundred lines, and the caller almost always wants five of them.
function shape(lines, { grep, max_lines = 200 } = {}) {
    let out = lines;
    let note = '';
    if (grep) {
        let re;
        try { re = new RegExp(grep, 'i'); } catch (e) { throw new Error(`bad grep regex: ${e.message}`); }
        const before = out.length;
        out = out.filter((l) => re.test(l));
        note = `\n\n(${out.length}/${before} lines matching /${grep}/i)`;
    }
    if (out.length > max_lines) {
        note += `\n\n(truncated: showing ${max_lines} of ${out.length} lines — raise max_lines or narrow grep)`;
        out = out.slice(0, max_lines);
    }
    if (!out.length) return (grep ? `no lines matching /${grep}/i` : '(no output)') + note;
    return out.join('\n') + note;
}

// Parse the Inspector's `dumpclass` output into { class -> {props, funcs} }. Format is fixed by
// tools/ue4ss-inspector/Scripts/main.lua:146-158 — `-- <class>`, then `   <name> <type>` for a
// property and `   fn <name>()` for a function.
function parseClassDump(lines) {
    const classes = [];
    let cur = null;
    for (const raw of lines) {
        const line = raw.replace(/\s+$/, '');
        const header = line.match(/^-- (.+)$/);
        if (header) { cur = { name: header[1], props: [], funcs: [] }; classes.push(cur); continue; }
        if (!cur) continue;
        const fn = line.match(/^\s+fn (\S+)\(\)$/);          // before the property shape: `fn X()`
        if (fn) { cur.funcs.push(fn[1]); continue; }          // matches it too
        const prop = line.match(/^\s+(\S+)\s+(\S+)$/);
        if (prop) cur.props.push({ name: prop[1], type: prop[2] });
    }
    return classes;
}

// Newest file in the dumps folder, optionally only ones written after `sinceMs`. discover.lua names
// its output dump_<epoch>_<n>.txt and reports no path, so "the newest one" is how it gets found.
function newestDump(sinceMs = 0, prefix = '') {
    let best = null;
    let entries;
    try { entries = fs.readdirSync(DUMPS_DIR); } catch { return null; }
    for (const name of entries) {
        if (prefix && !name.startsWith(prefix)) continue;
        const full = path.join(DUMPS_DIR, name);
        let st;
        try { st = fs.statSync(full); } catch { continue; }
        if (!st.isFile() || st.mtimeMs < sinceMs) continue;
        if (!best || st.mtimeMs > best.mtimeMs) best = { path: full, name, mtimeMs: st.mtimeMs, size: st.size };
    }
    return best;
}

module.exports = {
    GAME_DIR, DUMPS_DIR, LOG_PATH, CHANNELS,
    send, shape, parseClassDump, newestDump, tailFile, fileSize, sleep,
};
