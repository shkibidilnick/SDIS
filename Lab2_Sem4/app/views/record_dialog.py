from __future__ import annotations

from datetime import date

from PySide6.QtCore import QDate
from PySide6.QtWidgets import (
    QDateEdit,
    QDialog,
    QDialogButtonBox,
    QFormLayout,
    QLineEdit,
    QMessageBox,
    QTextEdit,
    QVBoxLayout,
)

from app.domain.models import PersonName, PetRecord
from app.domain.validators import PetRecordValidator


class RecordDialog(QDialog):
    def __init__(self, parent=None) -> None:
        super().__init__(parent)
        self.setWindowTitle("Добавить запись")

        self.pet_name_edit = QLineEdit()
        self.birth_date_edit = QDateEdit()
        self.last_visit_date_edit = QDateEdit()
        self.vet_last_name_edit = QLineEdit()
        self.vet_first_name_edit = QLineEdit()
        self.vet_middle_name_edit = QLineEdit()
        self.diagnosis_edit = QTextEdit()

        for date_edit in (self.birth_date_edit, self.last_visit_date_edit):
            date_edit.setCalendarPopup(True)
            date_edit.setDisplayFormat("dd.MM.yyyy")
            date_edit.setDate(QDate.currentDate())

        form = QFormLayout()
        form.addRow("Имя питомца:", self.pet_name_edit)
        form.addRow("Дата рождения:", self.birth_date_edit)
        form.addRow("Дата последнего приема:", self.last_visit_date_edit)
        form.addRow("Фамилия ветеринара:", self.vet_last_name_edit)
        form.addRow("Имя ветеринара:", self.vet_first_name_edit)
        form.addRow("Отчество ветеринара:", self.vet_middle_name_edit)
        form.addRow("Диагноз:", self.diagnosis_edit)

        buttons = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttons.accepted.connect(self._accept_with_validation)
        buttons.rejected.connect(self.reject)

        layout = QVBoxLayout(self)
        layout.addLayout(form)
        layout.addWidget(buttons)

    def get_record(self) -> PetRecord:
        return PetRecord(
            pet_name=self.pet_name_edit.text().strip(),
            birth_date=self.birth_date_edit.date().toPython(),
            last_visit_date=self.last_visit_date_edit.date().toPython(),
            veterinarian=PersonName(
                self.vet_last_name_edit.text().strip(),
                self.vet_first_name_edit.text().strip(),
                self.vet_middle_name_edit.text().strip(),
            ),
            diagnosis=self.diagnosis_edit.toPlainText().strip(),
        )

    def _accept_with_validation(self) -> None:
        try:
            PetRecordValidator.validate(self.get_record())
        except ValueError as error:
            QMessageBox.warning(self, "Ошибка валидации", str(error))
            return
        self.accept()
