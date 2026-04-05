from __future__ import annotations

from dataclasses import dataclass
from datetime import date
from typing import Optional


@dataclass(frozen=True)
class PersonName:
    last_name: str
    first_name: str
    middle_name: str = ""

    def full_name(self) -> str:
        parts = [self.last_name.strip(), self.first_name.strip(), self.middle_name.strip()]
        return " ".join(part for part in parts if part)

    @classmethod
    def from_full_name(cls, value: str) -> "PersonName":
        parts = [part.strip() for part in value.split() if part.strip()]
        if len(parts) < 2:
            raise ValueError("ФИО ветеринара должно содержать минимум фамилию и имя.")
        if len(parts) == 2:
            return cls(parts[0], parts[1], "")
        return cls(parts[0], parts[1], " ".join(parts[2:]))


@dataclass(frozen=True)
class PetRecord:
    pet_name: str
    birth_date: date
    last_visit_date: date
    veterinarian: PersonName
    diagnosis: str
    record_id: Optional[int] = None

    def with_id(self, record_id: int) -> "PetRecord":
        return PetRecord(
            record_id=record_id,
            pet_name=self.pet_name,
            birth_date=self.birth_date,
            last_visit_date=self.last_visit_date,
            veterinarian=self.veterinarian,
            diagnosis=self.diagnosis,
        )
