"""
Entity — base class for all game objects.
Provides rect, velocity, alive flag, and basic physics step.
"""
from __future__ import annotations

import pygame
from typing import List


class Entity:
    """Abstract base for every game entity (player, enemy, item)."""

    def __init__(self, x: int, y: int, width: int, height: int) -> None:
        self.rect = pygame.Rect(x, y, width, height)
        self.vel_x: float = 0.0
        self.vel_y: float = 0.0
        self.alive: bool = True
        self.on_ground: bool = False

    # ------------------------------------------------------------------ #
    # Physics helpers (used by subclasses)                                 #
    # ------------------------------------------------------------------ #

    def apply_gravity(self, gravity: float, max_fall: float = 15.0) -> None:
        self.vel_y = min(self.vel_y + gravity, max_fall)

    def move_x(self, tiles: List[pygame.Rect]) -> None:
        """Move horizontally and resolve tile collisions."""
        self.rect.x += int(self.vel_x)
        for tile in tiles:
            if self.rect.colliderect(tile):
                if self.vel_x > 0:
                    self.rect.right = tile.left
                elif self.vel_x < 0:
                    self.rect.left = tile.right
                self.vel_x = 0

    def move_y(self, tiles: List[pygame.Rect]) -> None:
        """Move vertically and resolve tile collisions."""
        self.rect.y += int(self.vel_y)
        self.on_ground = False
        for tile in tiles:
            if self.rect.colliderect(tile):
                if self.vel_y > 0:
                    self.rect.bottom = tile.top
                    self.on_ground = True
                elif self.vel_y < 0:
                    self.rect.top = tile.bottom
                self.vel_y = 0

    # ------------------------------------------------------------------ #
    # Interface                                                            #
    # ------------------------------------------------------------------ #

    def update(self, *args, **kwargs) -> None:
        raise NotImplementedError

    def draw(self, surface: pygame.Surface, camera) -> None:
        raise NotImplementedError
