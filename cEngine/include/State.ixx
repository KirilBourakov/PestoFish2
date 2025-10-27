//
// Created by Kiril on 2025-08-31.
//

export module State;
import Enums;
import Move;
import Board;
import ZobristHash;
import <optional>;
import <vector>;


export class State {
public:
    struct HistoricalEntry {
        Move move;
        Pieces::Piece movedPiece; // piece moved by move
        Pieces::Piece overwrittenPiece; // piece replaced by movedPiece

        int castlingBeforeMove;
        int halfMoveClockBeforeMove;
        std::optional<BoardPosition> enPassantBeforeMove;

        bool operator==(const HistoricalEntry& other) const {
            return other.move == move && other.movedPiece == movedPiece && other.overwrittenPiece == overwrittenPiece
                && castlingBeforeMove == other.castlingBeforeMove && halfMoveClockBeforeMove == other.halfMoveClockBeforeMove && enPassantBeforeMove == other.enPassantBeforeMove;
        }
    };

    State();
    State(const BoardArray &board, Color activeColor, int castlingRights, std::optional<BoardPosition> enPassantSquare);
    State(const BoardArray &board, Color activeColor, int castlingRights, std::optional<BoardPosition> enPassantSquare,
    int halfMoveClock, int fullMoveClock, BoardPosition whiteKingSquare, BoardPosition blackKingSquare, const std::vector<HistoricalEntry> &history);

    void addMoves(int x, int y, std::vector<Move>& out) const;
    void makeMove(const Move &move);
    void undoMove();
    std::vector<Move> getMoves();
    bool isLegalMove(BoardPosition start, BoardPosition end);
    void translateAndMove(BoardPosition start, BoardPosition end, std::optional<Pieces::Piece> promotedTo);
    GameState getGameState();

    [[nodiscard]] State makeThreadCopy();
    [[nodiscard]] bool samePosition(const State& other) const;
    [[nodiscard]] int getCastlingRights() const {return castlingRights;}
    [[nodiscard]] const BoardArray& getBoard() const {return board;}
    [[nodiscard]] std::optional<BoardPosition> getEnPassantSquare() const {return enPassantSquare;}
    [[nodiscard]] int getHalfMoveClock() const {return halfMoveClock;}
    [[nodiscard]] Color getActiveColor() const {return activeColor;}
    [[nodiscard]] bool isHalfMoveTie() const {return halfMoveClock >= 50;}
    [[nodiscard]] bool colorInCheck(Color color) const {
        BoardPosition kingSquare = color == Color::White ? whiteKingSquare : blackKingSquare;
        return isAttacked(board, kingSquare);
    }

    friend bool operator==(const State& lhs, const State& rhs);

private:
    std::vector<Move> purgeIllegal(const std::vector<Move>& pseudolegalMoves);

    // FEN info
    BoardArray board;
    Color activeColor;
    int castlingRights;
    std::optional<BoardPosition> enPassantSquare;
    int halfMoveClock;
    int fullMoveClock;

    // Other useful info
    BoardPosition whiteKingSquare;
    BoardPosition blackKingSquare;

    std::vector<HistoricalEntry> history;

    ZobristHash hash;
};
