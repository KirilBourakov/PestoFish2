//
// Created by Kiril on 2025-08-27.
//
export module Enum;

import <array>;
import <cstdint>;
import <optional>;
import <string>;

export enum Piece {
    EMPTY = 0,

    WHITE_PAWN = 1,
    WHITE_KNIGHT = 2,
    WHITE_BISHOP = 3,
    WHITE_ROOK = 4,
    WHITE_QUEEN = 5,
    WHITE_KING = 6,

    BLACK_PAWN = -1,
    BLACK_KNIGHT = -2,
    BLACK_BISHOP = -3,
    BLACK_ROOK = -4,
    BLACK_QUEEN = -5,
    BLACK_KING = -6
};

export enum Color {
    BLACK = -1,
    WHITE = 1,
};

export enum CastleType {
    SHORT = 0,
    LONG = 1
};

export enum GameState {
    IN_PLAY = 0,
    WHITE_WIN = 1,
    BLACK_WIN = -1,
    STALEMATE = 2,
    DRAW = 3
};
