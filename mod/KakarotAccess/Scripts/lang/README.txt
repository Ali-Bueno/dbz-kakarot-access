KakarotAccess — language files
===============================

Every line the mod SPEAKS comes from one of these text files, one per language:

    en.txt  es.txt  fr.txt  de.txt  it.txt  pt.txt  ru.txt
    pl.txt  ja.txt  ko.txt  zh.txt  ar.txt  th.txt

The mod automatically loads the file that matches the GAME's language (Spanish game ->
es.txt, French game -> fr.txt, ...). You can also force a language from the in-game config
menu (open it with L3 + R3 in the overworld -> "Language").

Editing a language
------------------
Open the file for your language in any plain-text editor (save as UTF-8). Each line is:

    key = value

Change only the VALUE (the text on the RIGHT of the '='). Keep the key (left side) exactly.
Example:

    reader_on = Menu reader on          ->   reader_on = Menu reading enabled

Rules
-----
* Lines starting with '#' are comments; blank lines are ignored.
* Keep the placeholders %s and %d exactly where the sentence needs them — they are replaced
  at runtime by a name or a number (e.g. "Tracking %s, %d meters"). You may move one to fit
  your grammar, but never delete or duplicate it.
* \n  in a value becomes a line break.
* \s  means a single space at the very start or end of a value (normal editors trim edge
  spaces, so this keeps them). Example:  combo_join = \splus\s  ->  " plus "  — used to join
  two button names ("L3 plus R3"). Leave the \s markers in place.
* Names like A, B, X, Y, L1, R1, L3, R3 are on-screen button glyphs — leave them as they are.

Your edits take effect on the next game launch, or immediately with Ctrl+Shift+R in-game.
If a file is missing or a key is deleted, the mod falls back to English (built in), so you
can never break it — worst case a line is spoken in English.

Adding a NEW language
---------------------
The game ships 13 languages (see the list above). If the game adds one the mod doesn't know,
copy en.txt to <code>.txt (the 2-letter base code the game uses, e.g. "sv") and translate it;
the mod will pick it up when the game runs in that language.
