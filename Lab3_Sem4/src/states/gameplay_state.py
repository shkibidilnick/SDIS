"""
GameplayState — coordinates all in-game systems:
  TileMap, Mario, Enemies, Bonuses, Camera, HUD, Timer.
Follows SRP by delegating specifics to sub-systems.
"""
from __future__ import annotations

import os
from typing import List, Optional, TYPE_CHECKING
import pygame

from src.states.base_state import BaseState
from src.tilemap import TileMap
from src.entities.mario import Mario
from src.entities.enemies import Goomba, Koopa, EnemyState
from src.entities.bonuses import Mushroom, FireFlower, Star, Coin
from src.hud import HUD
from src.utils.camera import Camera
from src.utils.config_loader import ConfigLoader

if TYPE_CHECKING:
    from src.game import Game


_CONFIGS_DIR = "configs"


class GameplayState(BaseState):
    """Full playable level state."""

    def __init__(self, game: "Game") -> None:
        super().__init__(game)
        self._settings  = game.settings
        self._assets    = game.assets
        self._scores_mgr = game.scores_manager

        self._tile_size: int = self._settings.get("tile_size", 32)
        self._gravity: float = self._settings.get("gravity", 0.5)
        self._screen_w: int  = self._settings["window"]["width"]
        self._screen_h: int  = self._settings["window"]["height"]

        self._score_values = self._settings.get("score", {})

        # Load level
        self._tilemap = self._load_level("level1.json")
        self._camera  = Camera(
            self._tilemap.width_px, self._screen_w, self._screen_h,
            self._settings
        )

        # Entities config
        ent_cfg = ConfigLoader.load(
            os.path.join(_CONFIGS_DIR, "entities.json"))
        self._enemy_cfgs = ent_cfg["enemies"]

        # Player
        lvl_cfg    = ConfigLoader.load(os.path.join(_CONFIGS_DIR, "level1.json"))
        start      = lvl_cfg.get("player_start", [2, 11])
        px = start[0] * self._tile_size
        py = start[1] * self._tile_size
        self._mario = Mario(px, py, self._settings, self._assets)

        # Enemies & bonuses
        self._enemies: List = []
        self._bonuses:  List = []
        self._floating_texts: List[dict] = []

        self._spawn_entities(lvl_cfg)
        self._spawn_coins(lvl_cfg)

        self._hud = HUD(self._assets, self._screen_w)

        # Timer
        self._time_left: float = self._tilemap.time_limit
        self._timer_tick: int  = 0   # counts game frames (60 fps → 1 sec)

        # Level end flag
        self._level_complete = False
        self._level_complete_timer = 0
        self._paused = False

        self._assets.play_music(self._tilemap.music)

    # ------------------------------------------------------------------ #
    # BaseState interface                                                  #
    # ------------------------------------------------------------------ #

    def handle_event(self, event: pygame.event.Event) -> None:
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                self._assets.stop_music()
                self._game.pop_state()
                self._game.push_state("menu")
            elif event.key == pygame.K_p:
                self._paused = not self._paused

    def update(self) -> None:
        if self._paused or self._level_complete:
            if self._level_complete:
                self._level_complete_timer += 1
                if self._level_complete_timer > 120:
                    self._finish_level()
            return

        keys = pygame.key.get_pressed()
        self._mario.handle_input(keys)
        self._mario.update(self._tilemap.solid_rects)

        self._update_timer()
        self._update_enemies()
        self._update_bonuses()
        self._camera.update(self._mario.rect)
        self._check_block_hits()
        self._check_flagpole()
        self._check_fall_death()
        self._update_floating_texts()

        # Death handling
        if self._mario.dead_finished:
            if self._mario.lives <= 0:
                self._game.push_state("gameover",
                                      score=self._mario.score)
            else:
                self._respawn_mario()

    def draw(self, surface: pygame.Surface) -> None:
        # Background
        surface.fill(self._tilemap.background_color)

        # Draw clouds / decorations
        self._draw_decorations(surface)

        self._tilemap.draw(surface, self._camera)

        for bonus in self._bonuses:
            bonus.draw(surface, self._camera)
        for enemy in self._enemies:
            enemy.draw(surface, self._camera)

        self._mario.draw(surface, self._camera)

        # Floating score texts
        for ft in self._floating_texts:
            rendered = self._hud._font.render(
                ft["text"], True, (255, 255, 100))
            surface.blit(rendered, (ft["x"] - self._camera.offset_x,
                                    ft["y"]))

        self._hud.draw(surface, self._mario,
                       int(self._time_left), "World 1-1")

        if self._paused:
            self._draw_pause(surface)

    # ------------------------------------------------------------------ #
    # Private — entity management                                          #
    # ------------------------------------------------------------------ #

    def _spawn_entities(self, lvl_cfg: dict) -> None:
        for e in lvl_cfg.get("entities", {}).get("enemies", []):
            x = e["x"] * self._tile_size
            y = e["y"] * self._tile_size
            etype = e.get("type", "goomba")
            if etype == "goomba":
                cfg = self._enemy_cfgs["goomba"]
                self._enemies.append(
                    Goomba(x, y, cfg, self._assets, self._gravity))
            elif etype == "koopa":
                cfg = self._enemy_cfgs["koopa"]
                self._enemies.append(
                    Koopa(x, y, cfg, self._assets, self._gravity))

    def _spawn_coins(self, lvl_cfg: dict) -> None:
        for c in lvl_cfg.get("entities", {}).get("coins", []):
            x = c["x"] * self._tile_size
            y = c["y"] * self._tile_size
            self._bonuses.append(Coin(x, y, self._assets))

    def _update_enemies(self) -> None:
        for enemy in self._enemies[:]:
            enemy.update(self._tilemap.solid_rects)
            if not enemy.alive:
                self._enemies.remove(enemy)
                continue

            # Mario vs enemy collision
            if not self._mario.is_dead and not self._mario.invincible \
                    and self._mario.rect.colliderect(enemy.rect):
                self._handle_mario_enemy_collision(enemy)

            # Fireball vs enemy
            for fb in self._mario.fireballs[:]:
                if fb.rect.colliderect(enemy.rect):
                    fb.alive = False
                    enemy.alive = False
                    pts = self._score_values.get("goomba_stomp", 100)
                    self._mario.add_score(pts)
                    self._add_floating_text(enemy.rect.centerx,
                                            enemy.rect.top - 10, f"+{pts}")

    def _handle_mario_enemy_collision(self, enemy) -> None:
        mario = self._mario
        # Stomp: Mario falling onto enemy top
        if mario.vel_y > 0 and \
                mario.rect.bottom <= enemy.rect.centery + 8:
            # Bounce Mario
            mario.vel_y = -8

            if isinstance(enemy, Goomba):
                enemy.stomp(lambda pts: (
                    mario.add_score(pts),
                    self._add_floating_text(
                        enemy.rect.centerx, enemy.rect.top - 10, f"+{pts}")
                ))
            elif isinstance(enemy, Koopa):
                from_right = mario.rect.centerx > enemy.rect.centerx
                enemy.stomp(lambda pts: (
                    mario.add_score(pts),
                    self._add_floating_text(
                        enemy.rect.centerx, enemy.rect.top - 10, f"+{pts}")
                ), from_right=from_right)
        else:
            # Side contact — damage Mario
            mario.take_damage()

    def _update_bonuses(self) -> None:
        for bonus in self._bonuses[:]:
            bonus.update(self._tilemap.solid_rects)
            if not bonus.alive:
                self._bonuses.remove(bonus)
                continue
            if not self._mario.is_dead \
                    and self._mario.rect.colliderect(bonus.rect):
                bonus.collect(self._mario)

    # ------------------------------------------------------------------ #
    # Private — block interaction                                          #
    # ------------------------------------------------------------------ #

    def _check_block_hits(self) -> None:
        """Detect Mario bumping a block from below."""
        mario = self._mario
        if mario.vel_y >= 0:  # only when moving up
            return

        # Small hitbox one pixel above Mario's head
        head_rect = pygame.Rect(mario.rect.x + 4, mario.rect.top - 1,
                                mario.rect.width - 8, 4)
        tile = self._tilemap.hit_block_at(head_rect)
        if tile is None:
            return

        if tile.tile_type == "brick":
            if mario.state.name != "SMALL":
                self._tilemap.remove_tile(tile)
                self._assets.play_sound("break")
                pts = self._score_values.get("brick_break", 50)
                mario.add_score(pts)
            else:
                self._assets.play_sound("bump")
        else:
            # Question block — spawn item
            self._assets.play_sound("coin")
            self._spawn_block_item(tile)

    def _spawn_block_item(self, tile) -> None:
        """Decide what comes out of the block."""
        bx = tile.rect.x
        by = tile.rect.y
        mario = self._mario

        # Determine drop
        if mario.state.name == "SMALL":
            item = Mushroom(bx, by, self._assets, self._gravity)
        else:
            item = FireFlower(bx, by, self._assets)

        self._bonuses.append(item)
        pts = self._score_values.get("coin", 200)
        mario.add_score(pts)
        self._add_floating_text(bx, by - 20, f"+{pts}")

    # ------------------------------------------------------------------ #
    # Private — misc                                                       #
    # ------------------------------------------------------------------ #

    def _update_timer(self) -> None:
        self._timer_tick += 1
        if self._timer_tick >= 60:
            self._timer_tick = 0
            self._time_left -= 1
            if self._time_left <= 0:
                self._time_left = 0
                self._mario.die()

    def _check_flagpole(self) -> None:
        if self._level_complete:
            return
        fp = self._tilemap.flagpole_rect
        if fp and self._mario.rect.colliderect(fp):
            self._level_complete = True
            self._level_complete_timer = 0
            self._assets.stop_music()
            self._assets.play_sound("flagpole")
            pts = self._score_values.get("flagpole", 5000)
            self._mario.add_score(pts)

    def _check_fall_death(self) -> None:
        if self._mario.rect.top > self._screen_h + 64:
            if not self._mario.is_dead:
                self._mario.die()

    def _respawn_mario(self) -> None:
        lvl_cfg = ConfigLoader.load(os.path.join(_CONFIGS_DIR, "level1.json"))
        start   = lvl_cfg.get("player_start", [2, 11])
        self._mario.reset_position(
            start[0] * self._tile_size,
            start[1] * self._tile_size
        )
        self._camera.reset()
        self._assets.play_music(self._tilemap.music)

    def _finish_level(self) -> None:
        score = self._mario.score
        if self._scores_mgr.is_high_score(score):
            self._game.push_state("new_record", score=score)
        else:
            self._game.pop_state()
            self._game.push_state("highscores")

    def _add_floating_text(self, wx: int, wy: int, text: str) -> None:
        self._floating_texts.append({
            "text": text, "x": wx, "y": wy,
            "life": 50, "max_life": 50
        })

    def _update_floating_texts(self) -> None:
        for ft in self._floating_texts[:]:
            ft["y"] -= 1
            ft["life"] -= 1
            if ft["life"] <= 0:
                self._floating_texts.remove(ft)

    def _load_level(self, filename: str) -> TileMap:
        path = os.path.join(_CONFIGS_DIR, filename)
        cfg  = ConfigLoader.load(path)
        return TileMap.from_config(cfg, self._tile_size)

    def _draw_decorations(self, surface: pygame.Surface) -> None:
        """Draw simple paralax clouds."""
        w = surface.get_width()
        cloud_positions = [(100, 80), (300, 60), (550, 90), (720, 70)]
        offset = (self._camera.offset_x // 3) % w  # slow parallax
        for cx, cy in cloud_positions:
            draw_x = (cx - offset) % (w + 120) - 60
            self._draw_cloud(surface, draw_x, cy)

    def _draw_cloud(self, surface: pygame.Surface,
                    x: int, y: int) -> None:
        for dx, dy, r in [(0, 0, 22), (24, -10, 28), (52, 0, 20)]:
            pygame.draw.circle(surface, (255, 255, 255),
                                (x + dx, y + dy), r)

    def _draw_pause(self, surface: pygame.Surface) -> None:
        overlay = pygame.Surface(surface.get_size(), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 100))
        surface.blit(overlay, (0, 0))
        font = self._assets.get_font(36, bold=True)
        txt = font.render("ПАУЗА", True, (255, 220, 0))
        surface.blit(txt, txt.get_rect(
            center=(surface.get_width() // 2, surface.get_height() // 2)))
