"""Базовый класс сцен. Каждая сцена обрабатывает события и рисует себя."""
from typing import List, Optional


class Scene:
    """Абстрактная сцена."""

    def __init__(self, app):
        self.app = app
        self.next_scene: Optional[str] = None  # имя следующей сцены или None
        self.done: bool = False  # сигнал переключения

    def handle_events(self, events: List) -> None:
        pass

    def update(self) -> None:
        pass

    def draw(self) -> None:
        pass

    def on_enter(self) -> None:
        """Вызывается при входе в сцену."""
        self.done = False
        self.next_scene = None

    def on_exit(self) -> None:
        """Вызывается при уходе из сцены."""
        pass
