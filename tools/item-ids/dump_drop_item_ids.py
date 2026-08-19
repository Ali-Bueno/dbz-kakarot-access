#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Regenerate mod/KakarotAccess/Scripts/item_drop_ids.lua from the shipped .pak files.

WHAT THIS PRODUCES
------------------
A flat Lua map:  field-drop-table id (a slug such as "Lost_Seaweed_Fixed")  ->  numeric item id.
A pickup actor in the world exposes that slug through its ItemTableComponent (FFieldItemData's
FixedId / NormalId, AT.hpp:6024). The game then shows the item's name via its own localized
message key "Item_<id>_Name", so the mod needs the NUMBER and nothing else -- no English text
lives in the generated file and it never needs translating.

WHY THE PARSING LOOKS LIKE THIS
-------------------------------
1) A plain text grep of the paks finds NOTHING for "Item_36029", and that is not a compression
   artefact. An FName in Unreal is a pair (index into the package name table, number). The name
   table holds the bare entry "Item"; the per-row number carries the digits. UE stores the number
   as N+1 so that 0 can mean "no numeric suffix at all", i.e. the FName rendered "Item_36029" is
   stored as (index-of "Item", 36030). KNOWN_GOOD below pins that off-by-one empirically: the
   script asserts the raw stored number, so if a future engine version changed the convention the
   run fails loudly instead of emitting a table that is silently off by one everywhere.
   The same convention applies to PROPERTY names: the tag for "DropItemId_1" is stored as
   ("DropItemId", 2) -- which is why the header's field names are absent from the name table.

2) We read the export as a TAGGED-PROPERTY stream rather than at fixed offsets. The layout in
   AT.hpp (FFieldDropItemTableRow @ 5995) is the in-MEMORY layout; on disk each property carries
   its own name/type/size and properties left at their default value are simply ABSENT. Walking
   the tags is therefore both robust across game patches and layout-agnostic -- the same code
   reads the sibling tables that use FFieldItemTableRow (no CondEpisode/CountMin/CountMax) with
   no special casing.

3) The DataTable's own row KEY is a meaningless ordinal ("0", "1", "2", ...) in these tables; the
   slug the game actually looks up lives in the row's ItemTableId property. We key the output by
   ItemTableId. (Several rows share one ItemTableId, differing only in CondEpisode -- see
   pick_row.)

RE-RUNNING AFTER A GAME PATCH
-----------------------------
    set PYTHONIOENCODING=utf-8
    python3 tools/item-ids/dump_drop_item_ids.py
Everything it needs is in the constants block below. It re-discovers the source tables from the
pak index each run, so a DLC that adds a new Lottery table is picked up automatically.
"""

import os
import re
import shutil
import struct
import subprocess
import tempfile
from collections import defaultdict

# ---------------------------------------------------------------------------
# CONSTANTS -- edit these if the game or the tools move.
# ---------------------------------------------------------------------------
GAME_DIR = r"D:/games/steam/steamapps/common/DRAGON BALL Z KAKAROT"
REPAK_EXE = r"D:/code/tools/repak/repak.exe"
PAK_INDEX = r"D:/code/tools/repak/pak_index.txt"   # newline-separated list of every asset path
PAKS = [                                           # searched in order for each asset
    GAME_DIR + "/AT/Content/Paks/pakchunk0-WindowsNoEditor.pak",
    GAME_DIR + "/AT/Content/Paks/pakchunk1-WindowsNoEditor.pak",
]
REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
OUT_LUA = os.path.join(REPO_ROOT, "mod", "KakarotAccess", "Scripts", "item_drop_ids.lua")

# The localized message table we cross-check emitted ids against. Any locale works -- the KEYS are
# locale-independent -- es_MX is simply the one this mod ships against.
MESSAGE_ASSET = "AT/Content/Message/PLAT_W/es_MX/messageData.uasset"

# Which tables to merge. Discovered from PAK_INDEX so new DLC tables need no code change.
# Blueprints/ is excluded: those are the pickup ACTORS (LostPropertyItem*), not drop tables.
TABLE_PATTERN = re.compile(
    r"^AT/Content/(?:DLC/\d+/)?Parameter/Field/[^/]*(?:Lottery|LostProperty)[^/]*\.uasset$")

# Rows whose drop resolves to one of these are dropped from the output: they are not item ids.
NONE_NAMES = ("None", "", None)

# The FName base name that marks a numeric item id ("Item" + number -> Item_<number>).
ITEM_NAME_BASE = "Item"

# Ground truth. These two MUST come out exactly right or the parse is wrong; the second number is
# the value actually stored in the file, and exists to pin the N+1 rule rather than assume it.
KNOWN_GOOD = {
    "Lost_Seaweed_Fixed":        (36029, 36030),   # message Item_36029_Name = "Algas marinas"
    "Lost_Sub_Npc086_01_Item01": (81071, 81072),   # message Item_81071_Name = "Fruta namekuseijin"
}

PACKAGE_FILE_TAG = 0x9E2A83C1


# ---------------------------------------------------------------------------
# Little-endian reader
# ---------------------------------------------------------------------------
class Reader(object):
    def __init__(self, buf, off=0):
        self.b = buf
        self.o = off

    def i32(self):
        v = struct.unpack_from("<i", self.b, self.o)[0]
        self.o += 4
        return v

    def u32(self):
        v = struct.unpack_from("<I", self.b, self.o)[0]
        self.o += 4
        return v

    def i64(self):
        v = struct.unpack_from("<q", self.b, self.o)[0]
        self.o += 8
        return v

    def u8(self):
        v = self.b[self.o]
        self.o += 1
        return v

    def fstring(self):
        """FString: int32 length then the bytes. A NEGATIVE length means UTF-16LE and counts
        UTF-16 code units. Both forms include the terminating NUL in the count."""
        n = self.i32()
        if n == 0:
            return ""
        if n < 0:
            raw = self.b[self.o:self.o - 2 * n - 2]
            self.o += -2 * n
            return raw.decode("utf-16-le", "replace")
        raw = self.b[self.o:self.o + n - 1]
        self.o += n
        return raw.decode("utf-8", "replace")


# ---------------------------------------------------------------------------
# .uasset package header
# ---------------------------------------------------------------------------
class Package(object):
    """Just enough of FPackageFileSummary to reach the name table and the export map.

    The summary is read SEQUENTIALLY (never at hardcoded offsets) because two of its fields are
    variable length: the custom-version array and the FolderName string.
    """

    def __init__(self, uasset_bytes):
        r = Reader(uasset_bytes)
        tag = r.u32()
        if tag != PACKAGE_FILE_TAG:
            raise ValueError("not a UE package (tag %08X)" % tag)
        legacy = r.i32()                       # -7 in this game
        if legacy != -4:
            r.i32()                            # LegacyUE3Version
        self.file_version_ue4 = r.i32()        # 0: shipped unversioned, engine version is implied
        r.i32()                                # FileVersionLicenseeUE4
        custom_version_count = r.i32()
        r.o += custom_version_count * 20       # each is FGuid(16) + int32
        self.total_header_size = r.i32()       # == len(uasset); the .uexp starts right after it
        r.fstring()                            # FolderName
        r.u32()                                # PackageFlags
        name_count = r.i32()
        name_offset = r.i32()
        r.i32()                                # GatherableTextDataCount
        r.i32()                                # GatherableTextDataOffset
        export_count = r.i32()
        export_offset = r.i32()

        # Name table: [FString][uint16 non-case-preserving hash][uint16 case-preserving hash]
        r.o = name_offset
        self.names = []
        for _ in range(name_count):
            self.names.append(r.fstring())
            r.o += 4

        # Export map. We only need where each export's data lives inside the .uexp.
        r.o = export_offset
        self.exports = []
        for _ in range(export_count):
            r.i32()                                     # ClassIndex
            r.i32()                                     # SuperIndex
            r.i32()                                     # TemplateIndex
            r.i32()                                     # OuterIndex
            name_idx = r.i32()
            name_num = r.i32()
            r.u32()                                     # ObjectFlags
            serial_size = r.i64()
            serial_offset = r.i64()
            r.o += 12                                   # bForcedExport/bNotForClient/bNotForServer
            r.o += 16                                   # PackageGuid
            r.u32()                                     # PackageFlags
            r.o += 8                                    # bNotAlwaysLoadedForEditorGame/bIsAsset
            r.o += 20                                   # 5 x preload-dependency int32
            self.exports.append({
                "name": self.render_name(name_idx, name_num),
                # SerialOffset is a whole-package offset; the .uexp is the tail of the package,
                # so subtracting the header size converts it to a .uexp offset.
                "uexp_offset": serial_offset - self.total_header_size,
                "size": serial_size,
            })

    def render_name(self, index, number):
        """(name-table index, number) -> display string. THE off-by-one: number 0 means no
        suffix, otherwise the printed suffix is number-1."""
        base = self.names[index]
        return base if number == 0 else "%s_%d" % (base, number - 1)


# ---------------------------------------------------------------------------
# Tagged property stream
# ---------------------------------------------------------------------------
def read_fname_raw(pkg, r):
    """Returns (rendered, base, raw_number). We keep the raw number so KNOWN_GOOD can assert it."""
    idx = r.i32()
    num = r.i32()
    return pkg.render_name(idx, num), pkg.names[idx], num


def read_tagged_properties(pkg, r):
    """Read one FPropertyTag stream up to its 'None' terminator.

    Tag layout (UE4, versioned/tagged serialization):
        FName Name  -- 'None' ends the block
        FName Type
        int32 Size            <- lets us skip any value we do not care about
        int32 ArrayIndex
        <type-specific tag data>
        uint8 HasPropertyGuid (+ FGuid if set)
        <Size bytes of value>
    Because Size is always present we never have to understand a type to skip past it, which is
    what makes this survive struct changes.
    """
    out = {}
    while True:
        name, _, _ = read_fname_raw(pkg, r)
        if name == "None":
            break
        ptype, _, _ = read_fname_raw(pkg, r)
        size = r.i32()
        r.i32()                                    # ArrayIndex
        if ptype == "StructProperty":
            read_fname_raw(pkg, r)                 # struct name
            r.o += 16                              # struct guid
        elif ptype == "BoolProperty":
            r.u8()                                 # value lives in the tag; Size is 0
        elif ptype in ("ByteProperty", "EnumProperty", "ArrayProperty",
                       "SetProperty", "MapProperty"):
            read_fname_raw(pkg, r)                 # enum / inner type name
            if ptype == "MapProperty":
                read_fname_raw(pkg, r)             # value type name
        if r.u8():
            r.o += 16                              # optional property guid
        start = r.o
        if ptype == "NameProperty":
            out[name] = read_fname_raw(pkg, r)     # (rendered, base, raw_number)
        elif ptype == "IntProperty":
            out[name] = r.i32()
        r.o = start + size                         # always trust Size, never the value reader
    return out


def parse_data_table(uasset_bytes, uexp_bytes):
    """Return (package, [(row_key, properties), ...]) for the single UDataTable export.

    UDataTable serialization is: UObject's own tagged properties (here just RowStruct), then the
    row payload written by UDataTable::LoadStructData -- int32 row count, then per row an FName
    key followed by the row struct as its own tagged-property block.
    """
    pkg = Package(uasset_bytes)
    exports = [e for e in pkg.exports if e["uexp_offset"] >= 0]
    if not exports:
        return pkg, []
    r = Reader(uexp_bytes, exports[0]["uexp_offset"])
    read_tagged_properties(pkg, r)                 # the DataTable object's own properties

    # UE writes 4 more bytes between the object's tagged-property block and the row payload
    # (the export's trailing "extras" in asset tooling); it is 0 in every table shipped here.
    # Asserted rather than skipped blindly, so a format change fails loudly instead of silently
    # shifting the row count by one field.
    extras = r.i32()
    if extras != 0:
        raise ValueError("unexpected non-zero export extras (%d) -- serialization changed" % extras)

    row_count = r.i32()
    rows = []
    for _ in range(row_count):
        key, _, _ = read_fname_raw(pkg, r)
        rows.append((key, read_tagged_properties(pkg, r)))

    # The reader must land exactly on the export's end (the .uexp tail is the 4-byte package tag).
    end = exports[0]["uexp_offset"] + exports[0]["size"]
    if r.o != end:
        raise ValueError("row stream ended at %d, expected %d" % (r.o, end))
    return pkg, rows


# ---------------------------------------------------------------------------
# Row -> item id
# ---------------------------------------------------------------------------
def pick_drop(props):
    """Choose one DropItemId slot from a row.

    Rule: the slot with the HIGHEST DropItemWeight -- that is the common drop. Ties (and rows with
    no weights at all) fall back to the lowest slot number, i.e. slot 1. Empty slots are stored as
    the FName 'None' and ignored.
    Returns (rendered, base, raw_number) or None.
    """
    best = None
    for slot in range(1, 6):
        val = props.get("DropItemId_%d" % slot)
        if not val or val[0] in NONE_NAMES:
            continue
        weight = props.get("DropItemWeight_%d" % slot, 0) or 0
        # -slot makes the comparison tie-break towards the LOWEST slot number.
        cand = (weight, -slot, val)
        if best is None or cand[:2] > best[:2]:
            best = cand
    return best[2] if best else None


def pick_row(rows):
    """Several rows can share one ItemTableId, differing only in CondEpisode: the same bush drops
    a bigger vegetable later in the story (Lost_Carrot_01 -> Item_36011 in episodes 1-2, Item_36012
    from episode 3). A flat static map cannot express that, so we deterministically emit the LOWEST
    CondEpisode variant (ties: first in file order). The count of affected slugs is reported.
    """
    return min(rows, key=lambda kv: (kv[1].get("CondEpisode", 0), kv[0]))


# ---------------------------------------------------------------------------
# Pak extraction
# ---------------------------------------------------------------------------
def extract(asset_path, dest_dir):
    """repak get <pak> <asset> -> stdout. Tries each pak; returns local file path or None."""
    out_path = os.path.join(dest_dir, asset_path.replace("/", "_"))
    for pak in PAKS:
        with open(out_path, "wb") as fh:
            proc = subprocess.run([REPAK_EXE, "get", pak, asset_path],
                                  stdout=fh, stderr=subprocess.DEVNULL)
        if proc.returncode == 0 and os.path.getsize(out_path) > 0:
            return out_path
    if os.path.exists(out_path):
        os.remove(out_path)
    return None


def discover_tables():
    """Every Lottery/LostProperty data table under Parameter/Field, base game and DLC."""
    with open(PAK_INDEX, "r", encoding="utf-8", errors="replace") as fh:
        return sorted({line.strip() for line in fh if TABLE_PATTERN.match(line.strip())})


def load_message_keys():
    """The set of N for which the message key Item_<N>_Name exists.

    Those keys live in messageData's NAME TABLE (they end in "_Name", so UE cannot fold them into
    the numeric-suffix form and stores each as a literal string). We parse the name table properly
    rather than regexing the raw bytes so a hit cannot come from unrelated binary data.
    """
    tmp = tempfile.mkdtemp(prefix="kakmsg_")
    try:
        path = extract(MESSAGE_ASSET, tmp)
        if not path:
            return None
        with open(path, "rb") as fh:
            pkg = Package(fh.read())
        pat = re.compile(r"^Item_(\d+)_Name$")
        return set(int(m.group(1)) for m in (pat.match(n) for n in pkg.names) if m)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def main():
    assets = discover_tables()
    print("discovered %d candidate tables under Parameter/Field" % len(assets))

    tmp = tempfile.mkdtemp(prefix="kakitem_")
    merged = {}            # slug -> item id
    origin = {}            # slug -> asset that first defined it
    used_assets = []
    skipped_none = 0
    skipped_non_item = 0
    episode_variants = 0
    conflicts = []
    raw_numbers = {}       # slug -> raw stored FName number, for the KNOWN_GOOD assertions

    try:
        for asset in assets:
            uasset = extract(asset, tmp)
            uexp = extract(asset[:-len(".uasset")] + ".uexp", tmp)
            if not uasset or not uexp:
                print("  SKIP  %s (not found in any pak)" % asset)
                continue
            with open(uasset, "rb") as fh:
                ua = fh.read()
            with open(uexp, "rb") as fh:
                ue = fh.read()
            try:
                _, rows = parse_data_table(ua, ue)
            except Exception as exc:                       # report and keep going
                print("  FAIL  %s: %s" % (asset, exc))
                continue

            # Group this table's rows by the slug the game looks up.
            by_slug = defaultdict(list)
            for key, props in rows:
                tid = props.get("ItemTableId")
                if not tid or tid[0] in NONE_NAMES:
                    continue                                # not a field-drop row shape
                by_slug[tid[0]].append((key, props))
            if not by_slug:
                print("  SKIP  %s (no ItemTableId rows -- different row struct)" % asset)
                continue

            emitted_here = 0
            for slug in sorted(by_slug):
                group = by_slug[slug]
                if len(set((pick_drop(p) or (None,))[0] for _, p in group)) > 1:
                    episode_variants += 1
                _, props = pick_row(group)
                drop = pick_drop(props)
                if drop is None:
                    skipped_none += 1
                    continue
                _, base, raw_num = drop
                if base != ITEM_NAME_BASE or raw_num == 0:
                    # a table that drops something other than a numeric item id
                    skipped_non_item += 1
                    continue
                item_id = raw_num - 1                       # THE off-by-one, see module docstring
                if slug in merged:
                    if merged[slug] != item_id:
                        conflicts.append((slug, origin[slug], merged[slug], asset, item_id))
                    continue                                # first table to define a slug wins
                merged[slug] = item_id
                raw_numbers[slug] = raw_num
                origin[slug] = asset
                emitted_here += 1
            used_assets.append(asset)
            print("  ok    %-72s %4d rows -> %4d ids" % (asset, len(rows), emitted_here))
    finally:
        shutil.rmtree(tmp, ignore_errors=True)

    # --- ground truth --------------------------------------------------------
    for slug in sorted(KNOWN_GOOD):
        want_id, want_raw = KNOWN_GOOD[slug]
        got_id = merged.get(slug)
        got_raw = raw_numbers.get(slug)
        if got_id != want_id or got_raw != want_raw:
            raise SystemExit("KNOWN-GOOD FAILED: %s -> id %r (raw %r), expected %r (raw %r). "
                             "If every id is off by one the FName number convention changed."
                             % (slug, got_id, got_raw, want_id, want_raw))
    print("known-good pairs verified (%s)"
          % ", ".join("%s=%d" % (k, merged[k]) for k in sorted(KNOWN_GOOD)))

    # --- cross-check against the localized message table ---------------------
    msg_ids = load_message_keys()
    if msg_ids is None:
        print("WARNING: could not read %s -- orphan check skipped" % MESSAGE_ASSET)
        orphans = []
    else:
        orphans = sorted(set(v for v in merged.values() if v not in msg_ids))
        print("message table: %d Item_<n>_Name keys; %d emitted ids have no name key"
              % (len(msg_ids), len(orphans)))
        if orphans:
            print("  orphans: %s" % ", ".join(str(o) for o in orphans[:20]))

    # --- write the Lua module ------------------------------------------------
    lines = [
        "-- GENERATED FILE -- DO NOT EDIT BY HAND.",
        "-- Regenerate with tools/item-ids/dump_drop_item_ids.py after a game update.",
        "-- Source: %d pak tables, %d ids." % (len(used_assets), len(merged)),
    ] + [
        "--   %s" % a for a in used_assets
    ] + [
        "-- Maps a field drop-table row key (the FixedId/NormalId an item actor exposes through its",
        "-- ItemTableComponent) to the game's numeric item id. The item's DISPLAYED name is then the",
        "-- game's own localized message `Item_<id>_Name`, so this table holds no text and never needs",
        "-- translating.",
        "return {",
    ]
    for slug in sorted(merged):
        lines.append('    ["%s"] = %d,' % (slug, merged[slug]))
    lines.append("}")
    out_dir = os.path.dirname(OUT_LUA)
    if not os.path.isdir(out_dir):
        os.makedirs(out_dir)
    with open(OUT_LUA, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines) + "\n")

    print("")
    print("ROWS: %d emitted, %d skipped (%d empty DropItemId_1, %d non-item drop)"
          % (len(merged), skipped_none + skipped_non_item, skipped_none, skipped_non_item))
    print("TABLES: %d merged" % len(used_assets))
    print("EPISODE VARIANTS: %d slugs whose drop changes with CondEpisode (lowest emitted)"
          % episode_variants)
    print("CONFLICTS: %d slugs defined differently by a later table (first wins)" % len(conflicts))
    for c in conflicts[:10]:
        print("  %s: %d (%s) kept, %d (%s) ignored" % (c[0], c[2], c[1], c[4], c[3]))
    print("ORPHANS: %d" % len(orphans))
    print("WROTE: %s" % OUT_LUA)


if __name__ == "__main__":
    main()
