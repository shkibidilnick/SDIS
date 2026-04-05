from __future__ import annotations

from datetime import date
from pathlib import Path
from xml.sax import ContentHandler, make_parser

from app.domain.models import PersonName, PetRecord


class _PetRecordSaxHandler(ContentHandler):
    def __init__(self) -> None:
        super().__init__()
        self.records: list[PetRecord] = []
        self._current_tag = ""
        self._current_pet: dict[str, str] = {}
        self._current_vet: dict[str, str] = {}
        self._buffer: list[str] = []
        self._inside_veterinarian = False

    def startElement(self, name, attrs):
        self._current_tag = name
        self._buffer = []
        if name == "pet":
            self._current_pet = {}
            self._current_vet = {"last_name": "", "first_name": "", "middle_name": ""}
        elif name == "veterinarian":
            self._inside_veterinarian = True

    def characters(self, content):
        self._buffer.append(content)

    def endElement(self, name):
        text = "".join(self._buffer).strip()
        if name in {"name", "birth_date", "last_visit_date", "diagnosis"}:
            self._current_pet[name] = text
        elif self._inside_veterinarian and name in {"last_name", "first_name", "middle_name"}:
            self._current_vet[name] = text
        elif name == "veterinarian":
            self._inside_veterinarian = False
        elif name == "pet":
            self.records.append(
                PetRecord(
                    pet_name=self._current_pet["name"],
                    birth_date=date.fromisoformat(self._current_pet["birth_date"]),
                    last_visit_date=date.fromisoformat(self._current_pet["last_visit_date"]),
                    veterinarian=PersonName(
                        self._current_vet["last_name"],
                        self._current_vet["first_name"],
                        self._current_vet["middle_name"],
                    ),
                    diagnosis=self._current_pet["diagnosis"],
                )
            )
        self._current_tag = ""
        self._buffer = []


class PetRecordSaxReader:
    def read(self, file_path: str | Path) -> list[PetRecord]:
        parser = make_parser()
        handler = _PetRecordSaxHandler()
        parser.setContentHandler(handler)
        with Path(file_path).open("r", encoding="utf-8") as stream:
            parser.parse(stream)
        return handler.records
