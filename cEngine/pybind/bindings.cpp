//
// Created by Kiril on 2025-08-23.
//
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
import Engine;
import Enums;
import State;
import Board;

namespace py = pybind11;

PYBIND11_MODULE(cEngine, m) {
    // CLASS
    py::class_<Engine>(m, "Engine")
        .def(py::init<>())
        .def("getState", &Engine::getState, py::return_value_policy::reference_internal)
        .def("make_engine_move", &Engine::makeEngineMove);

    py::class_<State>(m, "State")
        .def("getBoard", &State::getBoard, py::return_value_policy::reference_internal)
        .def("get_active_color", &State::getActiveColor)
        .def("is_legal_move", &State::isLegalMove)
        .def("translate_and_move", &State::translateAndMove)
        .def("get_half_move_clock", &State::getHalfMoveClock)
        .def("get_game_state", py::overload_cast<>(&State::getGameState));

    py::class_<BoardArray>(m, "BoardArray")
        .def("__getitem__", [](const BoardArray &self, size_t i) {
            if (i >= BOARD_SIZE){ throw py::index_error();}
            const auto &row = self[i];
            return std::vector<Pieces::Piece>(row.begin(), row.end());
        });

    py::class_<BoardPosition>(m, "BoardPosition")
        .def(py::init<int,int>(), py::arg("x"), py::arg("y"))
        .def_readwrite("x", &BoardPosition::x)
        .def_readwrite("y", &BoardPosition::y);

    // FUNCTION
    m.def("same_color", &Pieces::sameColor);

    // ENUM
    py::module Piece = m.def_submodule("Piece", "Pieces");
     Piece.attr("EMPTY") = Pieces::make_piece(Color::White, PieceType::None);
     Piece.attr("WHITE_PAWN") = Pieces::make_piece(Color::White, PieceType::Pawn);
     Piece.attr("WHITE_KNIGHT") = Pieces::make_piece(Color::White, PieceType::Knight);
     Piece.attr("WHITE_BISHOP") = Pieces::make_piece(Color::White, PieceType::Bishop);
     Piece.attr("WHITE_ROOK") = Pieces::make_piece(Color::White, PieceType::Rook);
     Piece.attr("WHITE_QUEEN") = Pieces::make_piece(Color::White, PieceType::Queen);
     Piece.attr("WHITE_KING") = Pieces::make_piece(Color::White, PieceType::King);

     Piece.attr("BLACK_PAWN") = Pieces::make_piece(Color::Black, PieceType::Pawn);
     Piece.attr("BLACK_KNIGHT") = Pieces::make_piece(Color::Black, PieceType::Knight);
     Piece.attr("BLACK_BISHOP") = Pieces::make_piece(Color::Black, PieceType::Bishop);
     Piece.attr("BLACK_ROOK") = Pieces::make_piece(Color::Black, PieceType::Rook);
     Piece.attr("BLACK_QUEEN") = Pieces::make_piece(Color::Black, PieceType::Queen);
     Piece.attr("BLACK_KING") = Pieces::make_piece(Color::Black, PieceType::King);

    py::enum_<Color>(m, "Color")
        .value("WHITE", Color::White)
        .value("BLACK", Color::Black);

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