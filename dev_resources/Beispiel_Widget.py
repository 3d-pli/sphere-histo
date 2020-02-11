class MyWidget(QtWidgets.QWidget):
    counter = 0

    def __init__(self):
        super().__init__()

        self.hello = ["Hallihallo", "Blabla", "Blubb"]

        self.button = QtWidgets.QPushButton("Click me pls!")
        self.text = QtWidgets.QLabel("Hello World")
        self.text.setAlignment(QtCore.Qt.AlignCenter)

        self.layout = QtWidgets.QVBoxLayout()
        self.layout.addWidget(self.text)
        self.layout.addWidget(self.button)

        self.setLayout(self.layout)

        self.button.clicked.connect(self.change_text)

    def change_text(self):
        MyWidget.counter = (MyWidget.counter + 1) % len(self.hello)
        self.text.setText(self.hello[MyWidget.counter])

