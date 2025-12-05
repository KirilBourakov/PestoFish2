from PyQt6.QtWidgets import QPushButton, QLabel, QVBoxLayout, QWidget


class Start(QWidget):
    def __init__(self, switch_callback):
        super().__init__()
        layout = QVBoxLayout()

        layout.addWidget(QLabel("Welcome to the Game!"))
        btn = QPushButton("Start Playing")
        btn.clicked.connect(switch_callback)

        layout.addWidget(btn)
        self.setLayout(layout)