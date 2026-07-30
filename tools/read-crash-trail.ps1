# Decode the mod's crash black box (crash_trail.bin) WITHOUT launching the game.
#
# WHY THIS EXISTS. The trail is a memory-mapped ring that records the last 64 operations the mod
# performed, so that a process death which leaves no Lua traceback and no UE4 crash dump still
# names the subsystem that was running (see the 2026-07-26 (b) entry in
# reference/dbz-kakarot/notes/dbz-kakarot-crash-bug.md, where it named the faulting adapter in one
# read). Until 2026-07-29 the ONLY way to read it was to relaunch the game and let
# `main.lua`'s boot print dump it into UE4SS.log — which means a reporting player had to be talked
# through a restart, and any second crash overwrote the evidence. The format is fixed and
# self-describing, so it can be decoded offline from the file alone. Ask a reporter for
# `Mods\KakarotAccess\Scripts\crash_trail.bin` (16 KB) and run this.
#
# The layout is NOT guessed — it is src/mem_bridge/mem_bridge.c:245-255:
#     MarkHeader { uint32 magic, version, slots, text_size; uint64 seq; uint64 reserved; }  = 32 B
#     MarkSlot   { uint64 seq; uint64 tick_ms; char text[MARK_TEXT]; }                      = 128 B
# with MARK_MAGIC 'KAK1' = 0x314B414B, MARK_SLOTS 64, MARK_TEXT 112. Slots start at offset 32 and
# are a ring indexed by (seq - 1) % slots; a slot with seq == 0 was never written. `mark_write`
# stores the text BEFORE the seq deliberately, so a torn write during a crash leaves a slot that
# looks unwritten rather than one with mismatched text — hence seq == 0 is skipped, not reported.
#
# READING THE OUTPUT. The mark is written BEFORE the operation it names, so the LAST line is the
# operation that was in flight when the process died. The Delta column is each mark's offset from
# the last one: a large gap immediately after a mark means that operation blocked the game thread
# for that long, which is how the explore sweep's cost was first measured (438 ms, 2026-07-29 (d)).
#
# Usage:  .\tools\read-crash-trail.ps1 [-Path <crash_trail.bin>]
# Default path is the dev install recorded in STATUS.md.

param(
    [string]$Path = 'D:\games\steam\steamapps\common\DRAGON BALL Z KAKAROT\AT\Binaries\Win64\Mods\KakarotAccess\Scripts\crash_trail.bin'
)

$ErrorActionPreference = 'Stop'

if (-not (Test-Path -LiteralPath $Path)) {
    Write-Error "crash trail not found: $Path"
    exit 1
}

$bytes = [System.IO.File]::ReadAllBytes($Path)

# Header must be complete before any field is trusted.
if ($bytes.Length -lt 32) {
    Write-Error "file is $($bytes.Length) bytes - too short to hold the 32-byte header"
    exit 1
}

$magic    = [BitConverter]::ToUInt32($bytes, 0)
$version  = [BitConverter]::ToUInt32($bytes, 4)
$slots    = [BitConverter]::ToUInt32($bytes, 8)
$textSize = [BitConverter]::ToUInt32($bytes, 12)
$seq      = [BitConverter]::ToUInt64($bytes, 16)

if ($magic -ne 0x314B414B) {
    Write-Error ("bad magic 0x{0:X8} (expected 0x314B414B 'KAK1') - not a crash trail, or written by a different build" -f $magic)
    exit 1
}

# Refuse to walk past the end of the buffer: the first mem_bridge build sized the mapping at a
# round 8192 while header+slots came to 8224, and the RECOVERY read faulted. Same class of bug,
# so the reader derives its bound instead of assuming one.
$slotSize = 16 + $textSize
$needed   = 32 + ($slots * $slotSize)
if ($bytes.Length -lt $needed) {
    Write-Error "header declares $slots slots x $slotSize B = $needed B, file is only $($bytes.Length) B"
    exit 1
}

Write-Host ("crash trail: version {0}, {1} slots x {2} B text, {3} marks written this session" -f $version, $slots, $textSize, $seq)

if ($seq -eq 0) {
    Write-Host 'Ring is EMPTY - the previous session wrote no marks (clean shutdown before any work, or a first run).'
    exit 0
}

$rows = for ($i = 0; $i -lt $slots; $i++) {
    $off     = 32 + ($i * $slotSize)
    $slotSeq = [BitConverter]::ToUInt64($bytes, $off)
    if ($slotSeq -eq 0) { continue }   # never written (or torn - see header comment)
    $tick    = [BitConverter]::ToUInt64($bytes, $off + 8)
    $raw     = [System.Text.Encoding]::UTF8.GetString($bytes, $off + 16, $textSize)
    $nul     = $raw.IndexOf([char]0)
    [pscustomobject]@{
        Seq  = $slotSeq
        Tick = $tick
        Text = if ($nul -ge 0) { $raw.Substring(0, $nul) } else { $raw }
    }
}

$rows = @($rows | Sort-Object Seq)
if ($rows.Count -eq 0) {
    Write-Host 'No written slots found despite a non-zero seq - the ring was reset mid-write.'
    exit 0
}

# Deltas are relative to the LAST mark, so the newest line is always +0.000s and the gaps that
# precede it read as negative offsets - the shape you scan for when hunting a game-thread stall.
$base = $rows[-1].Tick
Write-Host ''
Write-Host ('{0,10}  {1,10}  {2}' -f 'Seq', 'Delta', 'Operation')
Write-Host ('{0,10}  {1,10}  {2}' -f '---', '-----', '---------')
foreach ($r in $rows) {
    $delta = ([int64]$r.Tick - [int64]$base) / 1000.0
    Write-Host ('{0,10}  {1,10}  {2}' -f $r.Seq, ('{0:+0.000;-0.000;0.000}s' -f $delta), $r.Text)
}

Write-Host ''
Write-Host ("LAST OPERATION IN FLIGHT: {0}" -f $rows[-1].Text)
Write-Host 'The mark is written BEFORE the call, so the process died inside that operation.'
Write-Host 'A large negative gap between two adjacent marks = the earlier operation blocked the game thread that long.'
