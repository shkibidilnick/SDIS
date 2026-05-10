"""Рендерер игры. Все спрайты рисуются программно (без внешних PNG),
чтобы у проекта не было лишних зависимостей.

Соблюдает SRP: только отображение, без логики игры.
"""
from typing import List, TYPE_CHECKING, Tuple

import pygame

from ..entities.level import (TILE_BRICK, TILE_COIN, TILE_FLAG, TILE_GROUND,
                              TILE_PIPE, TILE_QUESTION, TILE_STAIR)

if TYPE_CHECKING:
    from ..entities.enemy import Enemy
    from ..entities.level import Level
    from ..entities.particle import Particle
    from ..entities.player import Player
    from ..entities.powerup import PowerUp


# Цвета палитры
COLOR_GROUND_TOP = (200, 76, 12)
COLOR_GROUND_BOTTOM = (160, 60, 10)
COLOR_BRICK = (200, 76, 12)
COLOR_BRICK_LINE = (90, 30, 5)
COLOR_QUESTION = (252, 188, 0)
COLOR_QUESTION_DARK = (180, 130, 0)
COLOR_PIPE = (0, 168, 0)
COLOR_PIPE_DARK = (0, 110, 0)
COLOR_COIN = (252, 220, 0)
COLOR_COIN_DARK = (200, 160, 0)
COLOR_FLAG_POLE = (220, 220, 220)
COLOR_FLAG = (240, 60, 60)
COLOR_STAIR = (170, 100, 40)
COLOR_PLAYER_RED = (220, 30, 30)
COLOR_PLAYER_BLUE = (30, 60, 180)
COLOR_PLAYER_SKIN = (252, 188, 140)
COLOR_PLAYER_HAIR = (60, 30, 0)
COLOR_PLAYER_BIG_RED = (240, 60, 60)
COLOR_WHITE = (255, 255, 255)
COLOR_BLACK = (0, 0, 0)
COLOR_HUD_BG = (0, 0, 0, 120)
COLOR_CLOUD = (255, 255, 255)


class Renderer:
    """Отвечает за всю отрисовку игры."""

    def __init__(self, surface: pygame.Surface, screen_width: int,
                 screen_height: int, tile_size: int):
        self.surface = surface
        self.screen_width = screen_width
        self.screen_height = screen_height
        self.tile_size = tile_size
        self.font_small = pygame.font.SysFont("monospace", 16, bold=True)
        self.font_med = pygame.font.SysFont("monospace", 22, bold=True)
        self.font_big = pygame.font.SysFont("monospace", 36, bold=True)
        self.font_huge = pygame.font.SysFont("monospace", 56, bold=True)

        # Анимация монет в карте
        self._coin_anim_frame = 0
        self._coin_anim_timer = 0

    # ----- Утилитарные функции отрисовки -----
    def update_animations(self) -> None:
        """Обновить общие анимационные таймеры (вызывать раз в кадр)."""
        self._coin_anim_timer += 1
        if self._coin_anim_timer >= 8:
            self._coin_anim_timer = 0
            self._coin_anim_frame = (self._coin_anim_frame + 1) % 4

    def clear(self, color: Tuple[int, int, int]) -> None:
        self.surface.fill(color)

    def draw_clouds(self, camera_x: int) -> None:
        """Декоративные облака с лёгким параллакс-эффектом."""
        cloud_positions = [(120, 80), (350, 50), (600, 100), (850, 70),
                           (1100, 90), (1400, 60), (1700, 80), (2000, 100),
                           (2400, 70), (2800, 90), (3200, 60), (3600, 80),
                           (4000, 70), (4400, 90), (4800, 60), (5200, 80),
                           (5600, 70), (6000, 90), (6400, 60), (6800, 80),
                           (7200, 70), (7600, 90)]
        for cx, cy in cloud_positions:
            sx = cx - camera_x // 2  # параллакс
            if -100 < sx < self.screen_width + 100:
                self._draw_cloud(sx, cy)

    def _draw_cloud(self, x: int, y: int) -> None:
        pygame.draw.circle(self.surface, COLOR_CLOUD, (x, y), 14)
        pygame.draw.circle(self.surface, COLOR_CLOUD, (x + 16, y - 6), 18)
        pygame.draw.circle(self.surface, COLOR_CLOUD, (x + 36, y), 14)
        pygame.draw.circle(self.surface, COLOR_CLOUD, (x + 22, y + 6), 14)

    # ----- Уровень -----
    def draw_level(self, level: "Level", camera_x: int) -> None:
        ts = self.tile_size
        x_start = max(0, camera_x // ts)
        x_end = min(level.width_tiles, (camera_x + self.screen_width) // ts + 1)
        for ty in range(level.height_tiles):
            for tx in range(x_start, x_end):
                tile = level.get_tile(tx, ty)
                if tile == ".":
                    continue
                px = tx * ts - camera_x
                py = ty * ts
                # Спецотрисовка верха трубы (если над ним пусто)
                if tile == TILE_PIPE and level.get_tile(tx, ty - 1) != TILE_PIPE:
                    self._draw_pipe_top(px, py)
                else:
                    self._draw_tile(tile, px, py)

    def _draw_pipe_top(self, x: int, y: int) -> None:
        """Верхняя часть трубы с расширенной крышкой."""
        ts = self.tile_size
        # Тело
        pygame.draw.rect(self.surface, COLOR_PIPE, (x, y + 6, ts, ts - 6))
        pygame.draw.rect(self.surface, COLOR_PIPE_DARK, (x, y + 6, ts, ts - 6), 2)
        # Крышка-«козырёк» (немного шире тела)
        pygame.draw.rect(self.surface, COLOR_PIPE, (x - 2, y, ts + 4, 10))
        pygame.draw.rect(self.surface, COLOR_PIPE_DARK, (x - 2, y, ts + 4, 10), 2)
        # Вертикальный блик
        pygame.draw.line(self.surface, COLOR_PIPE_DARK,
                         (x + 5, y + 12), (x + 5, y + ts - 5), 2)

    def _draw_tile(self, tile: str, x: int, y: int) -> None:
        ts = self.tile_size
        if tile == TILE_GROUND:
            pygame.draw.rect(self.surface, COLOR_GROUND_TOP, (x, y, ts, ts))
            pygame.draw.rect(self.surface, COLOR_GROUND_BOTTOM, (x, y + ts // 2,
                                                                  ts, ts // 2))
            # тёмные точки-камешки
            pygame.draw.rect(self.surface, COLOR_BRICK_LINE, (x + 4, y + 6, 4, 4))
            pygame.draw.rect(self.surface, COLOR_BRICK_LINE, (x + 20, y + 18, 4, 4))
        elif tile == TILE_BRICK:
            pygame.draw.rect(self.surface, COLOR_BRICK, (x, y, ts, ts))
            pygame.draw.rect(self.surface, COLOR_BRICK_LINE, (x, y, ts, ts), 2)
            pygame.draw.line(self.surface, COLOR_BRICK_LINE,
                             (x, y + ts // 2), (x + ts, y + ts // 2), 1)
            pygame.draw.line(self.surface, COLOR_BRICK_LINE,
                             (x + ts // 2, y), (x + ts // 2, y + ts // 2), 1)
            pygame.draw.line(self.surface, COLOR_BRICK_LINE,
                             (x + ts // 4, y + ts // 2),
                             (x + ts // 4, y + ts), 1)
            pygame.draw.line(self.surface, COLOR_BRICK_LINE,
                             (x + 3 * ts // 4, y + ts // 2),
                             (x + 3 * ts // 4, y + ts), 1)
        elif tile == TILE_QUESTION:
            pygame.draw.rect(self.surface, COLOR_QUESTION, (x, y, ts, ts))
            pygame.draw.rect(self.surface, COLOR_QUESTION_DARK, (x, y, ts, ts), 2)
            text = self.font_med.render("?", True, COLOR_BLACK)
            tr = text.get_rect(center=(x + ts // 2, y + ts // 2))
            self.surface.blit(text, tr)
        elif tile == TILE_PIPE:
            # Тело трубы
            pygame.draw.rect(self.surface, COLOR_PIPE, (x, y, ts, ts))
            pygame.draw.rect(self.surface, COLOR_PIPE_DARK, (x, y, ts, ts), 2)
            pygame.draw.line(self.surface, COLOR_PIPE_DARK,
                             (x + 5, y + 5), (x + 5, y + ts - 5), 2)
            # Если это верхняя часть трубы — рисуем «крышку» с расширением
            # (Renderer не знает соседних тайлов, но может проверить)
            tx_idx = (x + self._cam_x_for_pipe) // ts if hasattr(self, "_cam_x_for_pipe") else None
        elif tile == TILE_COIN:
            # анимированная монетка
            offset = self._coin_anim_frame
            w_variant = [ts // 2, ts // 3, ts // 4, ts // 3][offset]
            pygame.draw.ellipse(self.surface, COLOR_COIN,
                                (x + (ts - w_variant) // 2, y + 6,
                                 w_variant, ts - 12))
            pygame.draw.ellipse(self.surface, COLOR_COIN_DARK,
                                (x + (ts - w_variant) // 2, y + 6,
                                 w_variant, ts - 12), 2)
        elif tile == TILE_FLAG:
            # шест и флаг
            pygame.draw.rect(self.surface, COLOR_FLAG_POLE,
                             (x + ts // 2 - 2, y, 4, ts))
            pygame.draw.polygon(self.surface, COLOR_FLAG,
                                [(x + ts // 2 + 2, y + 4),
                                 (x + ts - 2, y + 10),
                                 (x + ts // 2 + 2, y + 16)])
        elif tile == TILE_STAIR:
            pygame.draw.rect(self.surface, COLOR_STAIR, (x, y, ts, ts))
            pygame.draw.rect(self.surface, COLOR_BRICK_LINE, (x, y, ts, ts), 2)

    # ----- Игрок -----
    def draw_player(self, player: "Player", camera_x: int) -> None:
        # мерцание при неуязвимости
        if player.invincible_timer > 0 and player.invincible_timer % 6 < 3:
            return
        x = int(player.rect.x - camera_x)
        y = int(player.rect.y)
        w = int(player.rect.w)
        h = int(player.rect.h)

        # цвета (звёздная неуязвимость даёт радугу)
        if player.star_timer > 0:
            colors = [(255, 50, 50), (255, 150, 0), (255, 255, 0),
                      (0, 200, 0), (0, 150, 255), (180, 0, 255)]
            body_color = colors[(player.star_timer // 4) % len(colors)]
        else:
            body_color = COLOR_PLAYER_BLUE  # штаны
        head_color = COLOR_PLAYER_SKIN
        cap_color = COLOR_PLAYER_RED

        # Голова
        head_h = h // 3
        body_h = h - head_h
        head_w = w
        # шапка сверху
        pygame.draw.rect(self.surface, cap_color, (x, y, head_w, head_h // 2))
        # лицо
        pygame.draw.rect(self.surface, head_color,
                         (x, y + head_h // 2, head_w, head_h // 2))
        # глаз
        eye_x = x + head_w - 8 if player.facing_right else x + 4
        pygame.draw.rect(self.surface, COLOR_BLACK,
                         (eye_x, y + head_h // 2 + 2, 3, 4))
        # тело: красное (рубашка) + штаны
        shirt_h = body_h // 2
        pygame.draw.rect(self.surface, COLOR_PLAYER_BIG_RED,
                         (x, y + head_h, w, shirt_h))
        pygame.draw.rect(self.surface, body_color,
                         (x, y + head_h + shirt_h, w, body_h - shirt_h))

        # Ноги (анимация ходьбы)
        if player.dying:
            return
        if abs(player.vx) > 0.1:
            leg_offset = [0, 2, 0][player.anim_frame]
            pygame.draw.rect(self.surface, COLOR_BLACK,
                             (x + 2, y + h - 4 + leg_offset, w // 2 - 4, 4))
            pygame.draw.rect(self.surface, COLOR_BLACK,
                             (x + w // 2 + 2, y + h - 4 - leg_offset,
                              w // 2 - 4, 4))
        else:
            pygame.draw.rect(self.surface, COLOR_BLACK,
                             (x + 2, y + h - 4, w - 4, 4))

    # ----- Враги -----
    def draw_enemy(self, enemy: "Enemy", camera_x: int) -> None:
        x = int(enemy.rect.x - camera_x)
        y = int(enemy.rect.y)
        w = int(enemy.rect.w)
        h = int(enemy.rect.h)
        if enemy.dying:
            pygame.draw.rect(self.surface, enemy.color,
                             (x, y + h - h // 4, w, h // 4))
            return

        if enemy.type_name == "goomba":
            pygame.draw.ellipse(self.surface, enemy.color, (x, y, w, h * 2 // 3))
            pygame.draw.ellipse(self.surface, COLOR_BLACK, (x, y, w, h * 2 // 3), 2)
            pygame.draw.rect(self.surface, COLOR_WHITE, (x + 5, y + 8, 5, 7))
            pygame.draw.rect(self.surface, COLOR_WHITE, (x + w - 10, y + 8, 5, 7))
            pygame.draw.rect(self.surface, COLOR_BLACK, (x + 7, y + 9, 2, 4))
            pygame.draw.rect(self.surface, COLOR_BLACK, (x + w - 8, y + 9, 2, 4))
            foot_offset = enemy.anim_frame * 2
            pygame.draw.rect(self.surface, COLOR_BLACK,
                             (x + 2, y + h - 6 + foot_offset, w // 2 - 4, 6))
            pygame.draw.rect(self.surface, COLOR_BLACK,
                             (x + w // 2 + 2, y + h - 6 - foot_offset,
                              w // 2 - 4, 6))

        elif enemy.type_name == "koopa":
            pygame.draw.ellipse(self.surface, enemy.color,
                                (x, y + h // 3, w, h * 2 // 3))
            pygame.draw.ellipse(self.surface, COLOR_BLACK,
                                (x, y + h // 3, w, h * 2 // 3), 2)
            head_x = x + w - 10 if enemy.vx > 0 else x
            pygame.draw.ellipse(self.surface, (240, 220, 120),
                                (head_x, y, 12, 14))
            pygame.draw.ellipse(self.surface, COLOR_BLACK,
                                (head_x, y, 12, 14), 1)
            eye_dx = 8 if enemy.vx > 0 else 2
            pygame.draw.rect(self.surface, COLOR_BLACK,
                             (head_x + eye_dx, y + 5, 2, 3))


        elif enemy.type_name == "piranha":
            # Обрезаем отрисовку по верху трубы — что ниже трубы, не рисуется
            pipe_top = getattr(enemy, "_piranha_pipe_top_y", None)
            old_clip = self.surface.get_clip()
            if pipe_top is not None:
                visible_h = max(0, int(pipe_top) - y)
                if visible_h <= 0:
                    return
                self.surface.set_clip(pygame.Rect(0, 0,
                                                  self.screen_width, int(pipe_top)))

            # Стебель (зелёный)
            stem_w = 6
            pygame.draw.rect(self.surface, (0, 130, 0),
                             (x + w // 2 - stem_w // 2, y + h // 2,
                              stem_w, h // 2))

            # Красная голова
            head_h = h // 2
            pygame.draw.ellipse(self.surface, (220, 40, 40), (x, y, w, head_h))
            pygame.draw.ellipse(self.surface, COLOR_BLACK, (x, y, w, head_h), 2)
            # Белые пятна
            pygame.draw.circle(self.surface, COLOR_WHITE,
                               (x + 7, y + head_h // 2), 3)
            pygame.draw.circle(self.surface, COLOR_WHITE,
                               (x + w - 7, y + head_h // 2), 3)
            # Рот (анимация открыто/закрыто)
            mouth_open = (enemy.anim_frame == 1)
            if mouth_open:
                pygame.draw.rect(self.surface, COLOR_BLACK,
                                 (x + 6, y + head_h // 2, w - 12, 6))
                for i in range(3):
                    pygame.draw.polygon(
                        self.surface, COLOR_WHITE,
                        [(x + 8 + i * 6, y + head_h // 2),
                         (x + 11 + i * 6, y + head_h // 2 + 4),
                         (x + 14 + i * 6, y + head_h // 2)])
            else:
                pygame.draw.line(self.surface, COLOR_BLACK,
                                 (x + 6, y + head_h // 2 + 2),
                                 (x + w - 6, y + head_h // 2 + 2), 2)
            # Восстановить клип
            self.surface.set_clip(old_clip)

    # ----- Бонусы -----
    def draw_powerup(self, p: "PowerUp", camera_x: int) -> None:
        x = int(p.rect.x - camera_x)
        y = int(p.rect.y)
        w = int(p.rect.w)
        h = int(p.rect.h)
        if p.type_name == "mushroom":
            pygame.draw.ellipse(self.surface, p.color, (x, y, w, h * 2 // 3))
            pygame.draw.ellipse(self.surface, COLOR_BLACK, (x, y, w, h * 2 // 3), 2)
            # белые пятна
            pygame.draw.circle(self.surface, p.secondary_color,
                               (x + w // 4, y + h // 4), 4)
            pygame.draw.circle(self.surface, p.secondary_color,
                               (x + 3 * w // 4, y + h // 4), 4)
            # ножка
            pygame.draw.rect(self.surface, p.secondary_color,
                             (x + 4, y + h * 2 // 3 - 2, w - 8, h // 3))
            pygame.draw.rect(self.surface, COLOR_BLACK,
                             (x + 4, y + h * 2 // 3 - 2, w - 8, h // 3), 2)
        elif p.type_name == "star":
            # пятиконечная звезда
            cx, cy = x + w // 2, y + h // 2
            r1 = w // 2
            r2 = w // 4
            import math
            pts = []
            for i in range(10):
                angle = -math.pi / 2 + i * math.pi / 5
                r = r1 if i % 2 == 0 else r2
                pts.append((cx + math.cos(angle) * r, cy + math.sin(angle) * r))
            pygame.draw.polygon(self.surface, p.color, pts)
            pygame.draw.polygon(self.surface, COLOR_BLACK, pts, 2)
        elif p.type_name == "fireflower":
            # цветок с лепестками
            cx, cy = x + w // 2, y + h // 3
            for dx, dy in [(-8, -4), (8, -4), (-8, 4), (8, 4), (0, -10), (0, 10)]:
                pygame.draw.circle(self.surface, p.color, (cx + dx, cy + dy), 5)
            pygame.draw.circle(self.surface, p.secondary_color, (cx, cy), 5)
            # стебель
            pygame.draw.rect(self.surface, (0, 130, 0),
                             (x + w // 2 - 2, y + h // 2, 4, h // 2))

    # ----- Частицы -----
    def draw_particles(self, particles: List["Particle"], camera_x: int) -> None:
        for p in particles:
            if not p.alive:
                continue
            sx = int(p.x - camera_x)
            sy = int(p.y)
            if -10 < sx < self.screen_width + 10:
                pygame.draw.rect(self.surface, p.color,
                                 (sx, sy, p.size, p.size))

    # ----- HUD -----
    def draw_hud(self, score: int, lives: int, time_left: int,
                 coins: int = 0) -> None:
        # фон HUD
        hud_surf = pygame.Surface((self.screen_width, 36), pygame.SRCALPHA)
        hud_surf.fill((0, 0, 0, 130))
        self.surface.blit(hud_surf, (0, 0))

        text = self.font_small.render(
            f"SCORE {score:06d}", True, COLOR_WHITE)
        self.surface.blit(text, (12, 10))
        text = self.font_small.render(
            f"COINS x{coins:02d}", True, COLOR_QUESTION)
        self.surface.blit(text, (180, 10))
        text = self.font_small.render(
            f"LIVES {lives}", True, COLOR_WHITE)
        self.surface.blit(text, (340, 10))
        text = self.font_small.render(
            f"TIME {time_left:03d}", True, COLOR_WHITE)
        self.surface.blit(text, (480, 10))

    def draw_centered_text(self, text: str, y: int, font: pygame.font.Font,
                           color: Tuple[int, int, int] = COLOR_WHITE) -> None:
        rendered = font.render(text, True, color)
        rect = rendered.get_rect(center=(self.screen_width // 2, y))
        self.surface.blit(rendered, rect)
