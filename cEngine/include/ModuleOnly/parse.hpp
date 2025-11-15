//
// Created by Kiril on 2025-11-15.
//

#pragma once
#include <sstream>
#include <string>

#include "State.hpp"

// inline moveFromLongAlgebric(std::string mv, const State state) {
//
// }

inline BoardPosition squareToPos(const std::string& square) {
    const int file = square[0] - 'a';
    const int rank = square[1] - '0';

    return {file, 8 - rank}; // a8 is 0,0
}

inline State fenToState(const std::string& fen) {
    std::istringstream iss(fen);
    const std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    if (tokens.size() != 6) {
        throw std::invalid_argument("Invalid FEN: must contain 6 fields");
    }

    const std::string& board_rep = tokens[0];
    std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> board{};
    int x = 0;
    int y = 0; // 0,0 is top left of board
    for (const char ch : board_rep) {
        if (x > BOARD_SIZE || y >= BOARD_SIZE) {
            throw std::invalid_argument("Invalid FEN: must contain board representation within BOARD_SIZE");
        }
        if (ch == '/') {
            if (x != 8) {
                throw std::invalid_argument("Invalid FEN: rank has incorrect number of squares");
            }
            y++;
            x = 0;
        } else if (isalpha(ch)) {
            board[y][x] = Pieces::from_char(ch);
            x++;
        } else {
            int skips = ch - '0';
            if (skips < 1 || skips > 8) {
                throw std::invalid_argument("Invalid FEN: skip digit must be 1–8");
            }

            for (int i = 0; i < skips; ++i) {
                board[y][x] = Pieces::EMPTY;
                x++;
            }
        }
    }

    Color activeColor;
    if (tokens[1] == "w") {
        activeColor = Color::White;
    } else if (tokens[1] == "b") {
        activeColor = Color::Black;
    } else {
        throw std::invalid_argument("Invalid FEN: active color must be 'w' or 'b'");
    }

    int castleRights = 0;
    if (tokens[2] != "-") {
        for (const char c : tokens[2]) {
            char lowered = tolower(c);
            Color color = lowered == c ? Color::Black : Color::White;
            switch (lowered) {
            case 'k':
                allowCastle(color, CastleType::SHORT, castleRights);
                break;
            case 'q':
                allowCastle(color, CastleType::LONG, castleRights);
                break;
            default:
                throw std::invalid_argument("Invalid castle type");
            }
        }
    }

    std::optional<BoardPosition> enPassantSquare = std::nullopt;
    if (tokens[3] != "-") {
        enPassantSquare = squareToPos(tokens[3]);
    }

    int halfMoveClock = std::stoi(tokens[4]);
    int fullMoveClock = std::stoi(tokens[5]);

    return {NewBoard(board), activeColor, castleRights, enPassantSquare, halfMoveClock, fullMoveClock};
}
