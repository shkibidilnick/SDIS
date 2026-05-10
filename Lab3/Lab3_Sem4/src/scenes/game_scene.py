"""Сцена игрового процесса."""
import random
from typing import List

import pygame

from ..entities.enemy import EnemyFactory
from ..entities.level import (Level, TILE_BRICK, TILE_COIN, TILE_EMPTY,
                              TILE_FLAG, TILE_QUESTION)
from ..entities.particle import Particle
from ..entities.player import Player
from ..entities.powerup import PowerUpFactory
from .scene import Scene


class GameScene(Scene):
    """Игровой уровень."""

    STATE_PLAYING = "playing"
    STATE_PAUSED = "paused"
    STATE_LEVEL_COMPLETE = "level_complete"
    STATE_GAME_OVER = "game_over"
    STATE_RESPAWNING = "respawning"

    def __init__(self, app):
        super().__init__(app)
        self.level: Level = None
        self.player: Player = None
        self.enemies = []
        self.powerups = []
        self.particles: List[Particle] = []
        self.camera_x = 0
        self.score = 0
        self.coins_collected = 0
        self.state = self.STATE_PLAYING
        self.time_left = 0
        self.time_tick = 0  # счётчик кадров для отсчёта секунд
        self.respawn_timer = 0
        self.complete_timer = 0
        # Хранит изначальные координаты тайлов с ?, в которых был powerup
        self._used_question_blocks = set()

    def on_enter(self) -> None:
        super().on_enter()
        self._reset_level()
        self.score = 0
        self.coins_collected = 0
        self.state = self.STATE_PLAYING

    def _reset_level(self) -> None:
        cfg = self.app.config
        # Уровень
        level_data = self.app.config_loader.load(cfg["files"]["level_map"],
                                                 use_cache=False)
        self.level = Level(level_data, tile_size=cfg["tile"]["size"])
        # Игрок
        spawn_tx, spawn_ty = self.level.spawn
        ts = cfg["tile"]["size"]
        px = spawn_tx * ts
        py = spawn_ty * ts + (ts - cfg["player"]["height"])
        self.player = Player(px, py, cfg)
        # Враги
        enemies_data = self.app.config_loader.load(cfg["files"]["enemies"])
        self.enemy_factory = EnemyFactory(enemies_data,
                                          gravity=cfg["physics"]["gravity"],
                                          max_fall_speed=cfg["physics"]["max_fall_speed"],
                                          tile_size=ts)
        self.enemies = self.enemy_factory.create_all()
        # Бонусы
        powerups_data = self.app.config_loader.load(cfg["files"]["powerups"])
        self.powerup_factory = PowerUpFactory(
            powerups_data,
            gravity=cfg["physics"]["gravity"],
            max_fall_speed=cfg["physics"]["max_fall_speed"],
            tile_size=ts,
        )
        self.powerups = []
        self.particles = []
        self.camera_x = 0
        self.time_left = cfg["level"]["time_limit"]
        self.time_tick = 0
        self._used_question_blocks = set()

    # ----- Ввод -----
    def handle_events(self, events: List) -> None:
        for e in events:
            if e.type == pygame.KEYDOWN:
                if e.key == pygame.K_ESCAPE:
                    if self.state == self.STATE_PLAYING:
                        self.state = self.STATE_PAUSED
                    elif self.state == self.STATE_PAUSED:
                        self.state = self.STATE_PLAYING
                    else:
                        self.next_scene = "menu"
                        self.done = True
                elif e.key in (pygame.K_SPACE, pygame.K_w, pygame.K_UP):
                    if self.state == self.STATE_PLAYING:
                        if self.player.jump():
                            self.app.audio.play_sfx("jump")
                elif self.state == self.STATE_GAME_OVER:
                    if e.key == pygame.K_RETURN:
                        self._on_game_over_finish()
                elif self.state == self.STATE_PAUSED:
                    if e.key == pygame.K_q:
                        self.next_scene = "menu"
                        self.done = True

        # непрерывное движение
        keys = pygame.key.get_pressed()
        if self.state == self.STATE_PLAYING and not self.player.dying:
            self.player.set_running(keys[pygame.K_LSHIFT] or keys[pygame.K_RSHIFT])
            if keys[pygame.K_LEFT] or keys[pygame.K_a]:
                self.player.move_left()
            elif keys[pygame.K_RIGHT] or keys[pygame.K_d]:
                self.player.move_right()
            else:
                self.player.stop_horizontal()

    # ----- Обновление -----
    def update(self) -> None:
        self.app.renderer.update_animations()
        if self.state == self.STATE_PAUSED:
            return

        if self.state == self.STATE_LEVEL_COMPLETE:
            self.complete_timer += 1
            # бонус за оставшееся время
            if self.time_left > 0 and self.complete_timer % 2 == 0:
                self.time_left -= 1
                self.score += self.app.config["scoring"]["time_bonus_per_second"]
            elif self.complete_timer >= 180:
                self._on_level_complete_finish()
            return

        if self.state == self.STATE_GAME_OVER:
            return

        if self.state == self.STATE_RESPAWNING:
            self.respawn_timer -= 1
            if self.respawn_timer <= 0:
                self.player.respawn()
                self.camera_x = 0
                self.state = self.STATE_PLAYING
            return

        # отсчёт времени
        self.time_tick += 1
        if self.time_tick >= 60:
            self.time_tick = 0
            self.time_left -= 1
            if self.time_left <= 0:
                self.time_left = 0
                self._kill_player()
                return

        # игрок
        result = self.player.update(self.level)

        # удар головой по блоку (?, кирпич)
        for tx, ty in result["tiles_hit_top"]:
            self._handle_block_hit(tx, ty)

        # сбор монет/флага
        for tx, ty, tile in self.level.interactive_tiles_in_rect(self.player.rect):
            if tile == TILE_COIN:
                self._collect_coin(tx, ty)
            elif tile == TILE_FLAG:
                self._on_flag_touched()

        # столкновения с врагами
        for enemy in self.enemies:
            if not enemy.alive or enemy.dying:
                continue
            # Спрятанная пиранья не наносит урон
            if (enemy.type_name == "piranha"
                    and getattr(enemy, "_piranha_state", "") == "hidden"):
                continue
            if not self.player.rect.intersects(enemy.rect):
                continue
            self._handle_enemy_collision(enemy)

        # столкновения с бонусами
        for p in self.powerups:
            if not p.alive or p.emerging:
                continue
            if self.player.rect.intersects(p.rect):
                self._collect_powerup(p)

        # обновление врагов
        for enemy in self.enemies:
            enemy.update(self.level)
        # обновление бонусов
        for p in self.powerups:
            p.update(self.level)
        # частицы
        for part in self.particles:
            part.update()

        # очистить мёртвые
        self.enemies = [e for e in self.enemies if e.alive]
        self.powerups = [p for p in self.powerups if p.alive]
        self.particles = [p for p in self.particles if p.alive]

        # проверка смерти игрока
        if not self.player.alive:
            self._on_player_died()

        # камера следит за игроком
        self._update_camera()

    def _update_camera(self) -> None:
        target = self.player.rect.center[0] - self.app.config["screen"]["width"] // 2
        target = max(0, target)
        target = min(target, self.level.pixel_width
                     - self.app.config["screen"]["width"])
        self.camera_x = int(target)

    def _handle_block_hit(self, tx: int, ty: int) -> None:
        tile = self.level.get_tile(tx, ty)
        if tile == TILE_QUESTION and (tx, ty) not in self._used_question_blocks:
            self._used_question_blocks.add((tx, ty))
            self.level.set_tile(tx, ty, TILE_BRICK)
            import random
            # Более равномерное распределение
            powerup_type = random.choices(
                ["mushroom", "fireflower", "star"],
                weights=[40, 35, 25],
                k=1,
            )[0]
            powerup = self.powerup_factory.create_at_block(tx, ty, powerup_type)
            self.powerups.append(powerup)
            self.app.audio.play_sfx("powerup")
        elif tile == TILE_BRICK:
            # большой Марио ломает кирпич
            if self.player.state == Player.STATE_BIG:
                self.level.set_tile(tx, ty, TILE_EMPTY)
                self._spawn_brick_particles(tx, ty)
            self.app.audio.play_sfx("stomp")

    def _collect_coin(self, tx: int, ty: int) -> None:
        self.level.set_tile(tx, ty, TILE_EMPTY)
        self.score += self.app.config["scoring"]["coin"]
        self.coins_collected += 1
        self.app.audio.play_sfx("coin")
        # частицы
        ts = self.level.tile_size
        for _ in range(6):
            self.particles.append(Particle(
                tx * ts + ts // 2, ty * ts + ts // 2,
                random.uniform(-2, 2), random.uniform(-4, -1),
                (255, 220, 0), lifetime=24, size=3))

    def _spawn_brick_particles(self, tx: int, ty: int) -> None:
        ts = self.level.tile_size
        for _ in range(8):
            self.particles.append(Particle(
                tx * ts + ts // 2, ty * ts + ts // 2,
                random.uniform(-3, 3), random.uniform(-5, -2),
                (200, 76, 12), lifetime=40, size=4))

    def _collect_powerup(self, powerup) -> None:
        powerup.alive = False
        self.score += powerup.score_value
        self.player.apply_powerup(powerup.effect, powerup.duration)
        self.app.audio.play_sfx("powerup")

    def _handle_enemy_collision(self, enemy) -> None:
        # === 1. Звезда (неуязвимость): любой контакт ===
        # Если игрок маленький — он не убивает, а проходит сквозь и отталкивается.
        # Если игрок большой — убивает врага касанием.
        if self.player.star_timer > 0:
            if self.player.state == Player.STATE_BIG:
                self.score += enemy.kill_by_star()
                self.app.audio.play_sfx("stomp")
            else:
                # Маленький Марио со звездой: враг живёт, Марио сдвигается
                self._push_player_away_from(enemy)
            return

        # === 2. Прыжок сверху (стандартный стомп) ===
        # Только если игрок реально летит вниз и его низ выше центра врага.
        is_stomp = (
                enemy.can_be_stomped
                and self.player.vy > 0
                and self.player.rect.bottom <= enemy.rect.center[1] + 4
        )
        if is_stomp:
            self.score += enemy.stomp()
            self.player.vy = self.app.config["physics"]["jump_strength"] * 0.6
            self.app.audio.play_sfx("stomp")
            return

        # === 3. Боковой/нижний контакт ===
        if self.player.state == Player.STATE_BIG:
            # Большой Марио: Гумба гибнет от прямого столкновения, Купа — нет
            if enemy.type_name == "goomba":
                self.score += enemy.kill_by_star()
                self.app.audio.play_sfx("stomp")
                return
            # С Купой и Пираньей — урон (уменьшение до маленького)
            died = self.player.take_damage()
            if died:
                self.app.audio.play_sfx("death")
        else:
            # Маленький Марио без неуязвимости — смерть/урон
            died = self.player.take_damage()
            if died:
                self.app.audio.play_sfx("death")

    def _push_player_away_from(self, enemy) -> None:
        """Лёгкое отталкивание игрока от врага (для звезды у маленького Марио)."""
        push = 4
        if self.player.rect.center[0] < enemy.rect.center[0]:
            self.player.rect.x -= push
        else:
            self.player.rect.x += push
        # лёгкий подскок, чтоб не залипало
        if self.player.on_ground:
            self.player.vy = -3

    def _on_flag_touched(self) -> None:
        if self.state != self.STATE_PLAYING:
            return
        self.state = self.STATE_LEVEL_COMPLETE
        self.complete_timer = 0
        self.score += self.app.config["scoring"]["level_complete"]
        # Бонус за каждую собранную монету (5×коэф)
        self.score += self.coins_collected * 5
        self.app.audio.stop_music()
        self.app.audio.play_sfx("level_complete")

    def _on_level_complete_finish(self) -> None:
        # перевод на сцену ввода имени или меню
        if self.app.highscores.is_high_score(self.score):
            self.app.pending_score = self.score
            self.next_scene = "name_entry"
        else:
            self.next_scene = "menu"
        self.done = True

    def _kill_player(self) -> None:
        if not self.player.dying:
            self.player.start_death()
            self.app.audio.play_sfx("death")

    def _on_player_died(self) -> None:
        self.player.lives -= 1
        if self.player.lives <= 0:
            self.state = self.STATE_GAME_OVER
        else:
            self._respawn_with_state_reset()

    def _respawn_with_state_reset(self) -> None:
        # сбросить уровень частично — игрок заново, но прогресс монет/очков сохранён
        cfg = self.app.config
        spawn_tx, spawn_ty = self.level.spawn
        ts = cfg["tile"]["size"]
        self.player.rect.x = spawn_tx * ts
        self.player.rect.y = spawn_ty * ts + (ts - cfg["player"]["height"])
        self.player.respawn()
        self.time_left = cfg["level"]["time_limit"]
        self.state = self.STATE_RESPAWNING
        self.respawn_timer = 60

    def _on_game_over_finish(self) -> None:
        if self.app.highscores.is_high_score(self.score):
            self.app.pending_score = self.score
            self.next_scene = "name_entry"
        else:
            self.next_scene = "menu"
        self.done = True

    # ----- Отрисовка -----
    def draw(self) -> None:
        s = self.app.surface
        s.fill(self.level.background_color)
        self.app.renderer.draw_clouds(self.camera_x)
        self.app.renderer.draw_level(self.level, self.camera_x)

        for p in self.powerups:
            self.app.renderer.draw_powerup(p, self.camera_x)
        for enemy in self.enemies:
            self.app.renderer.draw_enemy(enemy, self.camera_x)
        if self.player and self.player.alive or (self.player and self.player.dying):
            self.app.renderer.draw_player(self.player, self.camera_x)
        self.app.renderer.draw_particles(self.particles, self.camera_x)

        self.app.renderer.draw_hud(self.score, self.player.lives,
                                   self.time_left, self.coins_collected)

        if self.state == self.STATE_PAUSED:
            overlay = pygame.Surface(s.get_size(), pygame.SRCALPHA)
            overlay.fill((0, 0, 0, 160))
            s.blit(overlay, (0, 0))
            self.app.renderer.draw_centered_text(
                "ПАУЗА", s.get_height() // 2 - 30,
                self.app.renderer.font_huge, (255, 255, 0))
            self.app.renderer.draw_centered_text(
                "ESC — продолжить    Q — выйти в меню",
                s.get_height() // 2 + 30,
                self.app.renderer.font_small, (240, 240, 240))
        elif self.state == self.STATE_LEVEL_COMPLETE:
            overlay = pygame.Surface(s.get_size(), pygame.SRCALPHA)
            overlay.fill((0, 0, 0, 100))
            s.blit(overlay, (0, 0))
            self.app.renderer.draw_centered_text(
                "УРОВЕНЬ ПРОЙДЕН!", s.get_height() // 2 - 30,
                self.app.renderer.font_big, (255, 220, 0))
            self.app.renderer.draw_centered_text(
                f"СЧЁТ: {self.score}", s.get_height() // 2 + 20,
                self.app.renderer.font_med, (255, 255, 255))
        elif self.state == self.STATE_GAME_OVER:
            overlay = pygame.Surface(s.get_size(), pygame.SRCALPHA)
            overlay.fill((0, 0, 0, 200))
            s.blit(overlay, (0, 0))
            self.app.renderer.draw_centered_text(
                "ИГРА ОКОНЧЕНА", s.get_height() // 2 - 30,
                self.app.renderer.font_big, (255, 60, 60))
            self.app.renderer.draw_centered_text(
                f"СЧЁТ: {self.score}", s.get_height() // 2 + 20,
                self.app.renderer.font_med, (255, 255, 255))
            self.app.renderer.draw_centered_text(
                "ENTER — продолжить",
                s.get_height() // 2 + 80,
                self.app.renderer.font_small, (220, 220, 220))
