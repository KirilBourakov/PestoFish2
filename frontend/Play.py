from typing import Callable

import chess
from PyQt6.QtCore import QByteArray
from PyQt6.QtSvgWidgets import QSvgWidget
from PyQt6.QtWidgets import QWidget, QVBoxLayout, QLabel, QDialog, QPushButton
from chess.svg import SQUARE_SIZE, board

from GameType import GameType

class Play(QWidget):
    state: GameType
    size: int = 480
    board_frame_size: int = 5

    def __init__(self, to_start: Callable):
        super().__init__()

        self.to_start = to_start

        self.board = chess.Board()
        self.selected_square = None

        self.svg_widget = QSvgWidget()
        self.svg_widget.setFixedSize(self.size, self.size)

        self.load_board_svg()

        layout = QVBoxLayout()
        layout.addWidget(self.svg_widget)
        self.setLayout(layout)

    def set_state(self, state: GameType):
        self.state = state

    def load_board_svg(self, **kwargs):
        svg_text = chess.svg.board(self.board, size=self.size, **kwargs)
        svg_bytes = svg_text.encode("utf-8")
        self.svg_widget.load(QByteArray(svg_bytes))

    def mousePressEvent(self, event):
        sq_str = self.square_clicked(event.position().x(), event.position().y())
        square = chess.parse_square(sq_str)

        if self.selected_square is not None:
            self.handle_move(square)

        else:
            if self.state.is_playing(self.board.color_at(square)) and self.board.turn == self.board.color_at(square):
                self.selected_square = square
                moves = [move.to_square for move in self.board.legal_moves if move.from_square == square]
                self.load_board_svg(
                    fill={sq: "#cc0000cc" for sq in moves}
                )
            else:
                self.load_board_svg()
                self.selected_square = None

    def handle_move(self, square: int):
        move = chess.Move(from_square=self.selected_square, to_square=square)

        if self.is_promotion(move):
            dialog = PromotionDialog()
            if dialog.exec() == QDialog.DialogCode.Accepted:
                move.promotion = dialog.selected_piece_type

        if move in self.board.legal_moves:
            self.board.push(move)
            self.selected_square = None

            if self.board.is_game_over(claim_draw=True):
                dialog = GameOverDialog(self.board.outcome(claim_draw=True))
                dialog.exec()
                self.to_start()

            self.load_board_svg()

        else:
            self.load_board_svg()
            self.selected_square = None

    def is_promotion(self, move: chess.Move) -> bool:
        piece = self.board.piece_at(move.from_square)
        rank = chess.square_rank(move.to_square)
        return piece.piece_type == chess.PAWN and \
            ((piece.color == chess.WHITE and rank == 7) or (piece.color == chess.BLACK and rank == 0))


    def square_clicked(self, x: float, y: float) -> str:
        file = round((x-self.board_frame_size) // self.square_size)
        rank = round((y-self.board_frame_size) // self.square_size)

        file_char = chr(ord('a') + file)
        rank_char = chr(ord('8') - rank)

        return file_char + rank_char

    @property
    def square_size(self):
        return self.size // 8

class GameOverDialog(QDialog):
    def __init__(self, outcome: chess.Outcome | None):
        super().__init__()
        self.setWindowTitle("Game Over")

        layout = QVBoxLayout()
        if outcome is None:
            layout.addWidget(QLabel(f"Game over."))
        else:
            layout.addWidget(QLabel(f"Game over: {outcome.result()}"))
        self.setLayout(layout)

class PromotionDialog(QDialog):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Choose promotion")
        self.selected_piece_type = None

        layout = QVBoxLayout()
        self.setLayout(layout)

        pieces = [
            (chess.QUEEN, "Queen"),
            (chess.ROOK, "Rook"),
            (chess.BISHOP, "Bishop"),
            (chess.KNIGHT, "Knight")
        ]

        for piece_type, name in pieces:
            btn = QPushButton(name)
            btn.clicked.connect(lambda _, p=piece_type: self.select(p))
            layout.addWidget(btn)

    def select(self, piece_type):
        self.selected_piece_type = piece_type
        self.accept()