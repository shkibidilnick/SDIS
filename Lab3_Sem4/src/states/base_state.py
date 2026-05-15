"""
BaseState — interface every game state must implement.
"""
from __future__ import annotations

from typing import Optional, TYPE_CHECKING
import pygame

if TYPE_CHECKING:
    from src.game import Game


class BaseState:
    """Abstract game state (Menu, Gameplay, GameOver, …)."""

    def __init__(self, game: "Game") -> None:
        self._game = game

    def handle_event(self, event: pygame.event.Event) -> None:
        raise NotImplementedError

    def update(self) -> None:
        raise NotImplementedError

    def draw(self, surface: pygame.Surface) -> None:
        raise NotImplementedError
