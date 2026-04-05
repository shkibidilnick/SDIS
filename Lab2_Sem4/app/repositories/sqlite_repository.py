from __future__ import annotations

import sqlite3
from pathlib import Path

from app.domain.criteria import (
    ByDiagnosisPhraseCriteria,
    ByPetNameAndBirthDateCriteria,
    ByVisitDateAndVeterinarianCriteria,
)
from app.domain.models import PersonName, PetRecord
from app.repositories.interfaces import PetRecordRepository


class SQLitePetRecordRepository(PetRecordRepository):
    def __init__(self, db_path: str | Path) -> None:
        self._db_path = str(db_path)
        self._ensure_schema()

    def _connect(self) -> sqlite3.Connection:
        connection = sqlite3.connect(self._db_path)
        connection.row_factory = sqlite3.Row
        connection.create_function("NORM", 1, self._normalize)
        return connection

    @staticmethod
    def _normalize(value: str | None) -> str:
        return (value or "").strip().casefold()

    def _ensure_schema(self) -> None:
        with self._connect() as connection:
            connection.execute(
                """
                CREATE TABLE IF NOT EXISTS pet_records (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    pet_name TEXT NOT NULL,
                    birth_date TEXT NOT NULL,
                    last_visit_date TEXT NOT NULL,
                    veterinarian_last_name TEXT NOT NULL,
                    veterinarian_first_name TEXT NOT NULL,
                    veterinarian_middle_name TEXT NOT NULL,
                    diagnosis TEXT NOT NULL
                )
                """
            )
            connection.commit()

    def add(self, record: PetRecord) -> PetRecord:
        with self._connect() as connection:
            cursor = connection.execute(
                """
                INSERT INTO pet_records (
                    pet_name, birth_date, last_visit_date,
                    veterinarian_last_name, veterinarian_first_name, veterinarian_middle_name,
                    diagnosis
                ) VALUES (?, ?, ?, ?, ?, ?, ?)
                """,
                (
                    record.pet_name.strip(),
                    record.birth_date.isoformat(),
                    record.last_visit_date.isoformat(),
                    record.veterinarian.last_name.strip(),
                    record.veterinarian.first_name.strip(),
                    record.veterinarian.middle_name.strip(),
                    record.diagnosis.strip(),
                ),
            )
            connection.commit()
            return record.with_id(int(cursor.lastrowid))

    def get_all(self) -> list[PetRecord]:
        with self._connect() as connection:
            rows = connection.execute(
                "SELECT * FROM pet_records ORDER BY last_visit_date DESC, pet_name ASC, id ASC"
            ).fetchall()
            return [self._row_to_record(row) for row in rows]

    def replace_all(self, records: list[PetRecord]) -> None:
        with self._connect() as connection:
            connection.execute("DELETE FROM pet_records")
            for record in records:
                connection.execute(
                    """
                    INSERT INTO pet_records (
                        pet_name, birth_date, last_visit_date,
                        veterinarian_last_name, veterinarian_first_name, veterinarian_middle_name,
                        diagnosis
                    ) VALUES (?, ?, ?, ?, ?, ?, ?)
                    """,
                    (
                        record.pet_name.strip(),
                        record.birth_date.isoformat(),
                        record.last_visit_date.isoformat(),
                        record.veterinarian.last_name.strip(),
                        record.veterinarian.first_name.strip(),
                        record.veterinarian.middle_name.strip(),
                        record.diagnosis.strip(),
                    ),
                )
            connection.commit()

    def search_by_pet_name_and_birth_date(self, criteria: ByPetNameAndBirthDateCriteria) -> list[PetRecord]:
        with self._connect() as connection:
            rows = connection.execute(
                """
                SELECT * FROM pet_records
                WHERE NORM(pet_name) = NORM(?) AND birth_date = ?
                ORDER BY last_visit_date DESC, pet_name ASC
                """,
                (criteria.pet_name.strip(), criteria.birth_date.isoformat()),
            ).fetchall()
            return [self._row_to_record(row) for row in rows]

    def search_by_visit_date_and_veterinarian(
        self, criteria: ByVisitDateAndVeterinarianCriteria
    ) -> list[PetRecord]:
        full_name = " ".join(criteria.veterinarian_full_name.split())
        with self._connect() as connection:
            rows = connection.execute(
                """
                SELECT * FROM pet_records
                WHERE last_visit_date = ?
                  AND NORM(trim(
                        veterinarian_last_name || ' ' || veterinarian_first_name ||
                        CASE
                            WHEN veterinarian_middle_name <> '' THEN ' ' || veterinarian_middle_name
                            ELSE ''
                        END
                    )) = NORM(?)
                ORDER BY pet_name ASC, id ASC
                """,
                (criteria.last_visit_date.isoformat(), full_name),
            ).fetchall()
            return [self._row_to_record(row) for row in rows]

    def search_by_diagnosis_phrase(self, criteria: ByDiagnosisPhraseCriteria) -> list[PetRecord]:
        phrase = self._normalize(criteria.phrase)
        return [record for record in self.get_all() if phrase in self._normalize(record.diagnosis)]

    def delete_by_pet_name_and_birth_date(self, criteria: ByPetNameAndBirthDateCriteria) -> int:
        return self._delete(
            "DELETE FROM pet_records WHERE NORM(pet_name) = NORM(?) AND birth_date = ?",
            (criteria.pet_name.strip(), criteria.birth_date.isoformat()),
        )

    def delete_by_visit_date_and_veterinarian(self, criteria: ByVisitDateAndVeterinarianCriteria) -> int:
        full_name = " ".join(criteria.veterinarian_full_name.split())
        return self._delete(
            """
            DELETE FROM pet_records
            WHERE last_visit_date = ?
              AND NORM(trim(
                    veterinarian_last_name || ' ' || veterinarian_first_name ||
                    CASE
                        WHEN veterinarian_middle_name <> '' THEN ' ' || veterinarian_middle_name
                        ELSE ''
                    END
                )) = NORM(?)
            """,
            (criteria.last_visit_date.isoformat(), full_name),
        )

    def delete_by_diagnosis_phrase(self, criteria: ByDiagnosisPhraseCriteria) -> int:
        matches = self.search_by_diagnosis_phrase(criteria)
        with self._connect() as connection:
            for record in matches:
                connection.execute("DELETE FROM pet_records WHERE id = ?", (record.record_id,))
            connection.commit()
        return len(matches)

    def _delete(self, sql: str, params: tuple) -> int:
        with self._connect() as connection:
            cursor = connection.execute(sql, params)
            connection.commit()
            return cursor.rowcount

    @staticmethod
    def _row_to_record(row: sqlite3.Row) -> PetRecord:
        from datetime import date

        return PetRecord(
            record_id=row["id"],
            pet_name=row["pet_name"],
            birth_date=date.fromisoformat(row["birth_date"]),
            last_visit_date=date.fromisoformat(row["last_visit_date"]),
            veterinarian=PersonName(
                row["veterinarian_last_name"],
                row["veterinarian_first_name"],
                row["veterinarian_middle_name"],
            ),
            diagnosis=row["diagnosis"],
        )
