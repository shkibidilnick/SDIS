from __future__ import annotations

from pathlib import Path

from app.domain.models import PetRecord
from app.services.record_service import PetRecordService
from app.xmlio.dom_writer import PetRecordDomWriter
from app.xmlio.sax_reader import PetRecordSaxReader


class ImportExportService:
    def __init__(
        self,
        record_service: PetRecordService,
        writer: PetRecordDomWriter | None = None,
        reader: PetRecordSaxReader | None = None,
    ) -> None:
        self._record_service = record_service
        self._writer = writer or PetRecordDomWriter()
        self._reader = reader or PetRecordSaxReader()

    def export_to_xml(self, file_path: str | Path) -> None:
        records = self._record_service.get_all_records()
        self._writer.write(records, file_path)

    def import_from_xml(self, file_path: str | Path) -> list[PetRecord]:
        records = self._reader.read(file_path)
        existing_keys = {
            self._build_record_key(record) for record in self._record_service.get_all_records()
        }

        imported_records: list[PetRecord] = []

        for record in records:
            record_key = self._build_record_key(record)
            if record_key in existing_keys:
                continue

            saved_record = self._record_service.add_record(record)
            imported_records.append(saved_record)
            existing_keys.add(record_key)

        return imported_records

    @staticmethod
    def _build_record_key(record: PetRecord) -> tuple[str, str, str, str, str, str, str]:
        return (
            record.pet_name.strip().casefold(),
            record.birth_date.isoformat(),
            record.last_visit_date.isoformat(),
            record.veterinarian.last_name.strip().casefold(),
            record.veterinarian.first_name.strip().casefold(),
            record.veterinarian.middle_name.strip().casefold(),
            record.diagnosis.strip().casefold(),
        )