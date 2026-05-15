"""
Animation — frame-based sprite animation.
Single Responsibility: advance frames and return the current frame surface.
"""
from __future__ import annotations

from typing import List, Optional
import pygame


class Animation:
    """Cycles through a list of surfaces at a given frame rate."""

    def __init__(self, frames: List[pygame.Surface], speed: int = 8,
                 loop: bool = True) -> None:
        """
        Args:
            frames: list of surfaces in animation order.
            speed:  ticks per frame (lower = faster).
            loop:   if False, stop on last frame.
        """
        self._frames = frames
        self._speed = max(1, speed)
        self._loop = loop
        self._tick = 0
        self._index = 0
        self.finished = False

    # ------------------------------------------------------------------ #
    # Public API                                                           #
    # ------------------------------------------------------------------ #

    def update(self) -> None:
        """Advance the animation by one game tick."""
        if self.finished:
            return
        self._tick += 1
        if self._tick >= self._speed:
            self._tick = 0
            self._index += 1
            if self._index >= len(self._frames):
                if self._loop:
                    self._index = 0
                else:
                    self._index = len(self._frames) - 1
                    self.finished = True

    @property
    def current_frame(self) -> pygame.Surface:
        return self._frames[self._index]

    def reset(self) -> None:
        self._tick = 0
        self._index = 0
        self.finished = False


def build_colored_frames(color: tuple, size: tuple, count: int,
                         variants: Optional[List[tuple]] = None
                         ) -> List[pygame.Surface]:
    """
    Build *count* simple placeholder surfaces.
    If *variants* is given, each frame uses its own color.
    """
    frames: List[pygame.Surface] = []
    for i in range(count):
        surf = pygame.Surface(size, pygame.SRCALPHA)
        c = variants[i] if (variants and i < len(variants)) else color
        surf.fill(c)
        frames.append(surf)
    return frames
