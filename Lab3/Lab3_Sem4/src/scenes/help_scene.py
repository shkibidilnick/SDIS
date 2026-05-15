"""Справка о правилах игры."""
from typing import List

import pygame

from .scene import Scene


class HelpScene(Scene):
    """Показывает текст с правилами."""

    LINES = [
        "ПРАВИЛА ИГРЫ",
        "",
        "Цель: добраться до флага за 300 сек. Жизней: 3.",
        "",
        "УПРАВЛЕНИЕ:",
        "  ←/→ или A/D — движение          SHIFT — бег",
        "  ПРОБЕЛ / W / ↑ — прыжок         ESC — выход в меню",
        "",
        "ВРАГИ:",
        "  Гумба — Передвигается. Задавливается большим Марио",
        "  Купа — Передвигается быстрее гумбы, тоже избегает обрывов.",
        "  Пиранья — в трубе, периодически высовывается.",
        "",
        "БОНУСЫ (удар по блоку ? снизу):",
        "  Гриб (красный)     — большой Марио 20 сек      200 очков",
        "  Цветок (оранжевый) — ускорение 12 сек          300 очков",
        "  Звезда (жёлтая)    — неуязвимость 10 сек       500 очков",
        "",
        "ОЧКИ:",
        "  Монета — 50       Гумба — 100       Купа/Пиранья — 200",
        "  Завершение уровня — 1000            Время — 10/сек от оставшегося времени",
        "",
        "ENTER / ESC / ПРОБЕЛ — назад в меню",
    ]

    def handle_events(self, events: List) -> None:
        for e in events:
            if e.type == pygame.KEYDOWN:
                if e.key in (pygame.K_RETURN, pygame.K_ESCAPE,
                             pygame.K_SPACE, pygame.K_BACKSPACE):
                    self.next_scene = "menu"
                    self.done = True

    def draw(self) -> None:
        s = self.app.surface
        s.fill((20, 30, 50))
        y = 30
        for i, line in enumerate(self.LINES):
            font = self.app.renderer.font_med if i == 0 else self.app.renderer.font_small
            color = (255, 220, 0) if i == 0 else (240, 240, 240)
            self.app.renderer.draw_centered_text(line, y, font, color)
            y += 22 if i != 0 else 36