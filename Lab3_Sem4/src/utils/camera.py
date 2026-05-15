"""
Camera — tracks the player and provides a world-to-screen offset.
Single Responsibility: compute the horizontal scroll offset only.
"""
from __future__ import annotations

import pygame
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from src.utils.config_loader import ConfigLoader


class Camera:
    """Horizontal scrolling camera that follows the player."""

    def __init__(self, world_width: int, screen_width: int,
                 screen_height: int, settings: dict) -> None:
        self._world_width = world_width
        self._screen_width = screen_width
        self._screen_height = screen_height

        cfg = settings.get("camera", {})
        self._threshold_right: int = cfg.get("scroll_threshold_right", 400)
        self._threshold_left: int = cfg.get("scroll_threshold_left", 200)

        self.offset_x: int = 0  # pixels the world is shifted left

    # ------------------------------------------------------------------ #
    # Public API                                                           #
    # ------------------------------------------------------------------ #

    def update(self, player_rect: pygame.Rect) -> None:
        """Adjust offset so the player stays within thresholds."""
        player_screen_x = player_rect.centerx - self.offset_x

        if player_screen_x > self._threshold_right:
            self.offset_x = player_rect.centerx - self._threshold_right
        elif player_screen_x < self._threshold_left:
            self.offset_x = player_rect.centerx - self._threshold_left

        # Clamp to world bounds
        max_offset = self._world_width - self._screen_width
        self.offset_x = max(0, min(self.offset_x, max_offset))

    def apply(self, rect: pygame.Rect) -> pygame.Rect:
        """Return the screen-space rect for a world-space rect."""
        return rect.move(-self.offset_x, 0)

    def reset(self) -> None:
        self.offset_x = 0
