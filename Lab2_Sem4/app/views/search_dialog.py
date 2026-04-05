from __future__ import annotations

from datetime import date

from PySide6.QtCore import QDate
from PySide6.QtWidgets import (
    QComboBox,
    QDateEdit,
    QDialog,
    QFormLayout,
    QHBoxLayout,
    QLabel,
    QLineEdit,
    QMessageBox,
    QPushButton,
    QTableWidget,
    QTableWidgetItem,
    QTextEdit,
    QVBoxLayout,
)

from app.domain.pagination import Page
from app.domain.validators import SearchValidator
from app.views.pagination_widget import PaginationWidget


class SearchDialog(QDialog):
    def __init__(self, parent=None) -> None:
        super().__init__(parent)
        self.setWindowTitle("Поиск записей")
        self.resize(950, 600)

        self.criteria_combo = QComboBox()
        self.criteria_combo.addItems(
            [
                "По имени питомца и дате рождения",
                "По дате последнего приема и ФИО ветеринара",
                "По фразе из диагноза",
            ]
        )

        self.pet_name_edit = QLineEdit()
        self.birth_date_edit = QDateEdit()
        self.visit_date_edit = QDateEdit()
        self.vet_full_name_edit = QLineEdit()
        self.diagnosis_phrase_edit = QLineEdit()

        for date_edit in (self.birth_date_edit, self.visit_date_edit):
            date_edit.setCalendarPopup(True)
            date_edit.setDisplayFormat("dd.MM.yyyy")
            date_edit.setDate(QDate.currentDate())

        self.search_button = QPushButton("Найти")
        self.reset_button = QPushButton("Сбросить")

        self.results_table = QTableWidget(0, 5)
        self.results_table.setHorizontalHeaderLabels(
            ["Имя питомца", "Дата рождения", "Дата приема", "ФИО ветеринара", "Диагноз"]
        )
        self.results_table.horizontalHeader().setStretchLastSection(True)

        self.pagination = PaginationWidget()

        form = QFormLayout()
        form.addRow("Критерий:", self.criteria_combo)
        form.addRow("Имя питомца:", self.pet_name_edit)
        form.addRow("Дата рождения:", self.birth_date_edit)
        form.addRow("Дата последнего приема:", self.visit_date_edit)
        form.addRow("ФИО ветеринара:", self.vet_full_name_edit)
        form.addRow("Фраза из диагноза:", self.diagnosis_phrase_edit)

        buttons_layout = QHBoxLayout()
        buttons_layout.addWidget(self.search_button)
        buttons_layout.addWidget(self.reset_button)
        buttons_layout.addStretch(1)

        layout = QVBoxLayout(self)
        layout.addLayout(form)
        layout.addLayout(buttons_layout)
        layout.addWidget(QLabel("Результаты поиска:"))
        layout.addWidget(self.results_table)
        layout.addWidget(self.pagination)

        self.criteria_combo.currentIndexChanged.connect(self._update_fields_state)
        self.reset_button.clicked.connect(self._reset_form)
        self._update_fields_state()

    def _update_fields_state(self) -> None:
        index = self.criteria_combo.currentIndex()
        self.pet_name_edit.setEnabled(index == 0)
        self.birth_date_edit.setEnabled(index == 0)
        self.visit_date_edit.setEnabled(index == 1)
        self.vet_full_name_edit.setEnabled(index == 1)
        self.diagnosis_phrase_edit.setEnabled(index == 2)

    def _reset_form(self) -> None:
        self.pet_name_edit.clear()
        self.vet_full_name_edit.clear()
        self.diagnosis_phrase_edit.clear()
        self.birth_date_edit.setDate(QDate.currentDate())
        self.visit_date_edit.setDate(QDate.currentDate())
        self.results_table.setRowCount(0)
        self.pagination.update_state(1, 1, 0, 10)

    def current_criteria_kind(self) -> int:
        return self.criteria_combo.currentIndex()

    def build_criteria_payload(self) -> dict:
        index = self.current_criteria_kind()
        if index == 0:
            if not self.pet_name_edit.text().strip():
                raise ValueError("Введите имя питомца.")
            return {
                "pet_name": self.pet_name_edit.text().strip(),
                "birth_date": self.birth_date_edit.date().toPython(),
            }
        if index == 1:
            full_name = self.vet_full_name_edit.text().strip()
            SearchValidator.validate_veterinarian_full_name(full_name)
            return {
                "last_visit_date": self.visit_date_edit.date().toPython(),
                "veterinarian_full_name": full_name,
            }
        SearchValidator.validate_phrase(self.diagnosis_phrase_edit.text())
        return {"phrase": self.diagnosis_phrase_edit.text().strip()}

    def render_page(self, page: Page) -> None:
        self.results_table.setRowCount(len(page.items))
        for row_index, record in enumerate(page.items):
            values = [
                record.pet_name,
                record.birth_date.strftime("%d.%m.%Y"),
                record.last_visit_date.strftime("%d.%m.%Y"),
                record.veterinarian.full_name(),
                record.diagnosis,
            ]
            for column_index, value in enumerate(values):
                self.results_table.setItem(row_index, column_index, QTableWidgetItem(value))
        self.pagination.update_state(page.page_number, page.total_pages, page.total_items, page.page_size)
