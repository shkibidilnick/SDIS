"""
Game — application entry point and state-machine manager.
Single Responsibility: window lifecycle and state routing only.
"""
from __future__ import annotations

import os
import sys
from typing import Dict, Any
import pygame

from src.utils.config_loader import ConfigLoader
from src.utils.asset_manager import AssetManager
from src.utils.scores_manager import ScoresManager


_CONFIGS_DIR = "configs"


class Game:
    """
    Top-level game object.
    Owns the display, clock, asset manager, scores manager and the state stack.
    """

    def __init__(self) -> None:
        self.settings: Dict[str, Any] = ConfigLoader.load(
            os.path.join(_CONFIGS_DIR, "settings.json"))

        win = self.settings["window"]
        self._screen = pygame.display.set_mode(
            (win["width"], win["height"]))
        pygame.display.set_caption(win["title"])
        self._clock = pygame.time.Clock()
        self._fps   = win.get("fps", 60)
        self._running = True

        self.assets        = AssetManager()
        self.scores_manager = ScoresManager()

        # State stack
        self._states = []
        self.push_state("menu")

    # ------------------------------------------------------------------ #
    # Public API                                                           #
    # ------------------------------------------------------------------ #

    def run(self) -> None:
        """Main loop."""
        while self._running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.quit()
                elif self._states:
                    self._states[-1].handle_event(event)

            if self._states:
                self._states[-1].update()
                self._states[-1].draw(self._screen)

            pygame.display.flip()
            self._clock.tick(self._fps)

    def push_state(self, name: str, **kwargs) -> None:
        state = self._create_state(name, **kwargs)
        self._states.append(state)

    def pop_state(self) -> None:
        if self._states:
            self._states.pop()

    def quit(self) -> None:
        self._running = False

    # ------------------------------------------------------------------ #
    # Private                                                              #
    # ------------------------------------------------------------------ #

    def _create_state(self, name: str, **kwargs):
        # Lazy imports to avoid circular dependencies
        if name == "menu":
            from src.states.menu_state import MenuState
            return MenuState(self)
        if name == "gameplay":
            from src.states.gameplay_state import GameplayState
            return GameplayState(self)
        if name == "highscores":
            from src.states.highscores_state import HighScoresState
            return HighScoresState(self)
        if name == "help":
            from src.states.help_state import HelpState
            return HelpState(self)
        if name == "new_record":
            from src.states.new_record_state import NewRecordState
            return NewRecordState(self, kwargs.get("score", 0))
        if name == "gameover":
            from src.states.gameover_state import GameOverState
            return GameOverState(self, kwargs.get("score", 0))
        raise ValueError(f"Unknown state: {name!r}")
