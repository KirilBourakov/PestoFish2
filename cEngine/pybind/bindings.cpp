//
// Created by Kiril on 2025-08-23.
//
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
import Engine;
import Types;
import State;
import Board;

namespace py = pybind11;

PYBIND11_MODULE(cEngine, m) {
    // CLASS
    py::class_<Engine>(m, "Engine")
        .def(py::init<>())
        .def("getState", &Engine::getState, py::return_value_policy::reference_internal);

    py::class_<State>(m, "State")
        .def("getBoard", &State::getBoard, py::return_value_policy::reference_internal)
        .def("get_active_color", &State::getActiveColor)
        .def("is_legal_move", &State::isLegalMove)
        .def("translate_and_move", &State::translateAndMove)
        .def("get_half_move_clock", &State::getHalfMoveClock)
        .def("get_game_state", &State::getGameState);

    py::class_<BoardArray>(m, "BoardArray")
        .def("__getitem__", [](const BoardArray &self, size_t i) {
            if (i >= BOARD_SIZE){ throw py::index_error();}
            const auto &row = self[i];
            return std::vector<Piece>(row.begin(), row.end());
        });

    py::class_<BoardPosition>(m, "BoardPosition")
        .def(py::init<int,int>(), py::arg("x"), py::arg("y"))
        .def_readwrite("x", &BoardPosition::x)
        .def_readwrite("y", &BoardPosition::y);

    // FUNCTION
    m.def("same_color", &sameColor);

    // ENUM
    py::enum_<Piece>(m, "Piece")
        .value("WHITE_PAWN", Piece::WHITE_PAWN)
        .value("WHITE_BISHOP", Piece::WHITE_BISHOP)
        .value("WHITE_KNIGHT", Piece::WHITE_KNIGHT)
        .value("WHITE_ROOK", Piece::WHITE_ROOK)
        .value("WHITE_QUEEN", Piece::WHITE_QUEEN)
        .value("WHITE_KING", Piece::WHITE_KING)

        .value("BLACK_PAWN", Piece::BLACK_PAWN)
        .value("BLACK_BISHOP", Piece::BLACK_BISHOP)
        .value("BLACK_KNIGHT", Piece::BLACK_KNIGHT)
        .value("BLACK_ROOK", Piece::BLACK_ROOK)
        .value("BLACK_QUEEN", Piece::BLACK_QUEEN)
        .value("BLACK_KING", Piece::BLACK_KING)
        .value("EMPTY", Piece::EMPTY);

    py::enum_<Color>(m, "Color")
        .value("WHITE", Color::WHITE)
        .value("BLACK", Color::BLACK);

    py::enum_<CastleType>(m, "CastleType")
        .value("LONG", CastleType::LONG)
        .value("SHORT", CastleType::SHORT);

    py::enum_<GameState>(m, "GameState")
        .value("IN_PLAY", GameState::IN_PLAY)
        .value("WHITE_WIN", GameState::WHITE_WIN)
        .value("BLACK_WIN", GameState::BLACK_WIN)
        .value("STALEMATE", GameState::STALEMATE)
        .value("DRAW", GameState::DRAW);
}