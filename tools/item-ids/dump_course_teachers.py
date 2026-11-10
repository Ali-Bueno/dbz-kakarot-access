#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Regenerate mod/KakarotAccess/Scripts/course_teachers.lua from the shipped .pak files.

WHAT THIS PRODUCES
------------------
Which NPCs TEACH a full-course meal recipe, keyed the way the mod can recognise them at runtime,
plus the recipe item ids they grant. The radar's "full-course meals" category (player request
2026-11-09) lists every loaded NPC whose talk id matches a key, and names the recipe through the
game's own `Item_<id>_Name` message, so this file holds no text.

THE GRANT CHAIN (derived 2026-11-09 from the data tables, nothing hand-typed)
----------------------------------------------------------------------------
    ItemTableRecipe            rows with UnLockCourseRecipeId set      = the full-course recipe items
    ItemTradeParameter         trade id -> ItemId                       (for the SETUP_ITEM_TRADE path)
    ActionCommandParam/**      command_id runs INCREASE_ITEM_COUNT <item> or SETUP_ITEM_TRADE <trade>
    NpcTalkParam/**            a talk row whose begin/end_action_command_id is one of those commands
    QuestRewardParam           a quest reward row that lists the recipe item (Sub_Cpl032_01)
NpcTalkParam's talk_event / talk_event_parameter_id / quest_item_param_id play no part: there is not
one COOKING talk event in any table.

TWO KEY SHAPES
--------------
* mini-quest NPCs ("MNQ"): talk ids look like `C1m13041_mad0001s` -- chapter 1, mini-quest 13, step
  04, then the NPC's actor code. The same NPC speaks other steps of the same mini-quest under other
  ids (`C1m13011_mad0001s`), and variants carry suffixes (`_p1`, `_Clear`), so the key is
  "<C1m13>|<mad0001s>". The DLC3 vendor teaches seven courses under one `D3m09…_mad0001s` code.
* sub-quest NPCs: the grant is a quest command (`Sub_Cooking_01_WINDOW`, `Sub_Cpl041A_01_S080_START`)
  or reward (`Sub_Cpl032_01_Reward`), so the key is the quest prefix "Sub_Cooking_01".
At runtime the id is the NPC talk component's `m_talkParamId` (UNpcTalkComponent +0x110; seen live
as `Sub_Npc086_01_Npc086_03`). The mod applies the same two rules -- see Nav._course_teacher.

NOT COVERED: Item_96001 (a main-story gift) and Item_96016 (a DLC event command) have no NPC to
point at. The DLC3 drone shop also sells eight courses; that is a shop, not a teacher.

    set PYTHONIOENCODING=utf-8
    python3 tools/item-ids/dump_course_teachers.py
"""

import os
import re
import shutil
import sys
import tempfile
from collections import defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dump_drop_item_ids as D   # noqa: E402  (the shared pak / DataTable parser)

RECIPE_TABLE = "AT/Content/Parameter/Item/ItemTableRecipe.uasset"
TRADE_TABLE = "AT/Content/Parameter/Field/ItemTradeParameter.uasset"
REWARD_TABLE = "AT/Content/Parameter/Quest/QuestRewardParam.uasset"
OUT_LUA = os.path.join(D.REPO_ROOT, "mod", "KakarotAccess", "Scripts", "course_teachers.lua")

# Must stay equivalent to the Lua pattern in Nav._course_teacher: "^(%u%dm%d%d)%d*_(%a+%d+%a?)".
MNQ_RE = re.compile(r"^([A-Z]\dm\d\d)\d*_([A-Za-z]+\d+[A-Za-z]?)")
SUB_RE = re.compile(r"^(Sub_[A-Za-z0-9]+_\d\d)(?:_|$)")
ITEM_RE = re.compile(r"^Item_(\d+)$")


def val(v):
    """A parsed property value as a plain string (names come back as (rendered, base, num))."""
    if isinstance(v, tuple):
        return v[0]
    return v if isinstance(v, str) else ""


def load(asset, tmp):
    ua = D.extract(asset, tmp)
    ue = D.extract(asset[:-len(".uasset")] + ".uexp", tmp)
    if not ua or not ue:
        return None
    with open(ua, "rb") as fh:
        a = fh.read()
    with open(ue, "rb") as fh:
        b = fh.read()
    return D.parse_data_table(a, b)[1]


def main():
    with open(D.PAK_INDEX, "r", encoding="utf-8", errors="replace") as fh:
        index = [l.strip() for l in fh if l.strip().endswith(".uasset")]
    tmp = tempfile.mkdtemp(prefix="kakcourse_")
    try:
        # 1) the course recipe items
        courses = set()
        for key, p in load(RECIPE_TABLE, tmp) or []:
            if val(p.get("UnLockCourseRecipeId")) not in D.NONE_NAMES:
                m = ITEM_RE.match(str(val(key) if isinstance(key, tuple) else key))
                if m:
                    courses.add(int(m.group(1)))
        if not courses:
            raise SystemExit("no full-course recipe rows found in %s -- parser or path changed" % RECIPE_TABLE)
        is_course = lambda s: bool(ITEM_RE.match(s or "")) and int(ITEM_RE.match(s).group(1)) in courses

        # 2) trade id -> course item
        trade = {}
        for key, p in load(TRADE_TABLE, tmp) or []:
            item = val(p.get("ItemId"))
            if is_course(item):
                trade[str(val(key) if isinstance(key, tuple) else key)] = item

        # 3) commands that hand out a course
        cmd = defaultdict(set)
        for asset in (a for a in index if "/ActionCommandParam/" in a):
            for _, p in load(asset, tmp) or []:
                action, p1 = val(p.get("action_command")), val(p.get("action_param_1"))
                cid = val(p.get("command_id"))
                if action == "SETUP_ITEM_TRADE" and p1 in trade:
                    cmd[cid].add(trade[p1])
                elif action == "INCREASE_ITEM_COUNT" and is_course(p1):
                    cmd[cid].add(p1)

        mnq = defaultdict(set)     # "C1m13|mad0001s" -> items
        quest = defaultdict(set)   # "Sub_Cooking_01" -> items
        origin = defaultdict(set)  # key -> provenance strings, written as comments
        used = set()
        # 4) talk rows that run one of those commands
        for asset in (a for a in index if "/NpcTalkParam/" in a):
            for _, p in load(asset, tmp) or []:
                tid = val(p.get("talk_id"))
                for f in ("begin_action_command_id", "end_action_command_id"):
                    c = val(p.get(f))
                    if c not in cmd:
                        continue
                    m = MNQ_RE.match(tid)
                    if not m:
                        continue
                    k = m.group(1) + "|" + m.group(2)
                    mnq[k] |= cmd[c]
                    origin[k].add("talk %s -> %s" % (tid, c))
                    used.add(c)
        # 5) quest commands with no talk row: key by quest prefix
        for c, items in cmd.items():
            if c in used:
                continue
            m = SUB_RE.match(c)
            if m:
                quest[m.group(1)] |= items
                origin[m.group(1)].add("command %s" % c)
            else:
                print("  no NPC for command %s -> %s" % (c, sorted(items)))
        # 6) quest rewards
        for key, p in load(REWARD_TABLE, tmp) or []:
            rid = val(p.get("id")) or str(key)
            for f, v in p.items():
                s = val(v)
                if f.startswith("item_id") and is_course(s):
                    m = SUB_RE.match(rid)
                    if m:
                        quest[m.group(1)].add(s)
                        origin[m.group(1)].add("reward %s" % rid)
                    else:
                        print("  no NPC for reward %s -> %s" % (rid, s))
    finally:
        shutil.rmtree(tmp, ignore_errors=True)

    def ids(items):
        return ", ".join(str(int(ITEM_RE.match(i).group(1))) for i in sorted(items))

    lines = [
        "-- GENERATED FILE -- DO NOT EDIT BY HAND.",
        "-- Regenerate with tools/item-ids/dump_course_teachers.py after a game update.",
        "-- NPCs that teach a full-course meal recipe, keyed by their talk id (UNpcTalkComponent",
        "-- m_talkParamId): `mnq` by \"<C1m13>|<actor code>\", `quest` by sub-quest prefix. Values are",
        "-- recipe item ids; the spoken name is the game's own Item_<id>_Name. Ids only, no text.",
        "return {",
        "    mnq = {",
    ]
    for k in sorted(mnq):
        lines.append("        -- %s" % "; ".join(sorted(origin[k])))
        lines.append('        ["%s"] = { %s },' % (k, ids(mnq[k])))
    lines += ["    },", "    quest = {"]
    for k in sorted(quest):
        lines.append("        -- %s" % "; ".join(sorted(origin[k])))
        lines.append('        ["%s"] = { %s },' % (k, ids(quest[k])))
    lines += ["    },", "}"]
    with open(OUT_LUA, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines) + "\n")
    covered = set().union(*mnq.values(), *quest.values()) if (mnq or quest) else set()
    print("COURSES: %d recipe items; %d have a teacher" % (len(courses), len(covered)))
    print("  without a teacher: %s" % ", ".join(str(c) for c in sorted(courses)
                                              if "Item_%d" % c not in covered))
    print("KEYS: %d mini-quest NPCs, %d sub-quests" % (len(mnq), len(quest)))
    print("WROTE: %s" % OUT_LUA)


if __name__ == "__main__":
    main()
