from __future__ import annotations

from datetime import date

from app.domain.criteria import (
    ByDiagnosisPhraseCriteria,
    ByPetNameAndBirthDateCriteria,
    ByVisitDateAndVeterinarianCriteria,
)
from app.domain.validators import SearchValidator
from app.repositories.interfaces import PetRecordRepository


class DeleteService:
    def __init__(self, repository: PetRecordRepository) -> None:
        self._repository = repository

    def delete_by_pet_name_and_birth_date(self, pet_name: str, birth_date: date) -> int:
        return self._repository.delete_by_pet_name_and_birth_date(ByPetNameAndBirthDateCriteria(pet_name.strip(), birth_date))

    def delete_by_visit_date_and_veterinarian(self, last_visit_date: date, veterinarian_full_name: str) -> int:
        SearchValidator.validate_veterinarian_full_name(veterinarian_full_name)
        return self._repository.delete_by_visit_date_and_veterinarian(
            ByVisitDateAndVeterinarianCriteria(last_visit_date, " ".join(veterinarian_full_name.split()))
        )

    def delete_by_diagnosis_phrase(self, phrase: str) -> int:
        SearchValidator.validate_phrase(phrase)
        return self._repository.delete_by_diagnosis_phrase(ByDiagnosisPhraseCriteria(phrase.strip()))
