import sys
from PyQt6.QtWidgets import QApplication, QWidget, QStackedWidget

from Start import Start
from Play import Play
from GameType import GameType


class WidgetManager(QStackedWidget):
    def __init__(self):
        super().__init__()

        self.start = Start(self.to_play)
        self.play = Play(self.to_start)

        self.addWidget(self.start)
        self.addWidget(self.play)

        self.setCurrentIndex(0)

        self.setWindowTitle("PestoFish2")
        self.resize(400, 300)

    def to_play(self, game_type: GameType):
        self.play.set_state(game_type)
        self.setCurrentIndex(1)

    def to_start(self):
        self.setCurrentIndex(0)

def main():
    app = QApplication(sys.argv)

    window = WidgetManager()

    window.show()

    sys.exit(app.exec())

if __name__ == "__main__":
    main()
