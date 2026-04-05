from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from app.xmlio.dom_writer import PetRecordDomWriter
from app.xmlio.sax_reader import PetRecordSaxReader
from tests.conftest import build_sample_record, build_second_record


class XmlIoTests(unittest.TestCase):
    def test_dom_writer_and_sax_reader_roundtrip(self) -> None:
        with tempfile.TemporaryDirectory() as tmp_dir:
            file_path = Path(tmp_dir) / "pets.xml"
            writer = PetRecordDomWriter()
            reader = PetRecordSaxReader()

            sample_record = build_sample_record()
            second_record = build_second_record()

            writer.write([sample_record, second_record], file_path)
            loaded = reader.read(file_path)

            self.assertEqual(2, len(loaded))
            self.assertEqual(sample_record.pet_name, loaded[0].pet_name)
            self.assertEqual(second_record.diagnosis, loaded[1].diagnosis)


if __name__ == "__main__":
    unittest.main()