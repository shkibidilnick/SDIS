from __future__ import annotations

from datetime import date

from app.domain.criteria import (
    ByDiagnosisPhraseCriteria,
    ByPetNameAndBirthDateCriteria,
    ByVisitDateAndVeterinarianCriteria,
)
from app.domain.models import PetRecord
from app.domain.pagination import Page, Paginator
from app.domain.validators import SearchValidator
from app.repositories.interfaces import PetRecordRepository


class SearchService:
    def __init__(self, repository: PetRecordRepository) -> None:
        self._repository = repository
        self._paginator = Paginator[PetRecord]()

    def search_by_pet_name_and_birth_date(
        self, pet_name: str, birth_date: date, page_number: int = 1, page_size: int = 10
    ) -> Page[PetRecord]:
        records = self._repository.search_by_pet_name_and_birth_date(
            ByPetNameAndBirthDateCriteria(pet_name.strip(), birth_date)
        )
        return self._paginator.paginate(records, page_number, page_size)

    def search_by_visit_date_and_veterinarian(
        self, last_visit_date: date, veterinarian_full_name: str, page_number: int = 1, page_size: int = 10
    ) -> Page[PetRecord]:
        SearchValidator.validate_veterinarian_full_name(veterinarian_full_name)
        records = self._repository.search_by_visit_date_and_veterinarian(
            ByVisitDateAndVeterinarianCriteria(last_visit_date, " ".join(veterinarian_full_name.split()))
        )
        return self._paginator.paginate(records, page_number, page_size)

    def search_by_diagnosis_phrase(self, phrase: str, page_number: int = 1, page_size: int = 10) -> Page[PetRecord]:
        SearchValidator.validate_phrase(phrase)
        records = self._repository.search_by_diagnosis_phrase(ByDiagnosisPhraseCriteria(phrase.strip()))
        return self._paginator.paginate(records, page_number, page_size)
