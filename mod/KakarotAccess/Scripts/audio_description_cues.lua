-- Timed visual descriptions for the opening through the immediate aftermath of
-- the Raditz battle (C01_020_S060).
--
-- MOV timestamps were authored against the installed USM files. `before` is the
-- end of a dialogue-free interval verified against BOTH mono dialogue tracks;
-- the offline catalog test proves the same speech-duration estimate used by
-- speech.lua fits before that boundary. A few essential battle beats have no
-- `before`: those are attempted at the exact visual moment and the live dialogue
-- reader is the final gate, so a subtitle always wins.
--
-- GDM entries are restrained scene-setting lines for real-time sequencer demos.
-- They are keyed by the master sequence name recovered from the installed PAKs.

local CUES = {
    -- Title-screen opening (Content/Movies/C00_000_S000_mov.usm, 1:49). A music montage
    -- with no dialogue, so the cues run back to back with no `before` gaps to respect.
    -- Authored 2026-09-08 from FFmpeg contact sheets of the installed USM (1 frame/s
    -- across every cut), timed on the file's own clock, which is the Mana clock the
    -- adapter reads (the log already named this source at the title screen).
    C00_000_S000_MOV = {
        { at = 0.40,   text = "On red, the four-star ball lights up." },
        { at = 5.20,   text = "The Dragon Ball Z logo over a sunny sea." },
        { at = 10.20,  text = "Kakarot completes the title." },
        { at = 15.20,  text = "A fast flight over green cliffs." },
        { at = 18.30,  text = "The four-star ball on Gohan's hat." },
        { at = 20.60,  text = "Goku and Gohan smile side by side." },
        { at = 24.20,  text = "A huge fish leaps from the water." },
        { at = 26.30,  text = "Goku eats with chopsticks." },
        { at = 28.30,  text = "Goku on the Nimbus, Piccolo flying beside him." },
        { at = 30.40,  text = "Bulma waves." },
        { at = 32.30,  text = "Master Roshi with his staff at Kame House." },
        { at = 34.40,  text = "King Kai outside his round house." },
        { at = 36.40,  text = "A frozen sea under snowy mountains." },
        { at = 40.40,  text = "A green meadow among trees." },
        { at = 44.30,  text = "Kame House on its tiny island." },
        { at = 48.30,  text = "West City and the Capsule Corp domes." },
        { at = 51.20,  text = "A dinosaur chases young Gohan." },
        { at = 52.60,  text = "Goku grapples Raditz, then Frieza." },
        { at = 56.00,  text = "Krillin raises a Destructo Disc." },
        { at = 58.30,  text = "Goku roars, powering up." },
        { at = 60.20,  text = "A Great Ape against the full moon." },
        { at = 62.40,  text = "Fists fly at the camera." },
        { at = 65.20,  text = "Gohan is blasted back." },
        { at = 66.50,  text = "Vegeta erupts into Super Saiyan." },
        { at = 69.20,  text = "Piccolo charges a beam at his fingertips." },
        { at = 70.50,  text = "Trunks fires from his palm." },
        { at = 73.20,  text = "Vegeta powers up in a crater." },
        { at = 75.20,  text = "Goku crouches on a rock." },
        { at = 76.50,  text = "Goku and Vegeta, face to face." },
        { at = 78.20,  text = "Frieza screams." },
        { at = 81.20,  text = "Cell fires a green beam." },
        { at = 82.40,  text = "Super Saiyan Vegeta shouts." },
        { at = 84.50,  text = "Vegeta bursts with pink energy." },
        { at = 87.20,  text = "Goku dives, shouting." },
        { at = 88.50,  text = "The land seen from far above." },
        { at = 90.40,  text = "Goku grins in a fighting stance." },
        { at = 92.40,  text = "Red rock towers across a wide valley." },
        { at = 94.30,  text = "A Kamehameha builds and fires." },
        { at = 98.20,  text = "Three pillars of light rise at night." },
        { at = 100.50, text = "The heroes stand together on red." },
        { at = 104.30, text = "The team, three of them Super Saiyan." },
    },

    C01_000_S010_MOV = {
        { at = 0.10,  before = 1.85,  text = "A bamboo forest." },
        { at = 11.60, before = 13.91, text = "Young Gohan watches." },
        { at = 17.20, before = 23.11, text = "Goku sits cross-legged and meditates beside Gohan." },
        { at = 24.00, before = 37.50, text = "Gohan grows drowsy while Goku remains still." },
        { at = 37.80, before = 41.28, text = "Piccolo appears behind Goku." },
        { at = 53.50, before = 58.30, text = "Goku and Piccolo charge at each other." },
        { at = 60.10, before = 64.30, text = "They trade a blur of punches." },
        { at = 64.60, before = 67.47, text = "Their fists collide." },
    },

    C01_000_S020_MOV = {
        { at = 0.15, before = 2.62, text = "Piccolo fires." },
        { at = 8.80, before = 13.00, text = "Goku's blue beam meets Piccolo's blast." },
    },

    -- A rapid narrated recap. It was reviewed, but contains no common silent
    -- window long enough for even the shortest useful description.
    C01_000_S040_MOV = {},

    C01_010_S010_GDM = {
        { at = 0.25, text = "On a distant, burning world, Vegeta, Nappa, and Raditz stand together." },
    },

    C01_010_S020_GDM = {
        { at = 0.25, text = "Goku stands alone beneath an open sky." },
    },

    C01_010_S025_MOV = {
        { at = 0.20,  before = 3.53,  text = "Goku straightens Gohan's hat." },
        { at = 4.30,  before = 6.13,  text = "They hug." },
        { at = 9.70,  before = 14.62, text = "Father and son climb onto the Flying Nimbus." },
        { at = 25.00, before = 28.48, text = "A red dinosaur lunges at them." },
        { at = 29.80, before = 31.90, text = "Nimbus speeds away." },
    },

    C01_010_S030_MOV = {
        { at = 0.30,  before = 5.58,  text = "Goku and Gohan arrive home on the Flying Nimbus." },
        { at = 17.50, before = 20.38, text = "The four-star ball gleams." },
        { at = 67.80, before = 71.23, text = "Chi-Chi waves as they leave." },
    },

    C01_010_S040_MOV = {
        { at = 32.30, before = 34.90, text = "Goku and Gohan smile." },
    },

    C01_010_S070_GDM = {
        { at = 0.25, text = "Goku and Gohan reunite with Krillin outside Kame House." },
    },

    C01_010_S080_MOV = {
        -- ViddyScribe-assisted draft, shortened and retimed against source frames
        -- and BOTH voice tracks. Music/effects may overlap; spoken dialogue may not.
        -- Generated source: ViddyScribe (https://viddyscribe.com), edited locally.
        -- Review: reference/dbz-kakarot/notes/dbz-kakarot-cutscene-descriptions.md.
        { at = 0.20,  before = 4.73,   text = "A farmer bends beside his truck." },
        { at = 6.00,  before = 8.65,   text = "A fireball streaks by." },
        { at = 8.90,  before = 14.65,  text = "It crashes, throwing up dust." },
        { at = 20.00, before = 24.40,  text = "A round pod rests in the crater." },
        { at = 24.50, before = 27.00,  text = "The hatch is open." },
        { at = 31.80, before = 35.70,  text = "A long-haired warrior emerges in armor." },
        { at = 36.00, before = 40.70,  text = "He floats out of the crater." },
        { at = 41.00, before = 44.60,  text = "The farmer aims his shotgun." },
        { at = 45.50, before = 47.80,  text = "Raditz lands." },
        { at = 57.35, before = 59.66,  text = "Scouter glows green." },
        { at = 71.00, before = 73.80,  text = "He catches the bullet." },
        { at = 74.10, before = 76.63,  text = "He flicks it back." },
        { at = 77.30, before = 79.43,  text = "The farmer falls." },
        { at = 93.60, before = 97.80,  text = "Raditz rockets skyward in a white streak." },
        { at = 98.20, before = 101.90, text = "A vast rocky canyon stretches below." },
        { at = 104.00, before = 107.84, text = "Piccolo hovers above a ridge." },
    },

    C01_010_S120_GDM = {
        { at = 0.25, text = "Above Kame House, Goku faces Raditz while his friends watch below." },
    },

    C01_020_S010_GDM = {
        { at = 0.25, text = "Goku rides the Flying Nimbus as Piccolo flies beside him." },
    },

    C01_020_S030_GDM = {
        { at = 0.25, text = "Goku and Piccolo face Raditz in the rocky wasteland." },
    },

    C01_020_S040_MOV = {
        { at = 2.40,   before = 4.93,   text = "Goku crashes down." },
        { at = 8.60,   before = 12.21,  text = "Goku lies battered in a crater." },
        { at = 104.80, before = 107.30, text = "Goku grips Raditz's tail." },
        { at = 107.40, before = 110.00, text = "Raditz collapses, helpless." },
        { at = 110.20, before = 112.50, text = "Deceived, Goku lets go." },
        { at = 113.00, before = 115.20, text = "Raditz springs upright." },
        { at = 115.20, before = 117.04, text = "Gohan bursts free." },
        { at = 122.00, text = "Gohan smashes headfirst into Raditz's chest." },
        { at = 135.00, text = "Piccolo raises two fingers as energy gathers at their tips." },
        { at = 151.00, text = "Piccolo fires the spiraling Special Beam Cannon." },
        { at = 154.00, text = "The beam pierces Raditz and Goku together." },
        { at = 164.50, before = 166.43, text = "Both brothers fall." },
    },

    C01_020_S060_MOV = {
        { at = 0.20,  before = 2.08,  text = "A green planet." },
        { at = 6.80,  before = 13.97, text = "On a jungle world, Vegeta and Nappa sit beside their pods." },
        { at = 16.95, before = 18.82, text = "Nappa listens." },
        { at = 31.00, before = 34.16, text = "Nappa rises beside Vegeta." },
        { at = 44.65, before = 46.55, text = "Vegeta smirks." },
        { at = 60.30, before = 65.11, text = "Their pods launch from the planet." },
        { at = 73.10, before = 75.73, text = "They streak away." },
    },
}

-- Every cue carries a stable lang-file key, `ad_<source>_<at in centiseconds>`, so the
-- text above stays the English source (and the timing QA below stays valid for it) while
-- lang/<code>.txt supplies the same line in the mod's other languages. The adapter looks
-- the key up and falls back to `text` when a language has no entry.
for source, list in pairs(CUES) do
    for _, cue in ipairs(list) do
        cue.key = string.format("ad_%s_%d", source, math.floor(cue.at * 100 + 0.5))
    end
end

return CUES
