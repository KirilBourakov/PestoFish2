from typing import Callable

from PyQt6.QtWidgets import QPushButton, QLabel, QVBoxLayout, QWidget

from GameType import GameType


class Start(QWidget):
    def __init__(self, switch_callback: Callable):
        super().__init__()
        self.switch_callback = switch_callback



        white = QPushButton("Play as White")
        black = QPushButton("Play as Black")
        pvp = QPushButton("PVP")
        white.clicked.connect(lambda _, b=GameType.PLAYER_WHITE: self.switch_callback(b))
        black.clicked.connect(lambda _, b=GameType.PLAYER_BLACK: self.switch_callback(b))
        pvp.clicked.connect(lambda _, b=GameType.PVP: self.switch_callback(b))

        layout = QVBoxLayout()
        layout.addWidget(QLabel("Pestofish!"))
        layout.addWidget(white)
        layout.addWidget(black)
        layout.addWidget(pvp)


        self.setLayout(layout)