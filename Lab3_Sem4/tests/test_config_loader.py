"""Tests for ConfigLoader."""
import json
import os
import tempfile
import unittest

from src.utils.config_loader import ConfigLoader


class TestConfigLoader(unittest.TestCase):

    def setUp(self):
        ConfigLoader.clear_cache()
        self._tmpdir = tempfile.mkdtemp()

    def _write_json(self, name: str, data: dict) -> str:
        path = os.path.join(self._tmpdir, name)
        with open(path, "w") as f:
            json.dump(data, f)
        return path

    # -- load JSON ----------------------------------------------------------

    def test_load_json_returns_dict(self):
        path = self._write_json("cfg.json", {"key": "value"})
        data = ConfigLoader.load(path)
        self.assertIsInstance(data, dict)
        self.assertEqual(data["key"], "value")

    def test_load_json_is_cached(self):
        path = self._write_json("cached.json", {"n": 1})
        ConfigLoader.load(path)
        # Overwrite file — cache should still return old value
        with open(path, "w") as f:
            json.dump({"n": 99}, f)
        data = ConfigLoader.load(path)
        self.assertEqual(data["n"], 1)

    def test_clear_cache(self):
        path = self._write_json("clear.json", {"n": 1})
        ConfigLoader.load(path)
        ConfigLoader.clear_cache()
        with open(path, "w") as f:
            json.dump({"n": 99}, f)
        data = ConfigLoader.load(path)
        self.assertEqual(data["n"], 99)

    def test_missing_file_raises(self):
        with self.assertRaises(FileNotFoundError):
            ConfigLoader.load("/nonexistent/path.json")

    def test_unsupported_extension_raises(self):
        path = os.path.join(self._tmpdir, "file.txt")
        open(path, "w").close()
        with self.assertRaises(ValueError):
            ConfigLoader.load(path)

    # -- save_json ----------------------------------------------------------

    def test_save_json_writes_file(self):
        path = os.path.join(self._tmpdir, "out.json")
        ConfigLoader.save_json(path, {"saved": True})
        with open(path) as f:
            data = json.load(f)
        self.assertTrue(data["saved"])

    def test_save_json_updates_cache(self):
        path = os.path.join(self._tmpdir, "cache_update.json")
        ConfigLoader.save_json(path, {"v": 7})
        data = ConfigLoader.load(path)
        self.assertEqual(data["v"], 7)

    def test_load_nested(self):
        path = self._write_json("nested.json", {"a": {"b": [1, 2, 3]}})
        data = ConfigLoader.load(path)
        self.assertEqual(data["a"]["b"], [1, 2, 3])


if __name__ == "__main__":
    unittest.main()
