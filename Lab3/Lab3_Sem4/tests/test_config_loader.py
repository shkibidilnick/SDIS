"""Тесты для ConfigLoader."""
import json
import os
import tempfile
import unittest

from src.utils.config_loader import ConfigLoader


class TestConfigLoader(unittest.TestCase):
    def setUp(self):
        self.tmpdir = tempfile.TemporaryDirectory()
        self.loader = ConfigLoader(self.tmpdir.name)

    def tearDown(self):
        self.tmpdir.cleanup()

    def _write(self, name, data):
        path = os.path.join(self.tmpdir.name, name)
        with open(path, "w", encoding="utf-8") as f:
            json.dump(data, f)
        return path

    def test_load_existing(self):
        self._write("a.json", {"x": 1})
        self.assertEqual(self.loader.load("a.json"), {"x": 1})

    def test_load_caches(self):
        self._write("a.json", {"v": 1})
        self.loader.load("a.json")
        # Перезапишем файл
        self._write("a.json", {"v": 2})
        # Из кэша вернётся старое значение
        self.assertEqual(self.loader.load("a.json"), {"v": 1})
        # А с use_cache=False — новое
        self.assertEqual(self.loader.load("a.json", use_cache=False), {"v": 2})

    def test_load_missing(self):
        with self.assertRaises(FileNotFoundError):
            self.loader.load("nope.json")

    def test_load_invalid_json(self):
        path = os.path.join(self.tmpdir.name, "bad.json")
        with open(path, "w", encoding="utf-8") as f:
            f.write("{not valid")
        with self.assertRaises(json.JSONDecodeError):
            self.loader.load("bad.json")

    def test_save_and_reload(self):
        self.loader.save("out.json", {"a": [1, 2, 3]})
        # Файл создан
        self.assertTrue(os.path.exists(os.path.join(self.tmpdir.name, "out.json")))
        # Загрузка вернёт ровно сохранённые данные
        self.assertEqual(self.loader.load("out.json", use_cache=False),
                         {"a": [1, 2, 3]})

    def test_save_creates_directories(self):
        self.loader.save("nested/deep/file.json", {"k": "v"})
        path = os.path.join(self.tmpdir.name, "nested", "deep", "file.json")
        self.assertTrue(os.path.exists(path))

    def test_save_updates_cache(self):
        self.loader.save("a.json", {"v": 1})
        self.loader.save("a.json", {"v": 2})
        self.assertEqual(self.loader.load("a.json"), {"v": 2})

    def test_clear_cache(self):
        self._write("a.json", {"v": 1})
        self.loader.load("a.json")
        self.loader.clear_cache()
        self._write("a.json", {"v": 2})
        # После очистки кэша — свежее чтение
        self.assertEqual(self.loader.load("a.json"), {"v": 2})

    def test_absolute_path(self):
        path = self._write("a.json", {"k": 1})
        self.assertEqual(self.loader.load(path), {"k": 1})


if __name__ == "__main__":
    unittest.main()
