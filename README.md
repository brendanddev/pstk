# pstk

Pokémon Save ToolKit, a C library for reading and editing Pokémon save files. Currently supports Gen 3 (Emerald). Work in progress.

---

## Supported Features

### Save File I/O
- Load a `.sav` file from disk
- Validate the save file (size, sector signatures, checksums)
- Verify and recalculate sector checksums
- Select the current save slot (of the two the game alternates between)
- Write a modified save back to disk, recomputing checksums

### Player Data
- Read/write player name
- Read player gender, trainer ID, secret ID
- Read/write playtime
- Read/write money (handles Emerald's XOR encryption)

### Debugging
- Print every sector's footer info and checksum status
- Hex-dump a sector's contents

---

## Planned Features

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
