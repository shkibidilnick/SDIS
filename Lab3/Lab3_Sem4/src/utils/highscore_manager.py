"""Модуль работы с таблицей рекордов.

Соблюдает SRP: только управление таблицей рекордов (загрузка, сохранение,
проверка нового рекорда).
"""
from typing import Dict, List

from .config_loader import ConfigLoader


class HighScoreManager:
    """Управляет таблицей рекордов: загружает, сортирует, добавляет."""

    MAX_ENTRIES = 5

    def __init__(self, file_path: str, loader: ConfigLoader):
        """:param file_path: путь к JSON-файлу рекордов.
        :param loader: загрузчик конфигов (внедрение зависимости).
        """
        self.file_path = file_path
        self.loader = loader
        self._scores: List[Dict] = []
        self.reload()

    def reload(self) -> None:
        """Перечитать рекорды с диска. Если файл повреждён — начать с пустого."""
        try:
            data = self.loader.load(self.file_path, use_cache=False)
            self._scores = data.get("scores", [])
        except (FileNotFoundError, ValueError):
            self._scores = []
        self._scores.sort(key=lambda x: x.get("score", 0), reverse=True)

    def get_scores(self) -> List[Dict]:
        """Вернуть список рекордов (отсортированный по убыванию)."""
        return list(self._scores)

    def get_top_score(self) -> int:
        """Вернуть наивысший результат (0, если таблица пуста)."""
        if not self._scores:
            return 0
        return self._scores[0].get("score", 0)

    def is_high_score(self, score: int) -> bool:
        """Проверить, попадает ли результат в топ-MAX_ENTRIES.

        Условия:
        - score должен быть строго больше 0;
        - если в таблице меньше MAX_ENTRIES записей — попадает любой ненулевой;
        - иначе — должен быть строго больше score'а на последней позиции.
        """
        if score <= 0:
            return False
        if len(self._scores) < self.MAX_ENTRIES:
            return True
        return score > self._scores[-1].get("score", 0)

    def add_score(self, name: str, score: int) -> None:
        """Добавить новый рекорд и сохранить файл.

        Имя обрезается до 10 символов и приводится к верхнему регистру.
        В таблице остаётся не более MAX_ENTRIES записей.
        """
        name = (name or "ANON").strip()[:10].upper() or "ANON"
        self._scores.append({"name": name, "score": int(score)})
        self._scores.sort(key=lambda x: x.get("score", 0), reverse=True)
        self._scores = self._scores[: self.MAX_ENTRIES]
        self._save()

    def _save(self) -> None:
        """Сохранить таблицу на диск."""
        self.loader.save(self.file_path, {"scores": self._scores})
