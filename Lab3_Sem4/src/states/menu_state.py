"""
MenuState — the main menu screen.
Options: Start Game | High Scores | Help | Quit
"""
from __future__ import annotations

from typing import List
import pygame

from src.states.base_state import BaseState


class MenuState(BaseState):
    """Animated main menu."""

    _ITEMS: List[str] = ["Начать игру", "Таблица рекордов", "Справка", "Выход"]
    _SELECTED_COLOR = (255, 220, 0)
    _NORMAL_COLOR   = (255, 255, 255)
    _BG_TOP    = (10, 10, 80)
    _BG_BOTTOM = (20, 20, 140)

    def __init__(self, game) -> None:
        super().__init__(game)
        self._selected = 0
        self._assets = game.assets
        self._font_title = self._assets.get_font(52, bold=True)
        self._font_item  = self._assets.get_font(28)
        self._font_sub   = self._assets.get_font(14)
        self._tick = 0

        self._assets.play_music("menu", volume=0.45)

    def handle_event(self, event: pygame.event.Event) -> None:
        if event.type == pygame.KEYDOWN:
            if event.key in (pygame.K_UP, pygame.K_w):
                self._selected = (self._selected - 1) % len(self._ITEMS)
            elif event.key in (pygame.K_DOWN, pygame.K_s):
                self._selected = (self._selected + 1) % len(self._ITEMS)
            elif event.key in (pygame.K_RETURN, pygame.K_SPACE):
                self._confirm()

    def update(self) -> None:
        self._tick += 1

    def draw(self, surface: pygame.Surface) -> None:
        w, h = surface.get_size()

        # Gradient background
        for y in range(h):
            t = y / h
            r = int(self._BG_TOP[0] * (1 - t) + self._BG_BOTTOM[0] * t)
            g = int(self._BG_TOP[1] * (1 - t) + self._BG_BOTTOM[1] * t)
            b = int(self._BG_TOP[2] * (1 - t) + self._BG_BOTTOM[2] * t)
            pygame.draw.line(surface, (r, g, b), (0, y), (w, y))

        # Decorative ground blocks
        tile = 32
        for tx in range(0, w, tile):
            pygame.draw.rect(surface, (139, 90, 43),
                             (tx, h - tile, tile, tile))
            pygame.draw.rect(surface, (100, 60, 20),
                             (tx, h - tile, tile, tile), 1)

        # Title with shadow
        import math
        bob = int(math.sin(self._tick * 0.05) * 4)
        title = self._font_title.render("SUPER MARIO BROS", True, (255, 220, 0))
        shadow = self._font_title.render("SUPER MARIO BROS", True, (120, 80, 0))
        tx_pos = title.get_rect(centerx=w // 2, top=60 + bob)
        surface.blit(shadow, (tx_pos.x + 3, tx_pos.y + 3))
        surface.blit(title, tx_pos)

        subtitle = self._font_sub.render(
            "Лабораторная работа №3 — Вариант 3", True, (180, 180, 255))
        surface.blit(subtitle, subtitle.get_rect(centerx=w // 2, top=130))

        # Menu items
        start_y = 200
        for idx, label in enumerate(self._ITEMS):
            color = self._SELECTED_COLOR if idx == self._selected \
                else self._NORMAL_COLOR
            rendered = self._font_item.render(label, True, color)
            rect = rendered.get_rect(centerx=w // 2, top=start_y + idx * 52)
            if idx == self._selected:
                # Arrow indicator
                arrow = self._font_item.render("►", True, self._SELECTED_COLOR)
                surface.blit(arrow, (rect.left - 36, rect.top))
                # Highlight box
                box = rect.inflate(60, 8)
                pygame.draw.rect(surface, (255, 200, 0, 60), box, 2,
                                 border_radius=6)
            surface.blit(rendered, rect)

        # Controls hint
        hint = self._font_sub.render(
            "↑↓ — выбор    Enter/Space — подтвердить", True, (160, 160, 200))
        surface.blit(hint, hint.get_rect(centerx=w // 2, bottom=h - 12))

    # ------------------------------------------------------------------ #

    def _confirm(self) -> None:
        if self._selected == 0:
            self._assets.stop_music()
            self._game.push_state("gameplay")
        elif self._selected == 1:
            self._game.push_state("highscores")
        elif self._selected == 2:
            self._game.push_state("help")
        elif self._selected == 3:
            self._game.quit()
