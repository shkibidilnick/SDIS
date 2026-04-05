from __future__ import annotations

from datetime import date

from app.domain.models import PersonName, PetRecord


def build_sample_record() -> PetRecord:
    return PetRecord(
        pet_name="Барсик",
        birth_date=date(2020, 5, 10),
        last_visit_date=date(2025, 11, 1),
        veterinarian=PersonName("Иванов", "Петр", "Сергеевич"),
        diagnosis="Плановый осмотр, здоров",
    )


def build_second_record() -> PetRecord:
    return PetRecord(
        pet_name="Мурка",
        birth_date=date(2019, 3, 15),
        last_visit_date=date(2025, 11, 1),
        veterinarian=PersonName("Иванов", "Петр", "Сергеевич"),
        diagnosis="Аллергическая реакция, назначено лечение",
    )