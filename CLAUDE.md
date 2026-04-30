# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

Single-file C++17 project targeting Botzone's submission format (one `.cpp` file with no external dependencies).

```bash
make          # Release build (-O2 -DNDEBUG)
make debug    # Debug build with ASan/UBSan (-g -O0)
make clean    # Remove binary
```

Manual build:

```bash
g++ -std=c++17 -O2 -DNDEBUG naniwa.cpp -o naniwa
g++ -std=c++17 -g -O0 -fsanitize=address,undefined naniwa.cpp -o naniwa
```

## Formatting

Uses clang-format with Google base style and 4-space indent:

```bash
clang-format -i naniwa.cpp
```

The `// clang-format off` / `// clang-format on` block around enums and piece-square tables is intentional — those use a custom column layout that auto-formatting destroys.

## Architecture

This is a **Chinese Chess (Xiangqi) engine** designed as a competitive bot for Botzone. Everything lives in one file due to Botzone's single-file submission requirement.

### Board Representation

- **Grid system**: 10×9 board encoded as a 160-value enum (`Grid`). Each row occupies 16 entries so that grid arithmetic (`grid + delta`) naturally detects out-of-bounds moves — wrapping to an unused slot is equivalent to going out of bounds. `to_grid(r, c)` = `r << 4 | c`.
- **Piece encoding**: `Piece` enum with `RED_PIECE = (0 << 3) + type`, `BLACK_PIECE = (1 << 3) + type`. Color extracted via `piece >> 3`, type via `piece & 0x7`.
- **Move encoding**: 16-bit `uint16_t` packing `from << 8 | to`.
- **Position struct** holds: `board[160]`, `king_positions[COLOR_NB]` (direct king-grid lookup), Zobrist hash, move history, incremental evaluation values, and `ply_since_capture` (for 50-move rule).

### Key subsystems (in file order)

1. **Piece-square tables** (lines 70–156): `PIECE_POSITION_VALUES[PIECE_TYPE_NB][GRID_NB]`. Values from Red's perspective, including implicit material value. `get_piece_value()` returns the table entry for Red, mirrored for Black. Also `PIECE_VALUES[]` (line 158) for move-ordering heuristics.

2. **Precomputed bitsets** (lines 177–229): `IN_BOUNDS` (256-entry bitset for valid grids), `PALACE` (king zones d0-f2 and d7-f9), `RIVER` (rows 4-5), `SIDE[COLOR]` (each side's half of the board). All computed via immediately-invoked lambdas at static init.

3. **Zobrist hashing** (lines 267–296): `constexpr` table generated at compile time via `splitmix64`. Incrementally updated on `add_piece`/`remove_piece`/`change_player`. Stored in `HistoryEntry` for repetition detection.

4. **Move generation** (`generate_pseudo_legal_moves`, lines 512–549): Iterates over the full board (90 squares), dispatches to per-piece-type generators (`generate_king_moves`, etc.). Move deltas use `{-16, -1, 1, 16}` for orthogonal sliding (rook/cannon), `{-17, -15, 15, 17}` for diagonal (advisor/bishop) — note that with 16-wide rows, `±16` is one row, `±1` is one column.

5. **Check detection** (`is_in_check`, lines 402–426): Tests whether `color`'s king is attacked. Composed into `is_attacked_by_knight` (checks leg-blocking via `ADVISOR_MOVES` pins), `is_attacked_by_rook_or_cannon` (sliding with cannon's platform jump), `is_attacked_by_pawn` (forward and sideways), plus "flying general" (kings facing on same column with nothing between).

6. **Repetition detection** (`repetition_status`, lines 482–510): Walks history since last capture, counts zobrist matches. Requires all intervening positions to be checks for the repetition to count — perpetual check by one side is a LOSS, mutual perpetual check is a DRAW.

7. **History / make-unmake**: `make_move` (line 356) saves pre-move zobrist in the history entry, then updates state. `undo_move` (line 374) restores everything from the entry. This correctly preserves `ply_since_capture` and zobrist across undo.

8. **Null move** (lines 665–685): For future search pruning. `can_do_null_move()` checks not in check and material >= 600.

9. **Evaluation** (line 767): `evaluate()` returns `values[player] - values[opponent(player)]`, a simple positional-material score accumulated incrementally during `add_piece`/`remove_piece`.

10. **Perft** (lines 798–816): Generates pseudo-legal moves, filters out self-check and illegal repetitions, recursively counts leaf nodes. Currently operates on a mid-game FEN (line 796), not the initial position. For more perft test case, see [Chinese Chess Perft Results](https://www.chessprogramming.org/Chinese_Chess_Perft_Results#Initial_Position)

### Botzone constraints

- Single `.cpp` file, no `#include` of custom headers
- No filesystem or network access
- Communication via stdin/stdout (Botzone protocol, not yet implemented)
- Must handle FEN strings for position setup
