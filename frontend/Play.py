import chess
from PyQt6.QtCore import QByteArray
from PyQt6.QtSvgWidgets import QSvgWidget
from PyQt6.QtWidgets import QWidget, QVBoxLayout, QLabel
from chess.svg import SQUARE_SIZE, board

from GameType import GameType

class Play(QWidget):
    state: GameType
    size: int = 480
    board_frame_size: int = 5

    def __init__(self):
        super().__init__()

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
            move = chess.Move(from_square=self.selected_square, to_square=square)
            if move in self.board.legal_moves:
                self.board.push(move)
                self.load_board_svg()
                self.selected_square = None
            else:
                self.load_board_svg()
                self.selected_square = None

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

    def square_clicked(self, x: float, y: float) -> str:
        file = round((x-self.board_frame_size) // self.square_size)
        rank = round((y-self.board_frame_size) // self.square_size)

        file_char = chr(ord('a') + file)
        rank_char = chr(ord('8') - rank)

        return file_char + rank_char

    @property
    def square_size(self):
        return self.size // 8
