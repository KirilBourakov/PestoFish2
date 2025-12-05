import sys
from PyQt6.QtWidgets import QApplication, QWidget, QStackedWidget

from Start import Start
from Play import Play


class MainWindow(QStackedWidget):
    def __init__(self):
        super().__init__()

        self.landing = Start(self.show_game)
        self.game = Play()

        self.addWidget(self.landing)
        self.addWidget(self.game)

        self.setCurrentIndex(0)

        self.setWindowTitle("PestoFish2")
        self.resize(400, 300)

    def show_game(self):
        self.setCurrentIndex(1)


def main():
    app = QApplication(sys.argv)

    window = MainWindow()

    window.show()

    sys.exit(app.exec())

if __name__ == "__main__":
    main()
