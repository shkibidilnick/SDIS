"""Модуль загрузки конфигурационных файлов в формате JSON.

Соблюдает SRP: единственная обязанность — чтение конфигов с диска.
"""
import json
import os
from typing import Any, Dict


class ConfigLoader:
    """Загружает и кэширует JSON-конфиги."""

    def __init__(self, base_path: str = ""):
        """:param base_path: базовый путь для относительных файлов конфигов."""
        self.base_path = base_path
        self._cache: Dict[str, Any] = {}

    def load(self, path: str, use_cache: bool = True) -> Dict[str, Any]:
        """Загрузить JSON-файл по пути относительно base_path или абсолютному.

        :param path: путь к файлу
        :param use_cache: использовать ли кэш (по умолчанию да)
        :return: словарь с данными
        :raises FileNotFoundError: если файл не существует
        :raises json.JSONDecodeError: если файл повреждён
        """
        full_path = path if os.path.isabs(path) else os.path.join(self.base_path, path)
        if use_cache and full_path in self._cache:
            return self._cache[full_path]
        with open(full_path, "r", encoding="utf-8") as f:
            data = json.load(f)
        self._cache[full_path] = data
        return data

    def save(self, path: str, data: Dict[str, Any]) -> None:
        """Сохранить словарь в JSON-файл.

        :param path: путь к файлу
        :param data: данные для сохранения
        """
        full_path = path if os.path.isabs(path) else os.path.join(self.base_path, path)
        os.makedirs(os.path.dirname(full_path) or ".", exist_ok=True)
        with open(full_path, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=2, ensure_ascii=False)
        # обновим кэш
        self._cache[full_path] = data

    def clear_cache(self) -> None:
        """Очистить внутренний кэш."""
        self._cache.clear()
