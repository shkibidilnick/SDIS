from __future__ import annotations

from dataclasses import dataclass
from datetime import date


@dataclass(frozen=True)
class ByPetNameAndBirthDateCriteria:
    pet_name: str
    birth_date: date


@dataclass(frozen=True)
class ByVisitDateAndVeterinarianCriteria:
    last_visit_date: date
    veterinarian_full_name: str


@dataclass(frozen=True)
class ByDiagnosisPhraseCriteria:
    phrase: str
