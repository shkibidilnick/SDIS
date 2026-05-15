"""
ScoresManager — reads and writes high-score table.
Single Responsibility: manage persistence of high scores only.
"""
from __future__ import annotations

import os
from typing import List, Dict

from src.utils.config_loader import ConfigLoader

_SCORES_PATH = os.path.join("configs", "highscores.json")
_MAX_ENTRIES = 10


class ScoresManager:
    """Manages the high-score leaderboard."""

    def __init__(self, path: str = _SCORES_PATH) -> None:
        self._path = path
        self._scores: List[Dict] = self._load()

    # ------------------------------------------------------------------ #
    # Public API                                                           #
    # ------------------------------------------------------------------ #

    @property
    def scores(self) -> List[Dict]:
        """Return a sorted copy of the leaderboard."""
        return sorted(self._scores, key=lambda e: e["score"], reverse=True)

    def is_high_score(self, score: int) -> bool:
        """Return True if *score* qualifies for the leaderboard."""
        if len(self._scores) < _MAX_ENTRIES:
            return True
        return score > min(e["score"] for e in self._scores)

    def add_score(self, name: str, score: int) -> None:
        """Insert a new entry and trim to the top _MAX_ENTRIES."""
        self._scores.append({"name": name[:10].upper(), "score": score})
        self._scores.sort(key=lambda e: e["score"], reverse=True)
        self._scores = self._scores[:_MAX_ENTRIES]
        self._save()

    def top_score(self) -> int:
        """Return the highest recorded score (0 if table is empty)."""
        if not self._scores:
            return 0
        return max(e["score"] for e in self._scores)

    # ------------------------------------------------------------------ #
    # Private helpers                                                      #
    # ------------------------------------------------------------------ #

    def _load(self) -> List[Dict]:
        try:
            ConfigLoader.clear_cache()
            data = ConfigLoader.load(self._path)
            return list(data.get("scores", []))
        except (FileNotFoundError, KeyError):
            return []

    def _save(self) -> None:
        ConfigLoader.save_json(self._path, {"scores": self._scores})
