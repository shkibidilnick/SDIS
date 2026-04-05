from __future__ import annotations

from PySide6.QtCore import Signal
from PySide6.QtWidgets import (
    QComboBox,
    QHBoxLayout,
    QLabel,
    QPushButton,
    QWidget,
)


class PaginationWidget(QWidget):
    page_changed = Signal(int, int)

    def __init__(self, parent=None) -> None:
        super().__init__(parent)
        self._current_page = 1
        self._total_pages = 1
        self._page_size = 10

        self.first_button = QPushButton("<<")
        self.previous_button = QPushButton("<")
        self.next_button = QPushButton(">")
        self.last_button = QPushButton(">>")

        self.first_button.setToolTip("Перейти на первую страницу")
        self.previous_button.setToolTip("Перейти на предыдущую страницу")
        self.next_button.setToolTip("Перейти на следующую страницу")
        self.last_button.setToolTip("Перейти на последнюю страницу")

        self.page_size_combo = QComboBox()
        self.page_size_combo.addItems(["5", "10", "20", "50"])
        self.page_size_combo.setToolTip("Количество записей, отображаемых на одной странице")

        self.info_label = QLabel("Страница 1 из 1 | Всего записей: 0")

        layout = QHBoxLayout(self)
        layout.addWidget(self.first_button)
        layout.addWidget(self.previous_button)
        layout.addWidget(self.next_button)
        layout.addWidget(self.last_button)
        layout.addWidget(QLabel("На странице:"))
        layout.addWidget(self.page_size_combo)
        layout.addStretch(1)
        layout.addWidget(self.info_label)

        self.first_button.clicked.connect(lambda: self._emit_page(1))
        self.previous_button.clicked.connect(lambda: self._emit_page(self._current_page - 1))
        self.next_button.clicked.connect(lambda: self._emit_page(self._current_page + 1))
        self.last_button.clicked.connect(lambda: self._emit_page(self._total_pages))
        self.page_size_combo.currentTextChanged.connect(self._on_page_size_changed)

    def update_state(self, current_page: int, total_pages: int, total_items: int, page_size: int) -> None:
        self._current_page = max(1, current_page)
        self._total_pages = max(1, total_pages)
        self._page_size = page_size
        self.page_size_combo.setCurrentText(str(page_size))
        self.info_label.setText(
            f"Страница {self._current_page} из {self._total_pages} | Всего записей: {total_items}"
        )
        self.first_button.setEnabled(self._current_page > 1)
        self.previous_button.setEnabled(self._current_page > 1)
        self.next_button.setEnabled(self._current_page < self._total_pages)
        self.last_button.setEnabled(self._current_page < self._total_pages)

    def _on_page_size_changed(self, value: str) -> None:
        self._page_size = int(value)
        self.page_changed.emit(1, self._page_size)

    def _emit_page(self, page: int) -> None:
        page = min(max(1, page), self._total_pages)
        self.page_changed.emit(page, self._page_size)