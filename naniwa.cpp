#include <array>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

// clang-format off
enum PieceType : uint8_t {
    PIECE_NONE, KING, ADVISOR, BISHOP, KNIGHT, ROOK, CANNON, PAWN, PIECE_TYPE_NB
};

enum Color : uint8_t {
    RED, BLACK, COLOR_NB
};

inline Color opponent(Color color) {
    return (color == RED) ? BLACK : RED;
}

enum Piece : uint8_t {
    EMPTY,
    RED_KING, RED_ADVISOR, RED_BISHOP, RED_KNIGHT, RED_ROOK, RED_CANNON, RED_PAWN,
    BLACK_KING = RED_KING + 8, BLACK_ADVISOR, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK, BLACK_CANNON, BLACK_PAWN,
    PIECE_NB,
};

inline Piece make_piece(Color c, PieceType pt) {
    return Piece((c << 3) + pt);
}

inline constexpr const char* PIECE_CHAR = ".kabnrcp.KABNRCP";

constexpr PieceType type_of(Piece piece) {
    return PieceType(piece & 0x7);
}

constexpr Color color_of(Piece piece) {
    assert(piece != EMPTY);
    return Color(piece >> 3);
}

enum Row : uint8_t {
    ROW_0, ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8, ROW_9, ROW_NB
};

enum Col : uint8_t {
    COL_A, COL_B, COL_C, COL_D, COL_E, COL_F, COL_G, COL_H, COL_I, COL_NB
};

enum Grid : uint8_t {
    A0          , B0, C0, D0, E0, F0, G0, H0, I0,
    A1 = A0 + 16, B1, C1, D1, E1, F1, G1, H1, I1,
    A2 = A1 + 16, B2, C2, D2, E2, F2, G2, H2, I2,
    A3 = A2 + 16, B3, C3, D3, E3, F3, G3, H3, I3,
    A4 = A3 + 16, B4, C4, D4, E4, F4, G4, H4, I4,
    A5 = A4 + 16, B5, C5, D5, E5, F5, G5, H5, I5,
    A6 = A5 + 16, B6, C6, D6, E6, F6, G6, H6, I6,
    A7 = A6 + 16, B7, C7, D7, E7, F7, G7, H7, I7,
    A8 = A7 + 16, B8, C8, D8, E8, F8, G8, H8, I8,
    A9 = A8 + 16, B9, C9, D9, E9, F9, G9, H9, I9,
    GRID_NB = 160
};

// 子力位置价值表
constexpr int PIECE_POSITION_VALUES[PIECE_TYPE_NB][GRID_NB] = {
    {},
    { // KING
        0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    },
    { // ADVISOR
        0,  0,  0, 50,  0, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0, 58,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 50,  0, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 50,  0, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0, 58,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0, 50,  0, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    },
    { // BISHOP
        0,  0, 50,  0,  0,  0, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       46,  0,  0,  0, 56,  0,  0,  0, 46,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0, 50,  0,  0,  0, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0, 50,  0,  0,  0, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       46,  0,  0,  0, 56,  0,  0,  0, 46,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0, 50,  0,  0,  0, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    },
    { // KNIGHT
       88, 85, 90, 88, 90, 88, 90, 85, 88,  0,  0,  0,  0,  0,  0,  0,
       85, 90, 92, 93, 78, 93, 92, 90, 85,  0,  0,  0,  0,  0,  0,  0,
       93, 92, 94, 95, 92, 95, 94, 92, 93,  0,  0,  0,  0,  0,  0,  0,
       92, 94, 98, 95, 98, 95, 98, 94, 92,  0,  0,  0,  0,  0,  0,  0,
       90, 98,101,102,103,102,101, 98, 90,  0,  0,  0,  0,  0,  0,  0,
       90,100, 99,103,104,103, 99,100, 90,  0,  0,  0,  0,  0,  0,  0,
       93,108,100,107,100,107,100,108, 93,  0,  0,  0,  0,  0,  0,  0,
       92, 98, 99,103, 99,103, 99, 98, 92,  0,  0,  0,  0,  0,  0,  0,
       90, 96,103, 97, 94, 97,103, 96, 90,  0,  0,  0,  0,  0,  0,  0,
       90, 90, 90, 96, 90, 96, 90, 90, 90,  0,  0,  0,  0,  0,  0,  0,
    },
    { // ROOK
      194,206,204,212,200,212,204,206,194,  0,  0,  0,  0,  0,  0,  0,
      200,208,206,212,200,212,206,208,200,  0,  0,  0,  0,  0,  0,  0,
      198,208,204,212,212,212,204,208,198,  0,  0,  0,  0,  0,  0,  0,
      204,209,204,212,214,212,204,209,204,  0,  0,  0,  0,  0,  0,  0,
      208,212,212,214,215,214,212,212,208,  0,  0,  0,  0,  0,  0,  0,
      208,211,211,214,215,214,211,211,208,  0,  0,  0,  0,  0,  0,  0,
      206,213,213,216,216,216,213,213,206,  0,  0,  0,  0,  0,  0,  0,
      206,208,207,214,216,214,207,208,206,  0,  0,  0,  0,  0,  0,  0,
      206,212,209,216,233,216,209,212,206,  0,  0,  0,  0,  0,  0,  0,
      206,208,207,213,214,213,207,208,206,  0,  0,  0,  0,  0,  0,  0,
    },
    { // CANNON
       96, 96, 97, 99, 99, 99, 97, 96, 96,  0,  0,  0,  0,  0,  0,  0,
       96, 97, 98, 98, 98, 98, 98, 97, 96,  0,  0,  0,  0,  0,  0,  0,
       97, 96,100, 99,101, 99,100, 96, 97,  0,  0,  0,  0,  0,  0,  0,
       96, 96, 96, 96,100, 96, 96, 96, 96,  0,  0,  0,  0,  0,  0,  0,
       95, 96, 99, 96,100, 96, 99, 96, 95,  0,  0,  0,  0,  0,  0,  0,
       96, 96, 96, 96, 96, 96, 96, 96, 96,  0,  0,  0,  0,  0,  0,  0,
       96, 99, 99, 98,100, 98, 99, 99, 96,  0,  0,  0,  0,  0,  0,  0,
       97, 97, 96, 91, 92, 91, 96, 97, 97,  0,  0,  0,  0,  0,  0,  0,
       98, 98, 96, 92, 89, 92, 96, 98, 98,  0,  0,  0,  0,  0,  0,  0,
      100,100, 96, 91, 90, 91, 96,100,100,  0,  0,  0,  0,  0,  0,  0,  
    },
    { // Pawn
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       24,  0, 24,  0, 30,  0, 24,  0, 24,  0,  0,  0,  0,  0,  0,  0,
       24,  0, 30,  0, 36,  0, 30,  0, 24,  0,  0,  0,  0,  0,  0,  0,
       30, 36, 40, 50, 54, 50, 40, 36, 30,  0,  0,  0,  0,  0,  0,  0,
       40, 48, 54, 58, 60, 58, 54, 48, 40,  0,  0,  0,  0,  0,  0,  0,
       40, 50, 64, 74, 74, 74, 64, 50, 40,  0,  0,  0,  0,  0,  0,  0,
       40, 50, 68, 84, 88, 84, 68, 50, 40,  0,  0,  0,  0,  0,  0,  0,
       20, 20, 20, 24, 26, 24, 20, 20, 20,  0,  0,  0,  0,  0,  0,  0,
    },
};

constexpr int PIECE_VALUES[PIECE_TYPE_NB] = {0, 5, 1, 1, 3, 4, 3, 2};
// clang-format on

inline constexpr Grid to_grid(int r, int c) { return Grid(r << 4 | c); }

inline int row_of(Grid grid) { return (grid >> 4) & 0xf; }

inline int col_of(Grid grid) { return grid & 0xf; }

inline Grid mirror(Grid grid) {
    return to_grid(ROW_9 - row_of(grid), col_of(grid));
}

int get_piece_value(Piece piece, Grid grid) {
    return (color_of(piece) == RED)
               ? PIECE_POSITION_VALUES[type_of(piece)][grid]
               : PIECE_POSITION_VALUES[type_of(piece)][mirror(grid)];
}

const auto IN_BOUNDS = [] {
    std::bitset<256> in_bounds{};

    for (int r = ROW_0; r <= ROW_9; ++r) {
        for (int c = COL_A; c <= COL_I; ++c) {
            in_bounds.set(to_grid(r, c));
        }
    }

    return in_bounds;
}();

const auto PALACE = [] {
    std::bitset<GRID_NB> king_squares{};

    for (int c = COL_D; c <= COL_F; ++c) {
        for (int r = ROW_0; r <= ROW_2; ++r) {
            king_squares.set(to_grid(r, c));
        }
        for (int r = ROW_7; r <= ROW_9; ++r) {
            king_squares.set(to_grid(r, c));
        }
    }

    return king_squares;
}();

const auto RIVER = [] {
    std::bitset<GRID_NB> river_squares{};

    for (int c = COL_A; c <= COL_I; ++c) {
        for (int r = ROW_4; r <= ROW_5; ++r) {
            river_squares.set(to_grid(r, c));
        }
    }

    return river_squares;
}();

const auto SIDE = [] {
    std::array<std::bitset<GRID_NB>, COLOR_NB> side{};

    for (int c = COL_A; c <= COL_I; ++c) {
        for (int r = ROW_0; r <= ROW_4; ++r) {
            side[RED].set(to_grid(r, c));
        }
        for (int r = ROW_5; r <= ROW_9; ++r) {
            side[BLACK].set(to_grid(r, c));
        }
    }

    return side;
}();

struct Move {
    Move() = default;

    constexpr explicit Move(uint16_t move) : move(move) {}

    constexpr Move(Grid from, Grid to) : move(from << 8 | to) {}

    constexpr Grid from() const { return (Grid)((move >> 8) & 0xff); }

    constexpr Grid to() const { return (Grid)(move & 0xff); }

    constexpr bool operator==(const Move& other) const {
        return move == other.move;
    }

    constexpr bool operator!=(const Move& other) const {
        return move != other.move;
    }

    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        return os << char('a' + col_of(move.from()))
                  << (row_of(move.from()) + 0) << char('a' + col_of(move.to()))
                  << (row_of(move.to()) + 0);
    }

    uint16_t move;
};

namespace ansi {
constexpr const char* RESET = "\033[0m";
constexpr const char* GRAY = "\033[90m";
constexpr const char* RED = "\033[91m";
constexpr const char* YELLOW = "\033[93m";
constexpr const char* WHITE = "\033[97m";
};  // namespace ansi

namespace Zobrist {
constexpr uint64_t splitmix64(uint64_t& state) {
    uint64_t z = (state += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}

constexpr std::array<std::array<uint64_t, GRID_NB>, PIECE_NB>
init_zobrist_table() {
    std::array<std::array<uint64_t, GRID_NB>, PIECE_NB> zobrist_table{};
    uint64_t seed = 12345;

    for (int piece = 0; piece < PIECE_NB; ++piece) {
        for (int r = ROW_0; r <= ROW_9; ++r) {
            for (int c = COL_A; c <= COL_I; ++c) {
                zobrist_table[piece][to_grid(r, c)] = splitmix64(seed);
            }
        }
    }

    return zobrist_table;
}

inline constexpr auto TABLE = init_zobrist_table();
inline constexpr uint64_t PLAYER = [] {
    uint64_t s = 12345;
    return splitmix64(s);
}();
}  // namespace Zobrist

struct HistoryEntry {
    HistoryEntry(const Move& move, Piece captured_piece,
                 uint32_t ply_since_capture, uint64_t zobrist, bool checked)
        : move(move),
          captured_piece(captured_piece),
          ply_since_capture(ply_since_capture),
          zobrist(zobrist),
          checked(checked) {}

    Move move;
    Piece captured_piece;
    uint32_t ply_since_capture;
    uint64_t zobrist;
    bool checked;
};

constexpr const char* INIT_FEN =
    "rheakaehr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RHEAKAEHR w - - 0 1";

enum RepStatus { NO_REP, DRAW, LOSS };

struct Position {
    Position(const std::string& fen = INIT_FEN) { set(fen); }

    Position(const Position&) = delete;
    Position& operator=(const Position&) = delete;

    int ply() { return history.size(); }

    void add_piece(Piece piece, Grid grid) {
        assert(piece != EMPTY);
        assert(board[grid] == EMPTY);

        if (type_of(piece) == KING) {
            king_positions[color_of(piece)] = grid;
        }

        zobrist ^= Zobrist::TABLE[piece][grid];
        board[grid] = piece;

        values[color_of(piece)] += get_piece_value(piece, grid);
    }

    void remove_piece(Grid grid) {
        assert(board[grid] != EMPTY);
        Piece piece = board[grid];

        values[color_of(piece)] -= get_piece_value(piece, grid);

        zobrist ^= Zobrist::TABLE[piece][grid];
        board[grid] = EMPTY;
    }

    void change_player() {
        zobrist ^= Zobrist::PLAYER;
        player = (player == RED) ? BLACK : RED;
    }

    void make_move(const Move& move) {
        assert(board[move.from()] != EMPTY);

        history.emplace_back(move, board[move.to()], ply_since_capture, zobrist,
                             is_in_check(player));

        if (is_capture_move(move)) {
            remove_piece(move.to());
            ply_since_capture = 0;
        } else {
            ++ply_since_capture;
        }
        add_piece(board[move.from()], move.to());
        remove_piece(move.from());

        change_player();
    }

    void undo_move() {
        assert(!history.empty());
        const auto& entry = history.back();

        add_piece(board[entry.move.to()], entry.move.from());
        remove_piece(entry.move.to());
        if (entry.captured_piece != EMPTY) {
            add_piece(entry.captured_piece, entry.move.to());
        }
        ply_since_capture = entry.ply_since_capture;

        change_player();

        history.pop_back();
    }

    std::vector<Move> generate_legal_moves() {
        std::vector<Move> legal_moves;
        for (const Move& move : generate_pseudo_legal_moves()) {
            make_move(move);
            if (!is_in_check(opponent(player)) && repetition_status() == NO_REP) {
                legal_moves.push_back(move);
            }
            undo_move();
        }
        return legal_moves;
    }

    bool is_in_check(Color color) const {
        Grid king_pos = king_positions[color];
        Color attacker = opponent(color);

        // Check for attacks from opponent's pieces
        if (is_attacked_by_knight(king_pos, attacker)) return true;
        if (is_attacked_by_rook_or_cannon(king_pos, attacker)) return true;
        if (is_attacked_by_pawn(king_pos, attacker)) return true;

        // kings cannot meet up
        Grid opponent_king_pos = king_positions[attacker];
        if (col_of(king_pos) != col_of(opponent_king_pos)) return false;
        int forward = (color == RED) ? 16 : -16;
        Grid pos = Grid(king_pos + forward);
        while (IN_BOUNDS[pos]) {
            if (board[pos] == EMPTY) {
                pos = Grid(pos + forward);
                continue;
            }
            if (board[pos] == make_piece(attacker, KING)) return true;
            break;
        }

        return false;
    }

    static constexpr std::array<std::array<int, 2>, 4> KNIGHT_CHECK_MOVES{
        {{-33, -18}, {-31, -14}, {14, 31}, {18, 33}}};

    bool is_attacked_by_knight(Grid grid, Color color) const {
        for (size_t i = 0; i < ADVISOR_MOVES.size(); ++i) {
            Grid pin = Grid(grid + ADVISOR_MOVES[i]);
            if (!IN_BOUNDS[pin]) continue;      // Out of bounds
            if (board[pin] != EMPTY) continue;  // Blocked
            for (int delta : KNIGHT_CHECK_MOVES[i]) {
                Grid to = Grid(grid + delta);
                if (!IN_BOUNDS[to]) continue;  // Out of bounds
                if (board[to] != make_piece(color, KNIGHT))
                    continue;  // Not an attacking knight
                return true;
            }
        }
        return false;
    }

    bool is_attacked_by_rook_or_cannon(Grid grid, Color color) const {
        for (int delta : SLIDING_MOVES) {
            Grid to = Grid(grid + delta);
            while (IN_BOUNDS[to]) {
                if (board[to] == EMPTY) {
                    to = Grid(to + delta);
                    continue;
                }
                if (board[to] == make_piece(color, ROOK))
                    return true;  // Attacked by rook
                to = Grid(to + delta);
                while (IN_BOUNDS[to] && board[to] == EMPTY) {
                    to = Grid(to + delta);
                }
                if (IN_BOUNDS[to] && board[to] == make_piece(color, CANNON))
                    return true;  // Attacked by cannon
                break;
            }
        }
        return false;
    }

    bool is_attacked_by_pawn(Grid grid, Color color) const {
        int forward = (color == RED) ? -16 : 16;
        Grid to = Grid(grid + forward);
        if (IN_BOUNDS[to] && board[to] == make_piece(color, PAWN))
            return true;  // Attacked by forward pawn
        for (int delta : {-1, 1}) {
            to = Grid(grid + delta);
            if (IN_BOUNDS[to] && board[to] == make_piece(color, PAWN))
                return true;  // Attacked by side pawn
        }
        return false;
    }

    RepStatus repetition_status(int n = 3) const {
        if (history.size() < 4) return NO_REP;

        int cnt = 1;
        bool perp_check = true, opp_perp_check = true, self = false;
        for (int i = history.size() - 1; i >= 0; --i) {
            if (history[i].captured_piece != EMPTY)
                return NO_REP;
            if (self) {
                perp_check &= history[i].checked;
                if (history[i].zobrist == zobrist) {
                    cnt++;
                    if (cnt >= n) {
                        if (perp_check && opp_perp_check) {
                            return DRAW;
                        } else if (perp_check) {
                            return LOSS;
                        } else {
                            return NO_REP;
                        }
                    }
                }
            } else {
                opp_perp_check &= history[i].checked;
            }
            self = !self;
        }
        return NO_REP;
    }

    std::vector<Move> generate_pseudo_legal_moves() const {
        std::vector<Move> moves;
        for (int r = ROW_0; r <= ROW_9; ++r) {
            for (int c = COL_A; c <= COL_I; ++c) {
                Grid grid = to_grid(r, c);
                Piece piece = board[grid];
                if (piece == EMPTY || color_of(piece) != player) {
                    continue;
                }
                switch (type_of(piece)) {
                    case KING:
                        generate_king_moves(moves, (Grid)grid);
                        break;
                    case ADVISOR:
                        generate_advisor_moves(moves, (Grid)grid);
                        break;
                    case BISHOP:
                        generate_bishop_moves(moves, (Grid)grid);
                        break;
                    case KNIGHT:
                        generate_knight_moves(moves, (Grid)grid);
                        break;
                    case ROOK:
                        generate_rook_moves(moves, (Grid)grid);
                        break;
                    case CANNON:
                        generate_cannon_moves(moves, (Grid)grid);
                        break;
                    case PAWN:
                        generate_pawn_moves(moves, (Grid)grid);
                        break;
                    default:
                        assert(false && "Invalid piece type");
                }
            }
        }
        return moves;
    }

    static constexpr std::array<int, 4> SLIDING_MOVES = {-16, -1, 1, 16};

    void generate_king_moves(std::vector<Move>& moves, Grid from) const {
        for (int delta : SLIDING_MOVES) {
            Grid to = Grid(from + delta);
            if (!IN_BOUNDS[to]) continue;  // Out of bounds
            if (!PALACE[to]) continue;     // Not in palace
            if (board[to] != EMPTY && color_of(board[to]) == player)
                continue;  // Own piece
            moves.emplace_back(from, to);
        }
    }

    static constexpr std::array<int, 4> ADVISOR_MOVES = {-17, -15, 15, 17};

    void generate_advisor_moves(std::vector<Move>& moves, Grid from) const {
        for (int delta : ADVISOR_MOVES) {
            Grid to = Grid(from + delta);
            if (!IN_BOUNDS[to]) continue;  // Out of bounds
            if (!PALACE[to]) continue;     // Not in palace
            if (board[to] != EMPTY && color_of(board[to]) == player)
                continue;  // Own piece
            moves.emplace_back(from, to);
        }
    }

    static constexpr std::array<int, 4> BISHOP_MOVES = {-34, -30, 30, 34};

    void generate_bishop_moves(std::vector<Move>& moves, Grid from) const {
        for (size_t i = 0; i < ADVISOR_MOVES.size(); ++i) {
            Grid pin = Grid(from + ADVISOR_MOVES[i]);
            if (!IN_BOUNDS[pin]) continue;      // Out of bounds
            if (board[pin] != EMPTY) continue;  // Blocked
            Grid to = Grid(from + BISHOP_MOVES[i]);
            if (!IN_BOUNDS[to]) continue;     // Out of bounds
            if (!SIDE[player][to]) continue;  // Not on own side
            if (board[to] != EMPTY && color_of(board[to]) == player)
                continue;  // Own piece
            moves.emplace_back(from, to);
        }
    }

    static constexpr std::array<int, 4> KNIGHT_PINS = {-16, -1, 1, 16};
    static constexpr std::array<std::array<int, 2>, 4> KNIGHT_MOVES = {
        {{-33, -31}, {-18, 14}, {-14, 18}, {31, 33}}};

    void generate_knight_moves(std::vector<Move>& moves, Grid from) const {
        for (size_t i = 0; i < KNIGHT_PINS.size(); ++i) {
            Grid pin = Grid(from + KNIGHT_PINS[i]);
            if (!IN_BOUNDS[pin]) continue;      // Out of bounds
            if (board[pin] != EMPTY) continue;  // Blocked
            for (int delta : KNIGHT_MOVES[i]) {
                Grid to = Grid(from + delta);
                if (!IN_BOUNDS[to]) continue;  // Out of bounds
                if (board[to] != EMPTY && color_of(board[to]) == player)
                    continue;  // Own piece
                moves.emplace_back(from, to);
            }
        }
    }

    void generate_rook_moves(std::vector<Move>& moves, Grid from) const {
        for (int delta : SLIDING_MOVES) {
            Grid to = Grid(from + delta);
            while (IN_BOUNDS[to]) {
                if (board[to] == EMPTY) {
                    moves.emplace_back(from, to);
                    to = Grid(to + delta);
                    continue;
                }
                if (color_of(board[to]) != player) {
                    moves.emplace_back(from, to);
                }
                break;  // Blocked
            }
        }
    }

    void generate_cannon_moves(std::vector<Move>& moves, Grid from) const {
        for (int delta : SLIDING_MOVES) {
            Grid to = Grid(from + delta);
            while (IN_BOUNDS[to] && board[to] == EMPTY) {
                moves.emplace_back(from, to);
                to = Grid(to + delta);
            }
            if (!IN_BOUNDS[to]) continue;  // Out of bounds
            to = Grid(to + delta);
            while (IN_BOUNDS[to] && board[to] == EMPTY) {
                to = Grid(to + delta);
            }
            if (IN_BOUNDS[to] && color_of(board[to]) != player) {
                moves.emplace_back(from, to);
            }
        }
    }

    void generate_pawn_moves(std::vector<Move>& moves, Grid from) const {
        int forward = (player == RED) ? 16 : -16;
        Grid to = Grid(from + forward);
        if (IN_BOUNDS[to] &&
            (board[to] == EMPTY || color_of(board[to]) != player)) {
            moves.emplace_back(from, to);
        }
        if (SIDE[player][from]) return;  // Not crossed river
        for (int delta :
             {-1, 1}) {  // Left and right moves after crossing river
            to = Grid(from + delta);
            if (!IN_BOUNDS[to]) continue;  // Out of bounds
            if (board[to] == EMPTY || color_of(board[to]) != player) {
                moves.emplace_back(from, to);
            }
        }
    }

    bool can_do_null_move() const {
        if (is_in_check(player)) return false;  // Can't do null move in check
        return values[player] >= 600;
    }

    void null_move() {
        history.emplace_back(Move(0), EMPTY, ply_since_capture, zobrist, is_in_check(player));
        ply_since_capture++;
        change_player();
    }

    void undo_null_move() {
        assert(!history.empty());
        const auto& entry = history.back();
        assert(entry.move.move == 0);  // Ensure it's a null move

        ply_since_capture = entry.ply_since_capture;
        change_player();

        history.pop_back();
    }

    friend std::ostream& operator<<(std::ostream& os, const Position& state) {
        for (int r = ROW_9; r >= ROW_0; --r) {
            for (int c = COL_A; c <= COL_I; ++c) {
                if (state.board[to_grid(r, c)] == EMPTY) {
                    if (PALACE[to_grid(r, c)]) {
                        os << ansi::YELLOW << ". " << ansi::RESET;
                    } else if (RIVER[to_grid(r, c)]) {
                        os << ansi::GRAY << "- " << ansi::RESET;
                    } else {
                        os << ansi::GRAY << ". " << ansi::RESET;
                    }
                } else if (state.board[to_grid(r, c)] <= RED_PAWN) {
                    os << ansi::RED << PIECE_CHAR[state.board[to_grid(r, c)]]
                       << " " << ansi::RESET;
                } else {
                    os << ansi::WHITE << PIECE_CHAR[state.board[to_grid(r, c)]]
                       << " " << ansi::RESET;
                }
            }
            os << "\n";
        }

        return os;
    }

    void set(const std::string fen) {
        board.fill(EMPTY);
        std::stringstream ss(fen);
        char ch;
        int r = ROW_9, c = COL_A;
        while (ss >> ch) {
            if (isalpha(ch)) {
                Color color = isupper(ch) ? RED : BLACK;
                ch = tolower(ch);
                PieceType type{};
                switch (ch) {
                    case 'k':
                        type = KING;
                        break;
                    case 'a':
                        type = ADVISOR;
                        break;
                    case 'b':
                    case 'e':
                        type = BISHOP;
                        break;
                    case 'n':
                    case 'h':
                        type = KNIGHT;
                        break;
                    case 'r':
                        type = ROOK;
                        break;
                    case 'c':
                        type = CANNON;
                        break;
                    case 'p':
                        type = PAWN;
                        break;
                    default:
                        break;
                }
                add_piece(make_piece(color, type), to_grid(r, c));
                c++;
            } else if (isdigit(ch)) {
                c += ch - '0';
            } else if (ch == '/') {
                r--;
                c = 0;
            } else {
                break;
            }
        }
        if (ch == 'w') {
            player = RED;
        } else if (ch == 'b') {
            player = BLACK;
        }
    }

    int evaluate() const { return values[player] - values[opponent(player)]; }

    int heuristic_score(const Move& move) {
        int score = 0;

        if (is_capture_move(move)) {
            score = 10 * PIECE_VALUES[type_of(board[move.to()])] -
                    PIECE_VALUES[type_of(board[move.from()])];
        }

        return score;
    }

    bool is_capture_move(const Move& move) { return board[move.to()] != EMPTY; }

    std::array<Piece, GRID_NB> board{};
    Color player = RED;
    uint32_t ply_since_capture = 0;
    uint64_t zobrist = 0;

    std::vector<HistoryEntry> history;
    std::array<Grid, COLOR_NB> king_positions;
    std::array<int, COLOR_NB> values{};
};

int perft(Position &pos, int depth) {
    if (depth == 0) {
        return 1;
    }

    int count = 0;
    for (const auto& move : pos.generate_pseudo_legal_moves()) {
        pos.make_move(move);
        if (pos.is_in_check(opponent(pos.player)) ||
            pos.repetition_status() != NO_REP) {
            pos.undo_move();
            continue;
        }
        count += perft(pos, depth - 1);
        pos.undo_move();
    }

    return count;
}

int main() {}
