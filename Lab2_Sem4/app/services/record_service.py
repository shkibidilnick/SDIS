from __future__ import annotations

from app.domain.models import PetRecord
from app.domain.pagination import Page, Paginator
from app.domain.validators import PetRecordValidator
from app.repositories.interfaces import PetRecordRepository


class PetRecordService:
    def __init__(self, repository: PetRecordRepository) -> None:
        self._repository = repository
        self._paginator = Paginator[PetRecord]()

    def add_record(self, record: PetRecord) -> PetRecord:
        PetRecordValidator.validate(record)
        return self._repository.add(record)

    def get_all_records(self) -> list[PetRecord]:
        return self._repository.get_all()

    def get_page(self, page_number: int, page_size: int) -> Page[PetRecord]:
        return self._paginator.paginate(self.get_all_records(), page_number, page_size)

    def replace_all(self, records: list[PetRecord]) -> None:
        for record in records:
            PetRecordValidator.validate(record)
        self._repository.replace_all(records)
