"""Справка о правилах игры."""
from typing import List

import pygame

from .scene import Scene


class HelpScene(Scene):
    """Показывает текст с правилами."""

    LINES = [
        "ПРАВИЛА ИГРЫ",
        "",
        "Цель: добраться до флага в конце уровня.",
        "",
        "УПРАВЛЕНИЕ:",
        "  ←/→ или A/D — движение",
        "  ПРОБЕЛ или W или ↑ — прыжок",
        "  SHIFT — бег",
        "  ESC — пауза / выход в меню",
        "",
        "БОНУСЫ:",
        "  Гриб (красный) — превращение в большого Марио",
        "  Звезда (жёлтая) — неуязвимость на 6 секунд",
        "  Огненный цветок — ускорение на 8 секунд",
        "",
        "ОЧКИ:",
        "  Монета — 50 очков",
        "  Гумба (раздавлена сверху) — 100 очков",
        "  Купа — 200 очков",
        "  Бонус — 200-500 очков",
        "  За оставшееся время — 10 очков/сек",
        "",
        "ENTER — назад в меню",
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
