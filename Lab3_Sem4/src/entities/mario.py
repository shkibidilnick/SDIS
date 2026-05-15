"""
Mario — the player-controlled entity.
Handles input, physics, state (small/big/fire), animations.
"""
from __future__ import annotations

from enum import Enum, auto
from typing import List, Optional, TYPE_CHECKING
import pygame

from src.entities.entity import Entity
from src.utils.animation import Animation, build_colored_frames

if TYPE_CHECKING:
    from src.utils.asset_manager import AssetManager
    from src.utils.camera import Camera


class MarioState(Enum):
    SMALL = auto()
    BIG = auto()
    FIRE = auto()


class Mario(Entity):
    """Player-controlled Mario with full SMB movement and power-ups."""

    # Pixel sizes per state
    _SIZES = {
        MarioState.SMALL: (24, 28),
        MarioState.BIG:   (24, 48),
        MarioState.FIRE:  (24, 48),
    }

    def __init__(self, x: int, y: int, settings: dict,
                 assets: "AssetManager") -> None:
        cfg = settings.get("mario", {})
        self._cfg = cfg
        w, h = self._SIZES[MarioState.SMALL]
        super().__init__(x, y, w, h)

        self._assets = assets
        self.state = MarioState.SMALL
        self.lives: int = cfg.get("lives", 3)
        self.score: int = 0
        self.coins: int = 0

        # Movement params
        self._speed: float = cfg.get("speed", 3)
        self._run_speed: float = cfg.get("run_speed", 5)
        self._jump_force: float = cfg.get("jump_force", -12)

        # State flags
        self.invincible: bool = False
        self._inv_timer: int = 0
        self._inv_duration: int = cfg.get("invincibility_duration", 120)
        self._inv_blink: int = 0

        self._dead: bool = False
        self._dead_vel: float = -12.0
        self._dead_timer: int = 0

        self.facing_right: bool = True
        self._running: bool = False

        # Fire cooldown
        self._fire_cooldown: int = 0
        self.fireballs: List["Fireball"] = []

        # Animations
        self._animations: dict[str, Animation] = {}
        self._build_animations()
        self._anim_key: str = "idle_small"

        # Gravity reference (set by GameplayState)
        self._gravity: float = settings.get("gravity", 0.5)

    # ------------------------------------------------------------------ #
    # Public API                                                           #
    # ------------------------------------------------------------------ #

    @property
    def is_dead(self) -> bool:
        return self._dead

    @property
    def dead_finished(self) -> bool:
        """True when the death animation is fully over."""
        return self._dead and self._dead_timer > 150

    def handle_input(self, keys: pygame.key.ScancodeWrapper) -> None:
        if self._dead:
            return

        self._running = keys[pygame.K_LSHIFT] or keys[pygame.K_RSHIFT]
        spd = self._run_speed if self._running else self._speed

        if keys[pygame.K_LEFT] or keys[pygame.K_a]:
            self.vel_x = -spd
            self.facing_right = False
        elif keys[pygame.K_RIGHT] or keys[pygame.K_d]:
            self.vel_x = spd
            self.facing_right = True
        else:
            # Decelerate
            self.vel_x *= 0.8
            if abs(self.vel_x) < 0.3:
                self.vel_x = 0

        if (keys[pygame.K_SPACE] or keys[pygame.K_UP] or keys[pygame.K_w]) \
                and self.on_ground:
            self.vel_y = self._jump_force
            self.on_ground = False
            self._assets.play_sound("jump")

        if self._fire_cooldown > 0:
            self._fire_cooldown -= 1

        if keys[pygame.K_z] and self.state == MarioState.FIRE \
                and self._fire_cooldown == 0:
            self._shoot_fireball()
            self._fire_cooldown = 25

    def update(self, tiles: List[pygame.Rect]) -> None:  # type: ignore[override]
        if self._dead:
            self._update_death()
            return

        self.apply_gravity(self._gravity)
        self.move_x(tiles)
        self.move_y(tiles)

        # Invincibility countdown
        if self.invincible:
            self._inv_timer -= 1
            self._inv_blink += 1
            if self._inv_timer <= 0:
                self.invincible = False

        # Update fireballs
        for fb in self.fireballs[:]:
            fb.update(tiles)
            if not fb.alive:
                self.fireballs.remove(fb)

        self._update_animation()

    def draw(self, surface: pygame.Surface, camera: "Camera") -> None:
        # Blink when invincible
        if self.invincible and self._inv_blink % 6 < 3:
            return  # skip frame for blink effect

        anim = self._animations.get(self._anim_key)
        if anim:
            frame = anim.current_frame
            if not self.facing_right:
                frame = pygame.transform.flip(frame, True, False)
            surface.blit(frame, camera.apply(self.rect))
        else:
            pygame.draw.rect(surface, (220, 30, 30), camera.apply(self.rect))

        for fb in self.fireballs:
            fb.draw(surface, camera)

    def take_damage(self) -> None:
        """Handle collision with enemy."""
        if self.invincible or self._dead:
            return
        if self.state in (MarioState.BIG, MarioState.FIRE):
            self._shrink()
            self._start_invincibility()
            self._assets.play_sound("pipe")  # pipe sound as shrink placeholder
        else:
            self.die()

    def die(self) -> None:
        if self._dead:
            return
        self._dead = True
        self._dead_timer = 0
        self.vel_y = self._dead_vel
        self.lives -= 1
        self._assets.play_sound("die")

    def power_up(self, effect: str) -> None:
        """Apply a power-up effect."""
        if effect == "grow" and self.state == MarioState.SMALL:
            self._grow()
            self._assets.play_sound("powerup")
        elif effect == "fire":
            if self.state == MarioState.SMALL:
                self._grow()
            self.state = MarioState.FIRE
            self._build_animations()
            self._assets.play_sound("powerup")
        elif effect == "invincible":
            self._start_invincibility(duration=600)
            self._assets.play_sound("powerup")

    def add_score(self, points: int) -> None:
        self.score += points

    def add_coin(self) -> None:
        self.coins += 1
        self.score += 200
        if self.coins >= 100:
            self.coins -= 100
            self.lives += 1
            self._assets.play_sound("1up")
        else:
            self._assets.play_sound("coin")

    def reset_position(self, x: int, y: int) -> None:
        self.rect.topleft = (x, y)
        self.vel_x = 0.0
        self.vel_y = 0.0
        self._dead = False
        self._dead_timer = 0
        self.invincible = True
        self._inv_timer = self._inv_duration
        self.state = MarioState.SMALL
        self._resize_to_state()
        self._build_animations()

    # ------------------------------------------------------------------ #
    # Private helpers                                                      #
    # ------------------------------------------------------------------ #

    def _grow(self) -> None:
        old_bottom = self.rect.bottom
        self.state = MarioState.BIG
        self._resize_to_state()
        self.rect.bottom = old_bottom
        self._build_animations()

    def _shrink(self) -> None:
        old_bottom = self.rect.bottom
        self.state = MarioState.SMALL
        self._resize_to_state()
        self.rect.bottom = old_bottom
        self._build_animations()

    def _resize_to_state(self) -> None:
        w, h = self._SIZES[self.state]
        self.rect.width = w
        self.rect.height = h

    def _start_invincibility(self, duration: Optional[int] = None) -> None:
        self.invincible = True
        self._inv_timer = duration if duration else self._inv_duration
        self._inv_blink = 0

    def _shoot_fireball(self) -> None:
        fb = Fireball(
            self.rect.centerx, self.rect.centery,
            1 if self.facing_right else -1,
            self._assets
        )
        self.fireballs.append(fb)
        self._assets.play_sound("fireball")

    def _update_death(self) -> None:
        self._dead_timer += 1
        if self._dead_timer < 20:
            return  # short pause before flying up
        self.vel_y += 0.5
        self.rect.y += int(self.vel_y)

    def _update_animation(self) -> None:
        suffix = "big" if self.state != MarioState.SMALL else "small"
        if self._dead:
            key = "dead"
        elif not self.on_ground:
            key = f"jump_{suffix}"
        elif abs(self.vel_x) > 0.5:
            key = f"walk_{suffix}"
        else:
            key = f"idle_{suffix}"

        if key != self._anim_key:
            self._anim_key = key
            if key in self._animations:
                self._animations[key].reset()

        if self._anim_key in self._animations:
            self._animations[self._anim_key].update()

    def _build_animations(self) -> None:
        """Create placeholder colored-rect animations for all states."""
        red = (220, 30, 30)
        dark_red = (160, 20, 20)
        blue = (40, 80, 220)
        white = (255, 255, 255)

        sm = self._SIZES[MarioState.SMALL]
        lg = self._SIZES[MarioState.BIG]

        fire_color = (255, 140, 0) if self.state == MarioState.FIRE else red

        self._animations = {
            "idle_small":  Animation(build_colored_frames(red, sm, 1), loop=True),
            "walk_small":  Animation(build_colored_frames(red, sm, 2,
                                     [red, dark_red]), speed=8, loop=True),
            "jump_small":  Animation(build_colored_frames(blue, sm, 1), loop=True),
            "idle_big":    Animation(build_colored_frames(fire_color, lg, 1), loop=True),
            "walk_big":    Animation(build_colored_frames(fire_color, lg, 2,
                                     [fire_color, dark_red]), speed=8, loop=True),
            "jump_big":    Animation(build_colored_frames(blue, lg, 1), loop=True),
            "dead":        Animation(build_colored_frames(white, sm, 1), loop=False),
        }


# ---------------------------------------------------------------------------
# Fireball — separated to keep Mario class focused
# ---------------------------------------------------------------------------

class Fireball(Entity):
    """Projectile fired by Mario in Fire state."""

    _SIZE = (10, 10)
    _SPEED = 7
    _GRAVITY = 0.4
    _MAX_BOUNCES = 4

    def __init__(self, x: int, y: int, direction: int,
                 assets: "AssetManager") -> None:
        super().__init__(x, y, *self._SIZE)
        self.vel_x = self._SPEED * direction
        self.vel_y = -3.0
        self._bounces = 0
        self._assets = assets

        # Animation
        orange = (255, 140, 0)
        yellow = (255, 220, 0)
        self._anim = Animation(
            build_colored_frames(orange, self._SIZE, 2, [orange, yellow]),
            speed=4, loop=True
        )

    def update(self, tiles: List[pygame.Rect]) -> None:  # type: ignore[override]
        if not self.alive:
            return
        self.apply_gravity(self._GRAVITY)
        self.move_x(tiles)

        if self.vel_x == 0:  # hit a wall
            self.alive = False
            return

        prev_y = self.rect.y
        self.move_y(tiles)
        if self.on_ground:
            self._bounces += 1
            self.vel_y = -5.0
            if self._bounces > self._MAX_BOUNCES:
                self.alive = False

        self._anim.update()

    def draw(self, surface: pygame.Surface, camera) -> None:
        frame = self._anim.current_frame
        surface.blit(frame, camera.apply(self.rect))
