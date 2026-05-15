"""
Bonuses — collectible power-ups: Mushroom, FireFlower, Star, Coin.
Each bonus type is a separate class (SRP).
"""
from __future__ import annotations

from typing import TYPE_CHECKING
import pygame

from src.entities.entity import Entity
from src.utils.animation import Animation, build_colored_frames

if TYPE_CHECKING:
    from src.utils.asset_manager import AssetManager
    from src.utils.camera import Camera


class Bonus(Entity):
    """Abstract base for collectible bonuses."""

    def __init__(self, x: int, y: int, width: int, height: int,
                 effect: str, assets: "AssetManager") -> None:
        super().__init__(x, y, width, height)
        self.effect = effect
        self._assets = assets
        self.collected = False
        self._emerge_timer = 0
        self._emerge_duration = 30   # frames to pop out of block
        self._emerge_y = y           # final y after emerging
        self.rect.y = y + height     # start hidden below block
        self._emerging = True

    def collect(self, mario) -> None:
        """Apply effect to Mario and mark as collected."""
        raise NotImplementedError

    def update(self, tiles) -> None:  # type: ignore[override]
        if self.collected:
            return
        if self._emerging:
            self._emerge_timer += 1
            progress = self._emerge_timer / self._emerge_duration
            self.rect.y = int(self._emerge_y + self.rect.height * (1 - progress))
            if self._emerge_timer >= self._emerge_duration:
                self.rect.y = self._emerge_y
                self._emerging = False
            return
        self._update_active(tiles)

    def _update_active(self, tiles) -> None:
        pass

    def draw(self, surface: pygame.Surface, camera: "Camera") -> None:
        raise NotImplementedError


class Mushroom(Bonus):
    """Red mushroom — makes Mario grow."""

    _COLOR = (220, 30, 30)
    _SIZE = (26, 26)

    def __init__(self, x: int, y: int, assets: "AssetManager",
                 gravity: float = 0.5) -> None:
        super().__init__(x, y, *self._SIZE, "grow", assets)
        self._gravity = gravity
        self.vel_x = 1.5
        self._anim = Animation(
            build_colored_frames(self._COLOR, self._SIZE, 2,
                                 [self._COLOR, (180, 20, 20)]),
            speed=12, loop=True
        )

    def collect(self, mario) -> None:
        mario.power_up("grow")
        self.collected = True
        self.alive = False

    def _update_active(self, tiles) -> None:
        self.apply_gravity(self._gravity)
        prev = self.vel_x
        self.move_x(tiles)
        if self.vel_x == 0 and prev != 0:
            self.vel_x = -prev
        self.move_y(tiles)
        self._anim.update()

    def draw(self, surface: pygame.Surface, camera: "Camera") -> None:
        if self.collected:
            return
        sr = camera.apply(self.rect)
        frame = self._anim.current_frame
        surface.blit(frame, sr)
        # Mushroom cap dots
        pygame.draw.circle(surface, (255, 255, 255),
                            (sr.centerx - 5, sr.top + 6), 3)
        pygame.draw.circle(surface, (255, 255, 255),
                            (sr.centerx + 5, sr.top + 6), 3)


class FireFlower(Bonus):
    """Fire flower — gives fire ability."""

    _COLOR = (255, 100, 0)
    _SIZE = (24, 26)

    def __init__(self, x: int, y: int, assets: "AssetManager") -> None:
        super().__init__(x, y, *self._SIZE, "fire", assets)
        colors = [(255, 100, 0), (255, 200, 0), (255, 50, 50), (200, 80, 0)]
        self._anim = Animation(
            build_colored_frames(self._COLOR, self._SIZE, 4, colors),
            speed=8, loop=True
        )

    def collect(self, mario) -> None:
        mario.power_up("fire")
        self.collected = True
        self.alive = False

    def _update_active(self, tiles) -> None:
        self._anim.update()

    def draw(self, surface: pygame.Surface, camera: "Camera") -> None:
        if self.collected:
            return
        sr = camera.apply(self.rect)
        frame = self._anim.current_frame
        surface.blit(frame, sr)
        # Stem
        pygame.draw.line(surface, (0, 180, 0),
                         (sr.centerx, sr.bottom),
                         (sr.centerx, sr.bottom - 10), 3)


class Star(Bonus):
    """Blinking star — grants temporary invincibility."""

    _COLOR = (255, 220, 0)
    _SIZE = (26, 26)

    def __init__(self, x: int, y: int, assets: "AssetManager",
                 gravity: float = 0.5) -> None:
        super().__init__(x, y, *self._SIZE, "invincible", assets)
        self._gravity = gravity
        self.vel_x = 2.0
        colors = [(255, 220, 0), (255, 255, 150), (255, 200, 0)]
        self._anim = Animation(
            build_colored_frames(self._COLOR, self._SIZE, 3, colors),
            speed=6, loop=True
        )

    def collect(self, mario) -> None:
        mario.power_up("invincible")
        self.collected = True
        self.alive = False

    def _update_active(self, tiles) -> None:
        self.apply_gravity(self._gravity)
        prev = self.vel_x
        self.move_x(tiles)
        if self.vel_x == 0 and prev != 0:
            self.vel_x = -prev
        self.move_y(tiles)
        self._anim.update()

    def draw(self, surface: pygame.Surface, camera: "Camera") -> None:
        if self.collected:
            return
        sr = camera.apply(self.rect)
        frame = self._anim.current_frame
        surface.blit(frame, sr)
        # Draw a simple star shape
        cx, cy = sr.centerx, sr.centery
        points = []
        import math
        for i in range(5):
            angle = math.pi / 2 + i * 2 * math.pi / 5
            points.append((cx + 10 * math.cos(angle),
                            cy - 10 * math.sin(angle)))
            angle2 = angle + math.pi / 5
            points.append((cx + 4 * math.cos(angle2),
                            cy - 4 * math.sin(angle2)))
        pygame.draw.polygon(surface, (255, 255, 200), points)


class Coin(Bonus):
    """Floating coin — adds score and counts toward extra life."""

    _COLOR = (255, 220, 0)
    _SIZE = (18, 22)

    def __init__(self, x: int, y: int, assets: "AssetManager") -> None:
        # Coins don't emerge — they sit in place
        super().__init__(x, y, *self._SIZE, "coin", assets)
        self.rect.y = y   # override parent positioning
        self._emerging = False
        colors = [(255, 220, 0), (255, 255, 100), (200, 170, 0)]
        self._anim = Animation(
            build_colored_frames(self._COLOR, self._SIZE, 3, colors),
            speed=10, loop=True
        )

    def collect(self, mario) -> None:
        mario.add_coin()
        self.collected = True
        self.alive = False

    def _update_active(self, tiles) -> None:
        self._anim.update()

    def draw(self, surface: pygame.Surface, camera: "Camera") -> None:
        if self.collected:
            return
        sr = camera.apply(self.rect)
        frame = self._anim.current_frame
        surface.blit(frame, sr)
        pygame.draw.ellipse(surface, (255, 200, 0), sr.inflate(-6, -2))
