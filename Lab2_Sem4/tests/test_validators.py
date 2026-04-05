from __future__ import annotations

import unittest
from datetime import date, timedelta

from app.domain.models import PetRecord
from app.domain.validators import PetRecordValidator, SearchValidator
from tests.conftest import build_sample_record


class ValidatorTests(unittest.TestCase):
    def setUp(self) -> None:
        self.sample_record = build_sample_record()

    def test_validator_accepts_correct_record(self) -> None:
        PetRecordValidator.validate(self.sample_record)

    def test_validator_rejects_future_birth_date(self) -> None:
        invalid = PetRecord(
            pet_name=self.sample_record.pet_name,
            birth_date=date.today() + timedelta(days=1),
            last_visit_date=self.sample_record.last_visit_date,
            veterinarian=self.sample_record.veterinarian,
            diagnosis=self.sample_record.diagnosis,
        )
        with self.assertRaises(ValueError):
            PetRecordValidator.validate(invalid)

    def test_validator_rejects_last_visit_before_birth(self) -> None:
        invalid = PetRecord(
            pet_name=self.sample_record.pet_name,
            birth_date=date(2025, 1, 1),
            last_visit_date=date(2024, 1, 1),
            veterinarian=self.sample_record.veterinarian,
            diagnosis=self.sample_record.diagnosis,
        )
        with self.assertRaises(ValueError):
            PetRecordValidator.validate(invalid)

    def test_search_phrase_must_not_be_empty(self) -> None:
        with self.assertRaises(ValueError):
            SearchValidator.validate_phrase("   ")

    def test_search_veterinarian_name_requires_at_least_two_parts(self) -> None:
        with self.assertRaises(ValueError):
            SearchValidator.validate_veterinarian_full_name("Иванов")


if __name__ == "__main__":
    unittest.main()