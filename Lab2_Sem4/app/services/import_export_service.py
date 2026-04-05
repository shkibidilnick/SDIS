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
        self._record_service.replace_all(records)
        return records
