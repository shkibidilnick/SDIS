"""
Enemies — Goomba and Koopa with distinct behaviors.
Each class follows SRP: responsible only for its own AI and rendering.
"""
from __future__ import annotations

from enum import Enum, auto
from typing import List, TYPE_CHECKING
import pygame

from src.entities.entity import Entity
from src.utils.animation import Animation, build_colored_frames

if TYPE_CHECKING:
    from src.utils.asset_manager import AssetManager
    from src.utils.camera import Camera


class EnemyState(Enum):
    WALK = auto()
    DYING = auto()
    DEAD = auto()
    SHELL = auto()       # Koopa only
    SHELL_MOVING = auto()  # Koopa only


class Goomba(Entity):
    """
    Classic mushroom enemy.
    Behavior: walks in one direction, reverses on wall/edge.
    Dies when stomped; damages Mario on side contact.
    """

    _COLOR = (139, 90, 43)
    _STOMP_COLOR = (80, 50, 20)

    def __init__(self, x: int, y: int, cfg: dict,
                 assets: "AssetManager", gravity: float) -> None:
        w = cfg.get("width", 28)
        h = cfg.get("height", 28)
        super().__init__(x, y, w, h)
        self._cfg = cfg
        self._assets = assets
        self._gravity = gravity

        self.vel_x = -cfg.get("speed", 1.0)
        self.state = EnemyState.WALK
        self._death_timer: int = 0
        self._death_duration: int = cfg.get("death_duration", 30)

        self._anim_walk = Animation(
            build_colored_frames(self._COLOR, (w, h), 2,
                                 [self._COLOR, (160, 110, 60)]),
            speed=cfg.get("walk_animation_speed", 10), loop=True
        )

    # ------------------------------------------------------------------ #

    def stomp(self, score_callback) -> None:
        """Called when Mario lands on top of the Goomba."""
        if self.state != EnemyState.WALK:
            return
        self.state = EnemyState.DYING
        self._death_timer = 0
        self.vel_x = 0
        self.vel_y = 0
        self._assets.play_sound("stomp")
        score_callback(self._cfg.get("stomp_score", 100))

    def update(self, tiles: List[pygame.Rect]) -> None:  # type: ignore[override]
        if self.state == EnemyState.DEAD:
            return

        if self.state == EnemyState.DYING:
            self._death_timer += 1
            if self._death_timer >= self._death_duration:
                self.alive = False
                self.state = EnemyState.DEAD
            return

        self.apply_gravity(self._gravity)
        prev_vel_x = self.vel_x
        self.move_x(tiles)
        if self.vel_x == 0 and prev_vel_x != 0:
            self.vel_x = -prev_vel_x  # reverse on wall

        self.move_y(tiles)
        self._anim_walk.update()

    def draw(self, surface: pygame.Surface, camera: "Camera") -> None:
        if self.state == EnemyState.DEAD:
            return
        screen_rect = camera.apply(self.rect)
        if self.state == EnemyState.DYING:
            # Flattened stomp visual
            flat = pygame.Rect(screen_rect.x, screen_rect.bottom - 10,
                               screen_rect.width, 10)
            pygame.draw.rect(surface, self._STOMP_COLOR, flat)
        else:
            frame = self._anim_walk.current_frame
            surface.blit(frame, screen_rect)
            # Eyes
            eye_y = screen_rect.top + 6
            pygame.draw.circle(surface, (255, 255, 255),
                                (screen_rect.left + 7, eye_y), 4)
            pygame.draw.circle(surface, (255, 255, 255),
                                (screen_rect.right - 7, eye_y), 4)
            pygame.draw.circle(surface, (0, 0, 0),
                                (screen_rect.left + 7, eye_y), 2)
            pygame.draw.circle(surface, (0, 0, 0),
                                (screen_rect.right - 7, eye_y), 2)


class Koopa(Entity):
    """
    Turtle enemy.
    Behavior: walks, becomes a sliding shell when stomped.
    Shell can kill other enemies and Mario.
    """

    _BODY_COLOR  = (0, 128, 0)
    _SHELL_COLOR = (0, 100, 0)
    _SHELL_SPEED = 6

    def __init__(self, x: int, y: int, cfg: dict,
                 assets: "AssetManager", gravity: float) -> None:
        w = cfg.get("width", 28)
        h = cfg.get("height", 32)
        super().__init__(x, y, w, h)
        self._cfg = cfg
        self._assets = assets
        self._gravity = gravity
        self._shell_speed: float = cfg.get("shell_speed", self._SHELL_SPEED)
        self._shell_h: int = cfg.get("shell_height", 22)

        self.vel_x = -cfg.get("speed", 1.2)
        self.state = EnemyState.WALK
        self._death_timer: int = 0

        self._anim_walk = Animation(
            build_colored_frames(self._BODY_COLOR, (w, h), 2,
                                 [self._BODY_COLOR, (0, 150, 0)]),
            speed=cfg.get("walk_animation_speed", 12), loop=True
        )

    # ------------------------------------------------------------------ #

    def stomp(self, score_callback, from_right: bool = False) -> None:
        """First stomp → shell; second stomp → launch shell."""
        if self.state == EnemyState.WALK:
            self.state = EnemyState.SHELL
            self.vel_x = 0
            old_bottom = self.rect.bottom
            self.rect.height = self._shell_h
            self.rect.bottom = old_bottom
            self._assets.play_sound("stomp")
            score_callback(self._cfg.get("stomp_score", 200))
        elif self.state == EnemyState.SHELL:
            # Kick the shell
            self.state = EnemyState.SHELL_MOVING
            self.vel_x = self._shell_speed if not from_right else -self._shell_speed
            self._assets.play_sound("kick")

    def kill(self) -> None:
        """Kill by fireball or another shell."""
        self.alive = False
        self.state = EnemyState.DEAD

    def update(self, tiles: List[pygame.Rect]) -> None:  # type: ignore[override]
        if self.state in (EnemyState.DEAD,):
            return

        self.apply_gravity(self._gravity)

        if self.state == EnemyState.WALK:
            prev = self.vel_x
            self.move_x(tiles)
            if self.vel_x == 0 and prev != 0:
                self.vel_x = -prev
            self._anim_walk.update()

        elif self.state == EnemyState.SHELL_MOVING:
            prev = self.vel_x
            self.move_x(tiles)
            if self.vel_x == 0 and prev != 0:
                self.vel_x = -prev  # bounce off walls

        self.move_y(tiles)

    def draw(self, surface: pygame.Surface, camera: "Camera") -> None:
        if self.state == EnemyState.DEAD:
            return
        screen_rect = camera.apply(self.rect)

        if self.state in (EnemyState.SHELL, EnemyState.SHELL_MOVING):
            pygame.draw.rect(surface, self._SHELL_COLOR, screen_rect, border_radius=6)
            pygame.draw.ellipse(surface, (0, 160, 0),
                                screen_rect.inflate(-6, -4))
        else:
            frame = self._anim_walk.current_frame
            surface.blit(frame, screen_rect)
            # Head
            head = pygame.Rect(screen_rect.centerx - 8, screen_rect.top,
                               16, 14)
            pygame.draw.ellipse(surface, (200, 200, 100), head)
            pygame.draw.circle(surface, (0, 0, 0),
                                (screen_rect.centerx + 3, screen_rect.top + 5), 2)
