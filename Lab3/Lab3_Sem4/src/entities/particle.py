"""Модуль частиц для анимаций (искры монет, осколки кирпичей, пыль)."""
from typing import Tuple


class Particle:
    """Простая частица с физикой."""

    def __init__(self, x: float, y: float, vx: float, vy: float,
                 color: Tuple[int, int, int], lifetime: int = 30,
                 size: int = 3, gravity: float = 0.3):
        self.x = x
        self.y = y
        self.vx = vx
        self.vy = vy
        self.color = color
        self.lifetime = lifetime
        self.max_lifetime = lifetime
        self.size = size
        self.gravity = gravity
        self.alive = True

    def update(self) -> None:
        if not self.alive:
            return
        self.vy += self.gravity
        self.x += self.vx
        self.y += self.vy
        self.lifetime -= 1
        if self.lifetime <= 0:
            self.alive = False

    @property
    def alpha(self) -> int:
        """Прозрачность как функция оставшейся жизни."""
        if self.max_lifetime <= 0:
            return 0
        return max(0, min(255, int(255 * self.lifetime / self.max_lifetime)))
