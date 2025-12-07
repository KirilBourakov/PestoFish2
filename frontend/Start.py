from typing import Callable

from PyQt6.QtGui import QPixmap
from PyQt6.QtWidgets import QPushButton, QLabel, QVBoxLayout, QWidget

from GameType import GameType


class Start(QWidget):
    def __init__(self, switch_callback: Callable):
        super().__init__()
        self.switch_callback = switch_callback

        layout = QVBoxLayout()
        layout.addWidget(QLabel("<h1>Pestofish</h1>"))
        layout.addWidget(self.get_logo())

        # Start Game buttons
        for gameType in GameType:
            button = QPushButton(gameType.value)
            button.clicked.connect(lambda _, b=gameType: self.switch_callback(b))
            layout.addWidget(button)

        self.setLayout(layout)

    def get_logo(self) -> QLabel:
        label = QLabel()
        pixmap = QPixmap("assets/pesto.png").scaled(400, 400)
        label.setPixmap(pixmap)
        label.setScaledContents(True)
        return label
