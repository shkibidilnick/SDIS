"""Тесты для HighScoreManager."""
import json
import os
import tempfile
import unittest

from src.utils.config_loader import ConfigLoader
from src.utils.highscore_manager import HighScoreManager


class TestHighScoreManager(unittest.TestCase):
    def setUp(self):
        self.tmpdir = tempfile.TemporaryDirectory()
        self.loader = ConfigLoader(self.tmpdir.name)
        self.path = "scores.json"

    def tearDown(self):
        self.tmpdir.cleanup()

    def _write(self, scores):
        with open(os.path.join(self.tmpdir.name, self.path), "w") as f:
            json.dump({"scores": scores}, f)

    def test_load_existing_scores(self):
        self._write([{"name": "AAA", "score": 1000},
                     {"name": "BBB", "score": 500}])
        m = HighScoreManager(self.path, self.loader)
        scores = m.get_scores()
        self.assertEqual(len(scores), 2)
        self.assertEqual(scores[0]["score"], 1000)

    def test_load_missing_file(self):
        m = HighScoreManager(self.path, self.loader)
        self.assertEqual(m.get_scores(), [])

    def test_load_corrupted(self):
        with open(os.path.join(self.tmpdir.name, self.path), "w") as f:
            f.write("not json")
        m = HighScoreManager(self.path, self.loader)
        self.assertEqual(m.get_scores(), [])

    def test_top_score_empty(self):
        m = HighScoreManager(self.path, self.loader)
        self.assertEqual(m.get_top_score(), 0)

    def test_top_score(self):
        self._write([{"name": "A", "score": 50},
                     {"name": "B", "score": 200}])
        m = HighScoreManager(self.path, self.loader)
        self.assertEqual(m.get_top_score(), 200)

    def test_is_high_score(self):
        """Проверка попадания в топ-5 при разном заполнении таблицы."""
        # 1. Пустая таблица: любой ненулевой score попадает
        m = HighScoreManager(self.path, self.loader)
        m._scores = []
        self.assertTrue(m.is_high_score(100))
        self.assertTrue(m.is_high_score(1))
        self.assertFalse(m.is_high_score(0))
        self.assertFalse(m.is_high_score(-50))

        # 2. Неполная таблица (меньше 5 записей): любой ненулевой попадает
        m._scores = [
            {"name": "AAA", "score": 5000},
            {"name": "BBB", "score": 3000},
        ]
        self.assertTrue(m.is_high_score(100))  # хуже всех, но мест ещё много
        self.assertTrue(m.is_high_score(10000))  # лучший
        self.assertFalse(m.is_high_score(0))

        # 3. Полная таблица (ровно 5 записей): нужно быть строго выше последнего
        m._scores = [
            {"name": "AAA", "score": 5000},
            {"name": "BBB", "score": 4000},
            {"name": "CCC", "score": 3000},
            {"name": "DDD", "score": 2000},
            {"name": "EEE", "score": 1000},
        ]
        self.assertTrue(m.is_high_score(10000))  # лучший
        self.assertTrue(m.is_high_score(1500))  # вытесняет 1000
        self.assertFalse(m.is_high_score(1000))  # равен — не вытесняет
        self.assertFalse(m.is_high_score(500))  # хуже последнего

    def test_is_high_score_when_empty(self):
        m = HighScoreManager(self.path, self.loader)
        self.assertTrue(m.is_high_score(1))
        # 0 не больше 0 — не рекорд
        self.assertFalse(m.is_high_score(0))

    def test_add_score_sorts_and_limits(self):
        m = HighScoreManager(self.path, self.loader)
        for i in range(7):
            m.add_score(f"P{i}", i * 100)
        scores = m.get_scores()
        self.assertEqual(len(scores), HighScoreManager.MAX_ENTRIES)
        # отсортировано по убыванию
        for i in range(len(scores) - 1):
            self.assertGreaterEqual(scores[i]["score"], scores[i + 1]["score"])

    def test_add_score_persists(self):
        m = HighScoreManager(self.path, self.loader)
        m.add_score("HERO", 9999)
        # Проверяем, что файл создался и содержит данные
        path = os.path.join(self.tmpdir.name, self.path)
        with open(path) as f:
            data = json.load(f)
        self.assertEqual(data["scores"][0]["name"], "HERO")
        self.assertEqual(data["scores"][0]["score"], 9999)

    def test_add_score_uppercase_and_truncate(self):
        m = HighScoreManager(self.path, self.loader)
        m.add_score("verylongname", 100)
        scores = m.get_scores()
        self.assertEqual(scores[0]["name"], "VERYLONGNA")  # 10 символов

    def test_add_score_empty_name(self):
        m = HighScoreManager(self.path, self.loader)
        m.add_score("", 100)
        m.add_score("   ", 50)
        scores = m.get_scores()
        for s in scores:
            self.assertEqual(s["name"], "ANON")

    def test_reload(self):
        self._write([{"name": "A", "score": 100}])
        m = HighScoreManager(self.path, self.loader)
        # Внешне меняем файл
        self._write([{"name": "B", "score": 999}])
        m.reload()
        self.assertEqual(m.get_top_score(), 999)


if __name__ == "__main__":
    unittest.main()
