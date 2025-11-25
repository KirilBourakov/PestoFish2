//
// Created by Kiril on 2025-08-23.
//
#include <stdexcept>

#include "ModuleOnly/CastleUtils.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"
#include "NewBoard.hpp"

#include <optional>
#include <vector>

using moveSet = std::vector<std::pair<int, int>>;

void NewBoard::addMoves(int x, int y, const Color activeColor, const std::optional<BoardPosition> enPassantSquare, int castlingRights,
                        std::vector<Move>& out) const {
    switch (Pieces::piece_type(board[y][x])) {
    case PieceType::Pawn:
    case PieceType::King:
    case PieceType::Knight:
        break;
    case PieceType::Bishop:
        addSlidingMoves(x, y, activeColor, false, true, out);
        break;
    case PieceType::Rook:
        addSlidingMoves(x, y, activeColor, true, false, out);
        break;
    case PieceType::Queen:
        addSlidingMoves(x, y, activeColor, true, true, out);
        break;
    default:
        throw std::invalid_argument(std::format("Invalid piece {} at ({}, {})", static_cast<int>(board[y][x]), x, y));
    }
}

void NewBoard::addKingMoves(const int x, const int y, const Color color, const int castleRights, std::vector<Move>& moves) const {
    using namespace Pieces;

    static const moveSet straight_diag = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    const BoardPosition start{.x = x, .y = y};
    for (auto [dx, dy] : straight_diag) {
        const int newX = x + dx;
        const int newY = y + dy;
        if (inBounds(newX, newY) && !sameColor(color, board[newY][newX])) {
            moves.push_back(Move::standardMove(start, {newX, newY}));
        }
    }

    const int newY = color == Color::Black ? 0 : 7;
    if (castleAllowed(color, CastleType::SHORT, castleRights)) {
        constexpr int newX = 6;
        if (board[newY][newX - 1] == EMPTY && board[newY][newX] == EMPTY) {
            moves.push_back(Move::castleMove(start, {newX, newY}, CastleType::SHORT));
        }
    }
    if (castleAllowed(color, CastleType::LONG, castleRights)) {
        constexpr int newX = 2;
        if (board[newY][newX - 1] == EMPTY && board[newY][newX] == EMPTY && board[newY][newX + 1] == EMPTY) {
            moves.push_back(Move::castleMove(start, {newX, newY}, CastleType::LONG));
        }
    }
}

// TODO: test
void NewBoard::addPromotions(BoardPosition start, BoardPosition end, Pieces::Piece piece, Color color, std::vector<Move>& moves) {
    using namespace Pieces;

    static constexpr std::array<Piece, 4> whitePieces = {WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN};
    static constexpr std::array<Piece, 4> blackPieces = {BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN};
    const auto& usedPieces = (color == Color::White ? whitePieces : blackPieces);

    for (const Piece promoteTo : usedPieces) {
        moves.push_back(Move::promotionMove(start, end, promoteTo));
    }
}

void NewBoard::addPawnMoves(const int x, const int y, const Color color, const std::optional<BoardPosition>& enPassantSquare,
                            std::vector<Move>& moves) const {
    int dir = color == Color::White ? -1 : 1;
    int newY = y + dir;
    const BoardPosition start{.x = x, .y = y};
    // standard move
    if (inBounds(x, newY) && board[newY][x] == Pieces::EMPTY) {
        if (newY == 7 || newY == 0) {
            addPromotions(start, {x, newY}, board[newY][x], color, moves);
        } else {
            moves.push_back(Move::standardMove(start, {x, newY}));
        }
    }

    // attacks
    moveSet attacks = {{+1, dir}, {-1, dir}};
    for (auto [dx, dy] : attacks) {
        int newX = x + dx;
        newY = y + dy;
        if (inBounds(newX, newY)) {
            if (board[newY][newX] != Pieces::EMPTY && !Pieces::sameColor(color, board[newY][newX])) {
                if (newY == 7 || newY == 0) {
                    addPromotions(start, {newX, newY}, board[newY][x], color, moves);
                } else {
                    moves.push_back(Move::standardMove(start, {newX, newY}));
                }
            }
        }
    }

    // en passent
    if (enPassantSquare.has_value()) {
        const bool correctX = std::abs(enPassantSquare->x - x) == 1;
        const bool correctY = enPassantSquare->y == y + dir;
        if (correctX && correctY) {
            moves.push_back(Move::enPassantCaptureMove(start, {enPassantSquare->x, enPassantSquare->y}));
        }
    }

    // double move
    int startY = color == Color::White ? 6 : 1;
    if (y == startY) {
        newY = y + 2 * dir;
        if (inBounds(x, newY)) {
            const int enPassantSquareY = newY - dir;
            if (board[newY][x] == Pieces::EMPTY && board[enPassantSquareY][x] == Pieces::EMPTY) {
                moves.push_back(Move::doublePawnMove(start, {x, newY}, {x, enPassantSquareY}));
            }
        }
    }
}

void NewBoard::addKnightMoves(const int x, const int y, const Color color, std::vector<Move>& moves) const {
    static const moveSet possible = {{+2, -1}, {+2, +1}, {-2, -1}, {-2, +1}, {-1, +2}, {+1, +2}, {-1, -2}, {+1, -2}};
    const BoardPosition start{.x = x, .y = y};
    for (auto [off_x, off_y] : possible) {
        const int newX = x + off_x;
        const int newY = y + off_y;
        if (inBounds(newX, newY)) {
            if (!Pieces::sameColor(color, board[newY][newX])) {
                moves.push_back(Move::standardMove(start, {newX, newY}));
            }
        }
    }
}

void NewBoard::addSlidingMoves(int x, int y, const Color color, const bool straight, const bool diag, std::vector<Move>& moves) const {
    static const moveSet straight_diag = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    static const moveSet diag_dir = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    static const moveSet straight_dir = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    const BoardPosition start{.x = x, .y = y};
    const moveSet* dirs;
    if (straight && diag) {
        dirs = &straight_diag;
    } else if (diag) {
        dirs = &diag_dir;
    } else if (straight) {
        dirs = &straight_dir;
    } else {
        throw std::invalid_argument("Invalid direction");
    }

    for (auto [dx, dy] : *dirs) {
        for (int i = 1; i < BOARD_SIZE; i++) {
            const int newY = y + i * dy;
            const int newX = x + i * dx;
            if (!inBounds(newX, newY)) {
                break;
            }

            if (Pieces::sameColor(color, board[newY][newX])) {
                break;
            }

            moves.push_back(Move::standardMove(start, {newX, newY}));
            if (board[newY][newX] != Pieces::EMPTY) {
                break;
            }
        }
    }
}

bool NewBoard::isAttacked(const BoardPosition& position, const Color color) const {
    if (!inBounds(position.x, position.y)) {
        throw std::invalid_argument("Position is out of bounds");
    }

    static const moveSet straight_dir = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    for (auto [dx, dy] : straight_dir) {
        for (int i = 1; i < BOARD_SIZE; i++) {
            const int newY = position.y + i * dy;
            const int newX = position.x + i * dx;
            if (!inBounds(newX, newY) || Pieces::sameColor(color, board[newY][newX])) {
                break;
            }
            if (Pieces::piece_type(board[newY][newX]) == PieceType::Rook ||
                Pieces::piece_type(board[newY][newX]) == PieceType::Queen) {
                return true;
            }
            if (board[newY][newX] != Pieces::EMPTY) {
                break;
            }
        }
    }

    static const moveSet diag_dir = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (auto [dx, dy] : diag_dir) {
        for (int i = 1; i < BOARD_SIZE; i++) {
            const int newY = position.y + i * dy;
            const int newX = position.x + i * dx;
            if (!inBounds(newX, newY) || Pieces::sameColor(color, board[newY][newX])) {
                break;
            }
            if (Pieces::piece_type(board[newY][newX]) == PieceType::Bishop ||
                Pieces::piece_type(board[newY][newX]) == PieceType::Queen) {
                return true;
            }
            if (board[newY][newX] != Pieces::EMPTY) {
                break;
            }
        }
    }

    static const moveSet knightHops = {{+2, -1}, {+2, +1}, {-2, -1}, {-2, +1}, {-1, +2}, {+1, +2}, {-1, -2}, {+1, -2}};
    for (auto [dx, dy] : knightHops) {
        const int newY = position.y + dy;
        const int newX = position.x + dx;
        if (inBounds(newX, newY) && !Pieces::sameColor(color, board[newY][newX]) &&
            Pieces::piece_type(board[newY][newX]) == PieceType::Knight) {
            return true;
        }
    }
    // check pawn
    int dir = color == Color::White ? -1 : 1;
    moveSet pawnAttacks = {{+1, dir}, {-1, dir}};
    for (auto [dx, dy] : pawnAttacks) {
        const int newY = position.y + dy;
        const int newX = position.x + dx;
        if (inBounds(newX, newY) && !Pieces::sameColor(color, board[newY][newX]) &&
            Pieces::piece_type(board[newY][newX]) == PieceType::Pawn) {
            return true;
        }
    }

    // KINGS
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0)
                continue;
            int newX = position.x + dx;
            int newY = position.y + dy;
            if (inBounds(newX, newY) && !Pieces::sameColor(color, board[newY][newX]) &&
                Pieces::piece_type(board[newY][newX]) == PieceType::King) {
                return true;
            }
        }
    }

    return false;
}

bool NewBoard::isAttacked(const BoardPosition& position) const {
    if (!inBounds(position.x, position.y)) {
        throw std::invalid_argument("Position is out of bounds");
    }
    if (board[position.y][position.x] == Pieces::EMPTY) {
        throw std::invalid_argument("Cannot implicitly find color of empty square");
    }
    const Color color = Pieces::piece_color(board[position.y][position.x]);
    return isAttacked(position, color);
}

bool NewBoard::isDrawFromMaterial() const {
    int wBishops = 0;
    int wKnights = 0;
    int bBishops = 0;
    int bKnights = 0;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            switch (board[y][x]) {
                case Pieces::WHITE_PAWN:
                case Pieces::BLACK_PAWN:
                case Pieces::WHITE_ROOK:
                case Pieces::BLACK_ROOK:
                case Pieces::WHITE_QUEEN:
                case Pieces::BLACK_QUEEN:
                    return false;

                case Pieces::WHITE_KNIGHT:
                    wKnights++;
                    break;
                case Pieces::WHITE_BISHOP:
                    wBishops++;
                    break;
                case Pieces::BLACK_KNIGHT:
                    bKnights++;
                    break;
                case Pieces::BLACK_BISHOP:
                    bBishops++;
                    break;

                case Pieces::WHITE_KING:
                case Pieces::BLACK_KING:
                case Pieces::EMPTY:
                    break;
            }
        }
    }
    const bool whiteCanWin = (wBishops >= 1 && wKnights >= 1) || (wBishops >= 2) || (wKnights >= 3);
    const bool blackCanWin = (bBishops >= 1 && bKnights >= 1) || (bBishops >= 2) || (bKnights >= 3);
    return !whiteCanWin && !blackCanWin;
}

std::ostream& operator<<(std::ostream& os, const NewBoard& b) {
    os << "  +-----------------+\n";
    for (int i = 0; i < BOARD_SIZE; i++) {
        os << i + 1 << " | ";
        for (int j = 0; j < BOARD_SIZE; j++) {
            os << Pieces::piece_char(b.board[i][j]) << ' ';
        }
        os << "|\n";
    }
    os << "  +-----------------+\n";
    return os;
}