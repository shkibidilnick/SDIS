from __future__ import annotations

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
    QVBoxLayout,
)

from app.domain.validators import SearchValidator


class DeleteDialog(QDialog):
    def __init__(self, parent=None) -> None:
        super().__init__(parent)
        self.setWindowTitle("Удаление записей")
        self.resize(500, 280)

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
        self.delete_button = QPushButton("Удалить")
        self.close_button = QPushButton("Закрыть")
        self.result_label = QLabel("")

        for date_edit in (self.birth_date_edit, self.visit_date_edit):
            date_edit.setCalendarPopup(True)
            date_edit.setDisplayFormat("dd.MM.yyyy")
            date_edit.setDate(QDate.currentDate())

        form = QFormLayout()
        form.addRow("Критерий:", self.criteria_combo)
        form.addRow("Имя питомца:", self.pet_name_edit)
        form.addRow("Дата рождения:", self.birth_date_edit)
        form.addRow("Дата последнего приема:", self.visit_date_edit)
        form.addRow("ФИО ветеринара:", self.vet_full_name_edit)
        form.addRow("Фраза из диагноза:", self.diagnosis_phrase_edit)

        buttons = QHBoxLayout()
        buttons.addWidget(self.delete_button)
        buttons.addWidget(self.close_button)
        buttons.addStretch(1)

        layout = QVBoxLayout(self)
        layout.addLayout(form)
        layout.addLayout(buttons)
        layout.addWidget(self.result_label)

        self.criteria_combo.currentIndexChanged.connect(self._update_fields_state)
        self.close_button.clicked.connect(self.reject)
        self._update_fields_state()

    def _update_fields_state(self) -> None:
        index = self.criteria_combo.currentIndex()
        self.pet_name_edit.setEnabled(index == 0)
        self.birth_date_edit.setEnabled(index == 0)
        self.visit_date_edit.setEnabled(index == 1)
        self.vet_full_name_edit.setEnabled(index == 1)
        self.diagnosis_phrase_edit.setEnabled(index == 2)

    def build_criteria_payload(self) -> dict:
        index = self.criteria_combo.currentIndex()
        if index == 0:
            if not self.pet_name_edit.text().strip():
                raise ValueError("Введите имя питомца.")
            return {
                "kind": index,
                "pet_name": self.pet_name_edit.text().strip(),
                "birth_date": self.birth_date_edit.date().toPython(),
            }
        if index == 1:
            full_name = self.vet_full_name_edit.text().strip()
            SearchValidator.validate_veterinarian_full_name(full_name)
            return {
                "kind": index,
                "last_visit_date": self.visit_date_edit.date().toPython(),
                "veterinarian_full_name": full_name,
            }
        SearchValidator.validate_phrase(self.diagnosis_phrase_edit.text())
        return {"kind": index, "phrase": self.diagnosis_phrase_edit.text().strip()}

    def show_result(self, deleted_count: int) -> None:
        if deleted_count > 0:
            message = f"Удалено записей: {deleted_count}"
            self.result_label.setText(message)
            QMessageBox.information(self, "Удаление завершено", message)
        else:
            message = "Записи по указанным условиям не найдены."
            self.result_label.setText(message)
            QMessageBox.information(self, "Удаление завершено", message)

    def show_validation_error(self, message: str) -> None:
        QMessageBox.warning(self, "Некорректные данные", message)