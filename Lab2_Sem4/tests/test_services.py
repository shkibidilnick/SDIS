from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from app.repositories.sqlite_repository import SQLitePetRecordRepository
from app.services.delete_service import DeleteService
from app.services.import_export_service import ImportExportService
from app.services.record_service import PetRecordService
from app.services.search_service import SearchService
from tests.conftest import build_sample_record, build_second_record


class ServiceTests(unittest.TestCase):
    def setUp(self) -> None:
        self._tmp_dir = tempfile.TemporaryDirectory()
        self.db_path = Path(self._tmp_dir.name) / "test.sqlite3"
        self.repository = SQLitePetRecordRepository(self.db_path)
        self.sample_record = build_sample_record()
        self.second_record = build_second_record()

    def tearDown(self) -> None:
        self._tmp_dir.cleanup()

    def test_record_service_add_and_paginate(self) -> None:
        service = PetRecordService(self.repository)
        service.add_record(self.sample_record)
        service.add_record(self.second_record)

        page = service.get_page(1, 1)
        self.assertEqual(2, page.total_items)
        self.assertEqual(1, len(page.items))

    def test_search_service_by_phrase(self) -> None:
        self.repository.add(self.sample_record)
        self.repository.add(self.second_record)
        service = SearchService(self.repository)

        page = service.search_by_diagnosis_phrase("лечение")
        self.assertEqual(1, page.total_items)
        self.assertEqual("Мурка", page.items[0].pet_name)

    def test_delete_service_by_visit_and_veterinarian(self) -> None:
        self.repository.add(self.sample_record)
        self.repository.add(self.second_record)
        service = DeleteService(self.repository)

        deleted = service.delete_by_visit_date_and_veterinarian(
            self.sample_record.last_visit_date, "Иванов Петр Сергеевич"
        )
        self.assertEqual(2, deleted)

    def test_import_export_service_replaces_records(self) -> None:
        record_service = PetRecordService(self.repository)
        record_service.add_record(self.sample_record)
        record_service.add_record(self.second_record)

        xml_path = Path(self._tmp_dir.name) / "pets.xml"
        exchange = ImportExportService(record_service)
        exchange.export_to_xml(xml_path)

        self.repository.replace_all([])
        loaded = exchange.import_from_xml(xml_path)

        self.assertEqual(2, len(loaded))
        self.assertEqual(2, len(record_service.get_all_records()))

    def test_search_service_rejects_invalid_phrase(self) -> None:
        service = SearchService(self.repository)
        with self.assertRaises(ValueError):
            service.search_by_diagnosis_phrase("   ")


if __name__ == "__main__":
    unittest.main()