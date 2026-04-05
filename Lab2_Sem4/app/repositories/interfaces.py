from __future__ import annotations

from abc import ABC, abstractmethod

from app.domain.criteria import (
    ByDiagnosisPhraseCriteria,
    ByPetNameAndBirthDateCriteria,
    ByVisitDateAndVeterinarianCriteria,
)
from app.domain.models import PetRecord


class PetRecordRepository(ABC):
    @abstractmethod
    def add(self, record: PetRecord) -> PetRecord:
        raise NotImplementedError

    @abstractmethod
    def get_all(self) -> list[PetRecord]:
        raise NotImplementedError

    @abstractmethod
    def replace_all(self, records: list[PetRecord]) -> None:
        raise NotImplementedError

    @abstractmethod
    def search_by_pet_name_and_birth_date(self, criteria: ByPetNameAndBirthDateCriteria) -> list[PetRecord]:
        raise NotImplementedError

    @abstractmethod
    def search_by_visit_date_and_veterinarian(
        self, criteria: ByVisitDateAndVeterinarianCriteria
    ) -> list[PetRecord]:
        raise NotImplementedError

    @abstractmethod
    def search_by_diagnosis_phrase(self, criteria: ByDiagnosisPhraseCriteria) -> list[PetRecord]:
        raise NotImplementedError

    @abstractmethod
    def delete_by_pet_name_and_birth_date(self, criteria: ByPetNameAndBirthDateCriteria) -> int:
        raise NotImplementedError

    @abstractmethod
    def delete_by_visit_date_and_veterinarian(self, criteria: ByVisitDateAndVeterinarianCriteria) -> int:
        raise NotImplementedError

    @abstractmethod
    def delete_by_diagnosis_phrase(self, criteria: ByDiagnosisPhraseCriteria) -> int:
        raise NotImplementedError
