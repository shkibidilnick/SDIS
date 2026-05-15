"""
HelpState — displays game rules and controls.
"""
from __future__ import annotations

import pygame
from src.states.base_state import BaseState


_HELP_TEXT = [
    ("SUPER MARIO BROS — СПРАВКА", True),
    ("", False),
    ("УПРАВЛЕНИЕ:", True),
    ("  ← → / A D    — движение", False),
    ("  Пробел / ↑ / W  — прыжок", False),
    ("  Shift          — бег", False),
    ("  Z              — огонь (огненный цветок)", False),
    ("", False),
    ("БОНУСЫ:", True),
    ("  🍄 Гриб       — Марио вырастает", False),
    ("  🌸 Цветок     — огненные шары", False),
    ("  ⭐ Звезда     — временная неуязвимость", False),
    ("  🪙 Монета     — +200 очков (100 = доп. жизнь)", False),
    ("", False),
    ("ВРАГИ:", True),
    ("  Гумба  — прыгни сверху, чтобы убить", False),
    ("  Купа   — прыгни → панцирь, ещё раз → удар", False),
    ("", False),
    ("ЦЕЛЬ:", True),
    ("  Дойди до флагштока в конце уровня!", False),
    ("  Успей до истечения времени.", False),
    ("", False),
    ("Нажми Backspace или Escape для возврата", True),
]


class HelpState(BaseState):

    def __init__(self, game) -> None:
        super().__init__(game)
        self._font_h = game.assets.get_font(22, bold=True)
        self._font   = game.assets.get_font(17)
        self._scroll  = 0

    def handle_event(self, event: pygame.event.Event) -> None:
        if event.type == pygame.KEYDOWN:
            if event.key in (pygame.K_ESCAPE, pygame.K_BACKSPACE):
                self._game.pop_state()
            elif event.key == pygame.K_DOWN:
                self._scroll += 24
            elif event.key == pygame.K_UP:
                self._scroll = max(0, self._scroll - 24)

    def update(self) -> None:
        pass

    def draw(self, surface: pygame.Surface) -> None:
        surface.fill((15, 15, 50))
        w = surface.get_width()
        y = 20 - self._scroll
        for line, bold in _HELP_TEXT:
            font = self._font_h if bold else self._font
            color = (255, 220, 0) if bold else (220, 220, 220)
            rendered = font.render(line, True, color)
            surface.blit(rendered, rendered.get_rect(centerx=w // 2, top=y))
            y += rendered.get_height() + 6
