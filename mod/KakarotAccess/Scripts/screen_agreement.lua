-- Screen adapter: boot-time agreement viewer (AT_UIXcmnAgreement).
--
-- The first screens after launch: Bandai Namco's EULA / privacy-policy / data-analysis
-- consent documents — one viewer widget cycling through them. Every page is a pre-baked
-- TEXTURE (ImageTextureMap, AT.hpp 0x508) — the widget holds no readable text at all
-- (two F7 censuses: only the keyhelp bar carries strings), and brush reads on its images
-- throw the PIERCING nullptr error (killed two dumps, 2026-07-17 — never read brushes
-- here). So the reader announces WHAT the screen is (localized label + "shown as an
-- image" note), leaves the actions bar ("Confirm agreement / Change language / Next")
-- to the automatic keyhelp read, and names the DOCUMENT and PAGE from the widget's own
-- native state (Ghidra-confirmed, see native_offsets.agreement): docId @0x5B4
-- (0=EULA, 1=privacy, 2|3=KPI/data analysis), page @0x5A8, count @0x5B0. The reads are
-- sanity-checked (page within count, docId in range) and simply stay silent if the
-- bridge or a game patch breaks them — the entry announcement never depends on them.
-- The Accept/Reject consent popup raised by "Confirm agreement" is a standard pooled
-- Xcmn window — the dialog reader above this adapter owns it.
--
-- Detection is pointer reads off an ALREADY-CACHED class: Gametitle_C (live and
-- interactive underneath the agreement — F7 2026-07-17) → ActorTitle (AT_UIGameTitle
-- @reflected) → AgreementDialog (AT_Title @0x340, reflected). Falls back to a budgeted
-- scan of the native widget class. Gated on Core.pane_live per the pooled-pane rule.

local Core = require("ui_core")
local I18n = require("i18n")
local Speech = require("speech")
local Mem = require("mem")
local OFF = require("native_offsets")

local Agreement = {}

local ann = Core.make_announcer()
local host = nil
local tick = 0

-- Last announced (doc, page). Kept across reset(): the consent popup opening over the
-- viewer must not re-trigger the same announcement when the viewer takes the tick back.
local last_doc, last_page = nil, nil

-- Document label from the game's own id (the Ghidra-recovered enum). 2 and 3 are the
-- same KPI/data-analysis notice (3 = W180 region variant).
local DOC_KEY = {
    [0] = "agreement_doc_eula",
    [1] = "agreement_doc_privacy",
    [2] = "agreement_doc_kpi",
    [3] = "agreement_doc_kpi",
}
local function doc_label(doc)
    local key = DOC_KEY[doc]
    if key then return I18n.t(key) end
    return string.format(I18n.t("agreement_doc_fmt"), doc + 1)
end

local function find_host()
    local gt = Core.cached_live("Gametitle_C", tick)
    if Core.valid(gt) then
        local d
        pcall(function()
            local actor = gt.ActorTitle
            if actor and actor:IsValid() then d = actor.AgreementDialog end
        end)
        if Core.valid(d) then return d end
    end
    return Core.first_on_screen("AT_UIXcmnAgreement", tick)
end

-- Native doc/page state, self-checked; nils when unavailable (mem_bridge missing,
-- widget mid-teardown, or a game patch moved the members).
local function read_state()
    local count = Mem.i32(host, OFF.agreement.pageCount)
    if not count or count <= 0 or count >= 32 then return nil end
    local page = Mem.i32(host, OFF.agreement.page)
    if not page or page < 0 or page >= count then return nil end
    local doc = Mem.i32(host, OFF.agreement.docId)
    if not doc or doc < 0 or doc > 15 then return nil end
    return doc, page, count
end

function Agreement.is_active()
    tick = tick + 1
    host = find_host()
    if not Core.valid(host) or not Core.on_screen(host) then return false end
    return Core.pane_live(host)
end

function Agreement.reset() ann:reset() end

function Agreement.update()
    -- Entry announcement (screen label + image note). was_open distinguishes a fresh
    -- entry (queue the doc/page behind the note) from an in-screen flip (interrupt).
    local was_open = ann.open
    ann:focus(I18n.t("agreement_screen"), nil, nil, nil, nil)
    local doc, page, count = read_state()
    if not doc then return end
    if was_open and doc == last_doc and page == last_page then return end
    local doc_changed = (doc ~= last_doc)
    last_doc, last_page = doc, page
    local label = string.format(I18n.t("agreement_page_fmt"), page + 1, count)
    if doc_changed or not was_open then
        label = doc_label(doc) .. ", " .. label
    end
    Speech.say(label, was_open)
end

return Agreement
