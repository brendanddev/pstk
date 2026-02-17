
# pkmnemerald-lib   v0.0.1
A C library for parsing, reading, and modifying Pokemon Emerald save files at the binary level.

---

## Planned Features

### Save File I/O
- Load a `.sav` file from disk
- Validate the save file (correct size and structure)
- Verify and recalculate the checksum
- Write the modified save back to disk

### Player Data
- Read/write player name
- Read/write money
- Read/write playtime
- Read/write badges
- Read/write gender

### Pokemon
- Read/write party Pokemon (up to 6)
- Read/write PC box Pokemon
- Read/write moves
- Read/write stats (HP, Attack, Defense, etc.)
- Read/write level
- Read/write nickname
- Read/write species

---

## References & Resources

### Save Format Documentation
- [pokeemerald decomp](https://github.com/pret/pokeemerald) — Full decompilation of Pokemon Emerald in C, primary reference for data structures and memory layout
- [Bulbapedia - Save Data Structure](https://bulbapedia.bulbagarden.net/wiki/Save_data_structure_(Generation_III)) —  Generation III save format documentation
- [PKHeX](https://github.com/kwsch/PKHeX) — Reference for save parsing logic

### GBA/Technical
- [GBATEK](https://problemkaputt.de/gbatek.htm) — GBA hardware and memory documentation
- [mGBA](https://mgba.io) — Emulator used for testing

### Tools
- [ImHex](https://imhex.werwolv.net) — Hex editor used for inspecting raw save data
