"""Простая AABB-коллизия (Axis-Aligned Bounding Box).

Не зависит от pygame, чтобы можно было покрыть unit-тестами без библиотеки.
"""
from typing import Tuple


class Rect:
    """Минимальная реализация прямоугольника для коллизий и тестов."""

    __slots__ = ("x", "y", "w", "h")

    def __init__(self, x: float, y: float, w: float, h: float):
        self.x = float(x)
        self.y = float(y)
        self.w = float(w)
        self.h = float(h)

    @property
    def left(self) -> float:
        return self.x

    @property
    def right(self) -> float:
        return self.x + self.w

    @property
    def top(self) -> float:
        return self.y

    @property
    def bottom(self) -> float:
        return self.y + self.h

    @property
    def center(self) -> Tuple[float, float]:
        return (self.x + self.w / 2, self.y + self.h / 2)

    def intersects(self, other: "Rect") -> bool:
        """Пересекаются ли два прямоугольника."""
        return not (self.right <= other.left
                    or self.left >= other.right
                    or self.bottom <= other.top
                    or self.top >= other.bottom)

    def contains_point(self, px: float, py: float) -> bool:
        """Содержит ли прямоугольник точку."""
        return self.left <= px < self.right and self.top <= py < self.bottom

    def copy(self) -> "Rect":
        return Rect(self.x, self.y, self.w, self.h)

    def __repr__(self) -> str:
        return f"Rect({self.x}, {self.y}, {self.w}, {self.h})"

    def __eq__(self, other) -> bool:
        if not isinstance(other, Rect):
            return False
        return (self.x == other.x and self.y == other.y
                and self.w == other.w and self.h == other.h)
