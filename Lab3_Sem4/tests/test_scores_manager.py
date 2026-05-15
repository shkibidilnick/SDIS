"""Tests for ScoresManager."""
import json
import os
import tempfile
import unittest

from src.utils.scores_manager import ScoresManager
from src.utils.config_loader import ConfigLoader


def _make_manager(initial_scores=None):
    """Create a ScoresManager backed by a temp file."""
    tmpdir = tempfile.mkdtemp()
    path   = os.path.join(tmpdir, "scores.json")
    data   = {"scores": initial_scores or []}
    with open(path, "w") as f:
        json.dump(data, f)
    ConfigLoader.clear_cache()
    return ScoresManager(path=path), path


class TestScoresManager(unittest.TestCase):

    def test_empty_manager_top_score_zero(self):
        mgr, _ = _make_manager()
        self.assertEqual(mgr.top_score(), 0)

    def test_add_score_appears_in_list(self):
        mgr, _ = _make_manager()
        mgr.add_score("MARIO", 5000)
        names = [e["name"] for e in mgr.scores]
        self.assertIn("MARIO", names)

    def test_scores_sorted_descending(self):
        mgr, _ = _make_manager()
        mgr.add_score("A", 100)
        mgr.add_score("B", 500)
        mgr.add_score("C", 300)
        vals = [e["score"] for e in mgr.scores]
        self.assertEqual(vals, sorted(vals, reverse=True))

    def test_is_high_score_empty_table(self):
        mgr, _ = _make_manager()
        self.assertTrue(mgr.is_high_score(1))

    def test_is_high_score_beats_min(self):
        scores = [{"name": f"P{i}", "score": i * 100} for i in range(1, 11)]
        mgr, _ = _make_manager(scores)
        self.assertTrue(mgr.is_high_score(9999))

    def test_is_not_high_score(self):
        scores = [{"name": f"P{i}", "score": 9999} for i in range(10)]
        mgr, _ = _make_manager(scores)
        self.assertFalse(mgr.is_high_score(1))

    def test_top_score(self):
        mgr, _ = _make_manager([{"name": "A", "score": 1234}])
        self.assertEqual(mgr.top_score(), 1234)

    def test_name_truncated_to_10(self):
        mgr, _ = _make_manager()
        mgr.add_score("AVERYLONGNAME", 100)
        name = mgr.scores[0]["name"]
        self.assertLessEqual(len(name), 10)

    def test_name_uppercased(self):
        mgr, _ = _make_manager()
        mgr.add_score("lower", 100)
        self.assertEqual(mgr.scores[0]["name"], "LOWER")

    def test_max_10_entries(self):
        mgr, _ = _make_manager()
        for i in range(15):
            mgr.add_score(f"P{i}", i * 10)
        self.assertLessEqual(len(mgr.scores), 10)

    def test_persistence(self):
        mgr, path = _make_manager()
        mgr.add_score("SAVE", 9999)
        ConfigLoader.clear_cache()
        mgr2 = ScoresManager(path=path)
        names = [e["name"] for e in mgr2.scores]
        self.assertIn("SAVE", names)


if __name__ == "__main__":
    unittest.main()
