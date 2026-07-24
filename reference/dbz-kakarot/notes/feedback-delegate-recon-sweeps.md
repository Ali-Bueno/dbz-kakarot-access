# feedback-delegate-recon-sweeps

> User called out doing RE-recon greps/reads inline instead of delegating them to haiku


2026-07-13: while preparing the skill-palette rewrite I chained ~8 inline Greps/Reads over
AT.hpp / UE4SS_ObjectDump / mod scripts. The user interrupted: "estás omitiendo la regla de
buscar con otros agentes… eso ocupa mucho contexto".

**Why:** the director-model rule (global CLAUDE.md) exists precisely for this: header-dump and
object-dump sweeps produce bulky output that burns the director's context; only the conclusions
matter.

**How to apply:** in this repo, ANY multi-file or dump recon (CXXHeaderDump/AT.hpp, UE4SS_ObjectDump,
`Scripts/dumps/*`) goes to a haiku/sonnet subagent that returns `symbol → offset/verdict` lines,
even when each individual grep looks "targeted". Doing 1–2 pinpoint reads of a known symbol is
still fine; a CHAIN of them is a sweep and must be delegated. Say the delegation plan out loud
before the first read of each task ([dbz-kakarot-engine-facts](dbz-kakarot-engine-facts.md)).
