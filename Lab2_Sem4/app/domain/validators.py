from __future__ import annotations

from datetime import date

from app.domain.models import PersonName, PetRecord


class PetRecordValidator:
    @staticmethod
    def validate(record: PetRecord) -> None:
        if not record.pet_name or not record.pet_name.strip():
            raise ValueError("Имя питомца не должно быть пустым.")
        if record.birth_date > date.today():
            raise ValueError("Дата рождения не может быть в будущем.")
        if record.last_visit_date > date.today():
            raise ValueError("Дата последнего приема не может быть в будущем.")
        if record.last_visit_date < record.birth_date:
            raise ValueError("Дата последнего приема не может быть раньше даты рождения.")
        if not record.diagnosis or not record.diagnosis.strip():
            raise ValueError("Диагноз не должен быть пустым.")
        if not record.veterinarian.last_name.strip() or not record.veterinarian.first_name.strip():
            raise ValueError("ФИО ветеринара заполнено некорректно.")


class SearchValidator:
    @staticmethod
    def validate_phrase(phrase: str) -> None:
        if not phrase or not phrase.strip():
            raise ValueError("Фраза для поиска по диагнозу не должна быть пустой.")

    @staticmethod
    def validate_veterinarian_full_name(full_name: str) -> PersonName:
        return PersonName.from_full_name(full_name)
