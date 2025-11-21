import logging

import chess
import chess.engine
import pytest

ENGINE = r"D:\Core\Desktop\Projects\personal\Pestofish\cEngine\build\Debug\uci.exe"


def test_uci_initializes():
    """Test pinging the engine"""
    engine = chess.engine.SimpleEngine.popen_uci(ENGINE)
    engine.ping()
    engine.quit()


def test_bestmove_from_startpos():
    """Test getting best move from startpos."""
    engine = chess.engine.SimpleEngine.popen_uci(ENGINE)
    board = chess.Board()
    result = engine.play(board, limit=chess.engine.Limit(time=1))
    assert result.move in board.legal_moves
    engine.quit()


def test_position_after_moves():
    """Test getting position after some moves."""
    logging.basicConfig(level=logging.DEBUG)
    engine = chess.engine.SimpleEngine.popen_uci(ENGINE, debug=True)

    board = chess.Board()
    moves = ["e2e4", "e7e5", "g1f3"]
    for m in moves:
        board.push_uci(m)

    result = engine.play(board, limit=chess.engine.Limit(time=1))
    print("Best move:", result.move)

    engine.quit()


def test_promotion():
    """Test getting promotion move."""
    engine = chess.engine.SimpleEngine.popen_uci(ENGINE)
    board = chess.Board("2k3r1/P7/8/8/8/8/8/7K w - - 0 1")

    mv = engine.play(board, limit=chess.engine.Limit(time=1)).move
    assert mv.promotion in [chess.QUEEN, chess.ROOK, chess.BISHOP, chess.KNIGHT]

    engine.quit()
