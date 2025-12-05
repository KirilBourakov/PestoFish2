from PyQt6.QtWidgets import QWidget, QVBoxLayout, QLabel


class Play(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        layout.addWidget(QLabel("This is the Game Screen"))
        self.setLayout(layout)