"""Базовый класс для физических сущностей с гравитацией и коллизиями."""
from typing import TYPE_CHECKING

from ..utils.geometry import Rect

if TYPE_CHECKING:
    from .level import Level


class PhysicsEntity:
    """Объект, на который действует гравитация и который сталкивается с тайлами.

    Содержит общий код для игрока и врагов (DRY).
    """

    def __init__(self, x: float, y: float, w: float, h: float):
        self.rect = Rect(x, y, w, h)
        self.vx: float = 0.0
        self.vy: float = 0.0
        self.on_ground: bool = False
        self.alive: bool = True

    def apply_gravity(self, gravity: float, max_fall_speed: float) -> None:
        """Применить гравитацию с ограничением скорости падения."""
        self.vy += gravity
        if self.vy > max_fall_speed:
            self.vy = max_fall_speed

    def move_and_collide(self, level: "Level") -> dict:
        """Сдвинуть сущность с учётом столкновений с твёрдыми тайлами.

        Возвращает словарь с информацией о столкновениях:
            {"hit_top": bool, "hit_bottom": bool, "hit_left": bool, "hit_right": bool,
             "tiles_hit_top": [(tx, ty), ...], ...}
        """
        result = {
            "hit_top": False, "hit_bottom": False,
            "hit_left": False, "hit_right": False,
            "tiles_hit_top": [], "tiles_hit_bottom": [],
            "tiles_hit_left": [], "tiles_hit_right": [],
        }

        # горизонтальное движение
        self.rect.x += self.vx
        for tx, ty in level.solid_tiles_in_rect(self.rect):
            tile_rect = level.tile_rect(tx, ty)
            if not self.rect.intersects(tile_rect):
                continue
            if self.vx > 0:
                self.rect.x = tile_rect.left - self.rect.w
                result["hit_right"] = True
                result["tiles_hit_right"].append((tx, ty))
                self.vx = 0
            elif self.vx < 0:
                self.rect.x = tile_rect.right
                result["hit_left"] = True
                result["tiles_hit_left"].append((tx, ty))
                self.vx = 0

        # вертикальное движение
        self.rect.y += self.vy
        self.on_ground = False
        for tx, ty in level.solid_tiles_in_rect(self.rect):
            tile_rect = level.tile_rect(tx, ty)
            if not self.rect.intersects(tile_rect):
                continue
            if self.vy > 0:
                self.rect.y = tile_rect.top - self.rect.h
                result["hit_bottom"] = True
                result["tiles_hit_bottom"].append((tx, ty))
                self.vy = 0
                self.on_ground = True
            elif self.vy < 0:
                self.rect.y = tile_rect.bottom
                result["hit_top"] = True
                result["tiles_hit_top"].append((tx, ty))
                self.vy = 0

        return result
