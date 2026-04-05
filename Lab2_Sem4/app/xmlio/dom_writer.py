from __future__ import annotations

from pathlib import Path
from xml.dom.minidom import Document

from app.domain.models import PetRecord


class PetRecordDomWriter:
    def write(self, records: list[PetRecord], file_path: str | Path) -> None:
        document = Document()
        root = document.createElement("pets")
        document.appendChild(root)

        for record in records:
            pet_node = document.createElement("pet")
            root.appendChild(pet_node)

            self._append_text_node(document, pet_node, "name", record.pet_name)
            self._append_text_node(document, pet_node, "birth_date", record.birth_date.isoformat())
            self._append_text_node(document, pet_node, "last_visit_date", record.last_visit_date.isoformat())

            veterinarian_node = document.createElement("veterinarian")
            pet_node.appendChild(veterinarian_node)
            self._append_text_node(document, veterinarian_node, "last_name", record.veterinarian.last_name)
            self._append_text_node(document, veterinarian_node, "first_name", record.veterinarian.first_name)
            self._append_text_node(document, veterinarian_node, "middle_name", record.veterinarian.middle_name)

            self._append_text_node(document, pet_node, "diagnosis", record.diagnosis)

        Path(file_path).write_text(document.toprettyxml(indent="  ", encoding=None), encoding="utf-8")

    @staticmethod
    def _append_text_node(document: Document, parent, tag_name: str, value: str) -> None:
        node = document.createElement(tag_name)
        node.appendChild(document.createTextNode(value))
        parent.appendChild(node)
