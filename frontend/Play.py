from typing import Callable

import chess
import chess.engine

from PyQt6.QtCore import QByteArray, QSize, Qt
from PyQt6.QtGui import QPixmap, QIcon, QPainter, QColor
from PyQt6.QtSvg import QSvgRenderer
from PyQt6.QtSvgWidgets import QSvgWidget
from PyQt6.QtWidgets import QWidget, QVBoxLayout, QLabel, QDialog, QPushButton
from chess import WHITE
from chess.engine import SimpleEngine
from chess.svg import SQUARE_SIZE, board

from GameType import GameType

ENGINE = "engine/pestofish2.exe"

class Play(QWidget):
    # Constants
    size: int = 480
    padding: int = 15

    # State management
    to_start: Callable

    # Game
    state: GameType
    board: chess.Board
    selected_square: chess.Square | None = None
    engine: SimpleEngine | None = None

    # Display
    svg_widget: QSvgWidget

    def __init__(self, to_start: Callable):
        super().__init__()

        # State management
        self.to_start = to_start

        # Game
        self.board = chess.Board()
        self.selected_square = None

        # Display
        layout = QVBoxLayout()

        self.svg_widget = QSvgWidget()
        self.svg_widget.setFixedSize(self.size, self.size)
        layout.addWidget(self.svg_widget)
        self.load_board_svg()

        self.setLayout(layout)

    def enter(self, state: GameType):
        self.state = state
        if self.state == GameType.PLAYER_WHITE or self.state == GameType.PLAYER_BLACK:
            self.engine = chess.engine.SimpleEngine.popen_uci(ENGINE)
        if self.is_engine_move():
            self.make_engine_move()

    def load_board_svg(self, **kwargs):
        svg_text = chess.svg.board(self.board, size=self.size, **kwargs)
        svg_bytes = svg_text.encode("utf-8")
        self.svg_widget.load(QByteArray(svg_bytes))

    def mousePressEvent(self, event):
        local = self.svg_widget.mapFromGlobal(event.globalPosition().toPoint())
        sq_str = self.square_clicked(local.x(), local.y())
        if not self.is_valid_square(sq_str):
            return
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
            dialog = PromotionDialog(self.board.turn)
            if dialog.exec() == QDialog.DialogCode.Accepted:
                move.promotion = dialog.selected_piece_type

        if move in self.board.legal_moves:
            self.board.push(move)
            self.selected_square = None

            self.update_after_move()

            if self.is_engine_move():
                self.make_engine_move()

        else:
            self.load_board_svg()
            self.selected_square = None

    def make_engine_move(self):
        assert self.engine is not None
        result = self.engine.play(self.board, limit=chess.engine.Limit(time=1))
        self.board.push(result.move)
        self.update_after_move()

    def update_after_move(self):
        if self.board.is_game_over(claim_draw=True):
            dialog = GameOverDialog(self.board.outcome(claim_draw=True), self.state)
            dialog.exec()
            self.to_start()

        self.load_board_svg()

    def is_engine_move(self):
        return (self.state == GameType.PLAYER_WHITE and self.board.turn == chess.BLACK) \
            or (self.state == GameType.PLAYER_BLACK and self.board.turn == chess.WHITE)

    def is_promotion(self, move: chess.Move) -> bool:
        piece = self.board.piece_at(move.from_square)
        to_rank = chess.square_rank(move.to_square)
        from_rank = chess.square_rank(move.from_square)

        white_promote = piece.color == chess.WHITE and to_rank == 7 and from_rank == 6
        black_promote = piece.color == chess.BLACK and to_rank == 0 and from_rank == 1
        return piece.piece_type == chess.PAWN and (white_promote or black_promote)

    def square_clicked(self, x: float, y: float) -> str:
        file = int((x - self.padding) / self.square_size)
        rank = int((y - self.padding) / self.square_size)

        file_char = chr(ord('a') + file)
        rank_char = chr(ord('8') - rank)

        return file_char + rank_char

    @property
    def square_size(self):
        return (self.size - 2 * self.padding) // 8

    @staticmethod
    def is_valid_square(square: str) -> bool:
        try:
            chess.parse_square(square)
            return True
        except ValueError:
            return False

class GameOverDialog(QDialog):
    def __init__(self, outcome: chess.Outcome | None, game: GameType):
        assert outcome is not None

        super().__init__()
        self.setWindowTitle("Game Over")

        layout = QVBoxLayout()

        white = "assets/player32.png" if game in [GameType.PLAYER_WHITE, GameType.PVP] else "assets/pesto.png"
        black = "assets/player32.png" if game in [GameType.PLAYER_BLACK, GameType.PVP] else "assets/pesto.png"
        for color in [white, black]:
            label = QLabel()
            pixmap = QPixmap(color).scaled(128, 128)
            label.setPixmap(pixmap)
            layout.addWidget(label, alignment=Qt.AlignmentFlag.AlignHCenter)

        if outcome.winner == chess.WHITE:
            text = "White won"
        elif outcome.winner == chess.BLACK:
            text = "Black won"
        else:
            text = "Draw"

        layout.addWidget(QLabel(text), alignment=Qt.AlignmentFlag.AlignHCenter)
        layout.setContentsMargins(20, 20, 20, 20)
        self.setLayout(layout)

class PromotionDialog(QDialog):
    def __init__(self, color: chess.Color):
        super().__init__()
        self.setWindowTitle("Choose promotion")
        self.selected_piece_type = None

        layout = QVBoxLayout()
        self.setLayout(layout)

        pieces = [
            (chess.QUEEN, "Queen", chess.svg.piece(chess.Piece.from_symbol("Q" if color == WHITE else "q"))),
            (chess.ROOK, "Rook", chess.svg.piece(chess.Piece.from_symbol("R" if color == WHITE else "r"))),
            (chess.BISHOP, "Bishop", chess.svg.piece(chess.Piece.from_symbol("B" if color == WHITE else "b"))),
            (chess.KNIGHT, "Knight", chess.svg.piece(chess.Piece.from_symbol("N" if color == WHITE else "n")))
        ]

        for piece_type, name, svg in pieces:
            renderer = QSvgRenderer(QByteArray(svg.encode("utf-8")))

            pixmap = QPixmap(48, 48)
            pixmap.fill(QColor(0, 0, 0, 0))

            painter = QPainter(pixmap)
            renderer.render(painter)
            painter.end()

            icon = QIcon(pixmap)

            btn = QPushButton(name)
            btn.setIcon(icon)
            btn.setIconSize(QSize(48, 48))
            btn.clicked.connect(lambda _, p=piece_type: self.select(p))

            layout.addWidget(btn)

    def select(self, piece_type):
        self.selected_piece_type = piece_type
        self.accept()