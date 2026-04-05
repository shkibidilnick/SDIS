from __future__ import annotations

import tempfile
import unittest
from datetime import date
from pathlib import Path

from app.repositories.sqlite_repository import SQLitePetRecordRepository
from tests.conftest import build_sample_record, build_second_record


class RepositoryTests(unittest.TestCase):
    def setUp(self) -> None:
        self._tmp_dir = tempfile.TemporaryDirectory()
        self.db_path = Path(self._tmp_dir.name) / "test.sqlite3"
        self.repository = SQLitePetRecordRepository(self.db_path)
        self.sample_record = build_sample_record()
        self.second_record = build_second_record()

    def tearDown(self) -> None:
        self._tmp_dir.cleanup()

    def test_repository_add_and_get_all(self) -> None:
        self.repository.add(self.sample_record)
        self.repository.add(self.second_record)

        all_records = self.repository.get_all()
        self.assertEqual(2, len(all_records))
        self.assertEqual({"Барсик", "Мурка"}, {record.pet_name for record in all_records})

    def test_repository_search_by_pet_name_and_birth_date(self) -> None:
        self.repository.add(self.sample_record)

        criteria = type("Criteria", (), {"pet_name": "Барсик", "birth_date": date(2020, 5, 10)})()
        results = self.repository.search_by_pet_name_and_birth_date(criteria)

        self.assertEqual(1, len(results))
        self.assertEqual("Плановый осмотр, здоров", results[0].diagnosis)

    def test_repository_search_and_delete_by_diagnosis_phrase(self) -> None:
        self.repository.add(self.sample_record)
        self.repository.add(self.second_record)

        search_criteria = type("Criteria", (), {"phrase": "аллерг"})()
        results = self.repository.search_by_diagnosis_phrase(search_criteria)
        self.assertEqual(1, len(results))
        self.assertEqual("Мурка", results[0].pet_name)

        delete_criteria = type("Criteria", (), {"phrase": "аллерг"})()
        deleted = self.repository.delete_by_diagnosis_phrase(delete_criteria)
        self.assertEqual(1, deleted)
        self.assertEqual(1, len(self.repository.get_all()))

    def test_repository_search_by_visit_date_and_veterinarian(self) -> None:
        self.repository.add(self.sample_record)
        self.repository.add(self.second_record)

        criteria = type(
            "Criteria",
            (),
            {"last_visit_date": date(2025, 11, 1), "veterinarian_full_name": "Иванов Петр Сергеевич"},
        )()
        results = self.repository.search_by_visit_date_and_veterinarian(criteria)

        self.assertEqual(2, len(results))


if __name__ == "__main__":
    unittest.main()