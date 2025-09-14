//
// Created by Kiril on 2025-08-23.
//
#include <pybind11/pybind11.h>
import Engine;
import Types;
namespace py = pybind11;

Move makeMove(
    const BoardPosition start,
    const BoardPosition end,
    const bool enPassantCapture,
    const std::optional<Piece> promotedPiece,
    const std::optional<CastleType> castleType,
    const std::optional<BoardPosition> newEnPassantSquare
) {
    if (promotedPiece) {
        return Move::promotionMove(start, end, promotedPiece.value());
    }
    if (castleType) {
        return Move::castleMove(start, end, castleType.value());
    }
    if (newEnPassantSquare) {
        return Move::doublePawnMove(start, end, newEnPassantSquare.value());
    }
    if (enPassantCapture) {
        return Move::enPassantCaptureMove(start, end);
    }
    return Move::standardMove(start, end);
}

PYBIND11_MODULE(cEngine, m) {
    py::class_<Engine>(m, "Engine")
        .def(py::init<>())
    .def("getBestMove", &Engine::getBestMove);

    py::class_<BoardPosition>(m, "BoardPosition")
        .def(py::init<int,int>(), py::arg("x"), py::arg("y"))
        .def_readwrite("x", &BoardPosition::x)
        .def_readwrite("y", &BoardPosition::y);

    py::class_<Move>(m, "Move")
        .def_readwrite("start", &Move::start)
        .def_readwrite("end", &Move::end)
        .def_readwrite("promotedTo", &Move::promotedTo);

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

    py::enum_<CastleType>(m, "CastleType")
        .value("LONG", CastleType::LONG)
        .value("SHORT", CastleType::SHORT);

    m.def("make_move", &makeMove);
}