# Speech arbitration — who gets to talk, and when a cached line is a lie

*Written 2026-09-07, from one user report: "there are mod messages that mix into dialogues, or get
repeated after a loading screen or whatever… and the objective reader on L3 + Triangle doesn't
update, it reads out objectives from quests I already finished."*

**Status: coded, lint-clean, NEVER PLAYED.** Everything below is a mechanism traced in the code, not
a behaviour observed in game. The first cut of this fix contained two defects that adversarial
review caught before it shipped; both are recorded here, because the rejected designs are the
interesting part.

---

## 1. The requeue is a repeat machine when the interrupting stream is periodic

`speech.lua` keeps `pending`: every line spoken with `interrupt=false` that the backend may not have
finished yet. On **every** `interrupt=true` say, the unfinished ones are **re-appended after the
interrupting line** (up to `REQUEUE_MAX = 2` times each).

That is right for what it was built for (2026-07-16): a pickup toast is said once and lost forever
if a subtitle lands on top of it. It is wrong for a line the loop re-emits by itself.

A conversation is a **stream of `interrupt=true` lines** (the shared `Announcer` speaks each new
dialogue line that way). So one beacon distance queued as the player starts talking is spoken again
after the first subtitle, and again after the second: three utterances from one cue, interleaved
into the story. **That is both halves of the report** — the "mixing" and the "repeating" are one
mechanism, and a loading screen is simply a gap long enough for a six-second requeue window to
survive it.

**Fix:** `Nav.say_cue` (`nav_tracker.lua`, under `local Nav = {}`) = `Speech.say(t, false, true)` —
queued, never requeued.

### Which lines belong on it — the review's first correction

The obvious rule ("ambient lines") is wrong, and it is wrong in a way that stays invisible until you
read each call site: **almost every cue commits its latch before it speaks.**

```lua
if dir ~= last_dir_cue and ms - last_dir_ms >= DIR_CUE_MS then
    last_dir_cue, last_dir_ms = dir, ms      -- committed…
    Nav.say_cue(...)                         -- …before the line is even queued
end
```

So "the loop will just say it again" holds only when **the latch re-arms from ordinary movement**.
The working rule:

| On `say_cue` (re-arms as you walk) | Keeps the requeue (one shot per event) |
|---|---|
| direction word, elevation zone, distance filler, stealth side, explore focus, "going around" | retarget label, enemy came into range, arrival + pickup prompt, sweep finished |

The enemy warning is the sharpest case: it latches `last_enemy_key` and re-arms only when that enemy
is at **half** the distance. Lose it once and there is no warning at all until you are twice as
close. The arrival prompt is worse — a hard one-shot under `arrived`, so `Audio.arrival()` plays and
the player is never told which button to press.

## 2. The rejected fix: muting the radar while story text is on screen

The first cut also had `say_cue` return early while the active adapter declared `reads_story`
(a flag on `screen_dialogue`). It was removed the same day, unshipped. Three reasons:

* the dialogue adapter also claims **ambient street chatter** (`FreeTalkWidgetAry`), and bubbles pop
  constantly while walking through a town — the busiest navigation context there is;
* a pooled talk window can sit on a **stale line indefinitely** (this adapter's own header records
  `Field_Talk_Win_C` holding one across two dumps 15 s apart), and nothing would have logged the
  silence, because `nav_mute` stays false on the overworld branch;
* combined with §1, the drop was permanent for every latched cue.

It was also redundant. **A conversation already wins on its own:** its lines interrupt, so a cue in
flight is cut — and, being un-requeued, stays cut. What survives is one queued cue heard *between*
dialogue lines, which is the correct priority rather than a bug.

The standing decision this respects: `screen_dialogue.nav_mute = false` on the overworld branch
(2026-08-03) was about the **audio beacon**, whose gaps were measured and complained about. Speech
is a different currency.

## 3. A queued line must not survive the world it describes

`pending` was never cleared on a map change and its window is up to six seconds — comfortably longer
than a short load. `Speech.flush_pending()` is now registered with the transition gate from
`app.lua` (keyed `"speech"`, so a Ctrl+Shift+R reload replaces it rather than stacking). It also
zeroes `protect_until`, the same stale promise in the other direction: a reward line protected a
breath before the load otherwise keeps **every** reader in the mod deferring to it for six seconds,
against a screen that has nothing to do with it.

It cannot lose a first utterance: `timed_say` already handed the line to the backend, and `pending`
is only the requeue list.

## 4. "The HUD is hidden" and "the HUD says there is nothing" are different facts

`quest_objective.objective_text()` returns `nil` for both. `read_now()` (F10 and the L3 + Triangle
pad chord) fell back to `last_text`, a cache written on announcement and **never cleared** — so once
a quest finished, the key read it back for the rest of the session. The fallback itself is right and
was added deliberately (2026-07-31): the quest HUD is hidden in menus, battles and cutscenes, which
is exactly when a player reaches for the key.

### The review's second correction: `host_ok` was not the evidence it looked like

The 4th return meant "the host widget was found on screen". But every title and row is fetched
through the **strict** `Core.member` gate, which refuses the name whenever the host's property set
is unavailable or partial this tick — the enumeration budget is **one set per tick shared by every
adapter**, and every set is flushed on a map transition. So "host found, nothing on it" and "host
found, nothing askable" arrived identical, and a fix built on that distinction would have announced
*no active objective* for a live quest and then deleted the cache — inverting the fail-open rule on
the one reader that previously degraded gracefully.

`Core.prop_ready(o)` (new, `ui_core.lua`, beside `Core.member`) answers whether the set was
available **and complete**, and `host_ok` is now that. This also tightens the flag's older consumer:
`presence_check` could previously read a budget-starved tick as "the side story ended" and hand the
radar's focus back to the main quest.

### What actually ships

| Where | Change |
|---|---|
| `objective_text` | 4th return is `Core.prop_ready(host)`, not `true` |
| `step` | `empty_hold` counts consecutive readable-and-empty polls; at `GONE_POLLS` it clears **`last_text` only**, never `last_key` (that is the diff gate, and leaving it stale is what stops a re-announce), and logs `objective -> none` |
| `step` | `defer_poll()` — used by the four early-return gates — resets `empty_hold`, because three "consecutive" polls straddling a menu or a load are not consecutive |
| `step` | `last_text = text` on every settled reading, not only when announcing, so a wrongly-cleared cache self-heals on the next poll |
| `read_now` | unchanged from 2026-07-31, deliberately: a per-press verdict was written and rejected, because the HUD repopulates progressively and one sample under a keypress would deny a live quest. The debounce belongs in the loop |
| `Quest.reannounce` | says `objective_none` instead of nothing when both the cache and the live read are empty |

## What to listen for

1. Talk to a field NPC while the radar is tracking: cues should no longer repeat around the
   subtitles. They will still be heard **between** lines — that is intended, not the bug.
2. Cross a loading screen while tracking: nothing from before the load is spoken after it.
3. Finish a quest, then press **L3 + Triangle** in the field: *no active objective*, not the quest
   just finished. Then press it inside a menu with a quest active — it must still read the current
   one there (that is the cache doing its job).
4. Open the map with no objective: it must say *no active objective*, not go silent.
5. The one log line to grep: `objective -> none (quest HUD readable and empty)`.
