"""Сцена таблицы рекордов."""
from typing import List

import pygame

from .scene import Scene


class HighScoresScene(Scene):
    """Отображает топ рекордов."""

    def on_enter(self) -> None:
        super().on_enter()
        self.app.highscores.reload()

    def handle_events(self, events: List) -> None:
        for e in events:
            if e.type == pygame.KEYDOWN:
                if e.key in (pygame.K_RETURN, pygame.K_ESCAPE,
                             pygame.K_SPACE, pygame.K_BACKSPACE):
                    self.next_scene = "menu"
                    self.done = True

    def draw(self) -> None:
        s = self.app.surface
        s.fill((25, 25, 60))
        self.app.renderer.draw_centered_text(
            "ТАБЛИЦА РЕКОРДОВ", 60,
            self.app.renderer.font_big, (255, 220, 0))

        scores = self.app.highscores.get_scores()
        if not scores:
            self.app.renderer.draw_centered_text(
                "ПОКА НЕТ РЕКОРДОВ", 200,
                self.app.renderer.font_med, (255, 255, 255))
        else:
            for i, entry in enumerate(scores):
                y = 160 + i * 50
                name = entry.get("name", "???")
                score = entry.get("score", 0)
                line = f"{i + 1}.  {name:<10s}  {score:>8d}"
                self.app.renderer.draw_centered_text(
                    line, y, self.app.renderer.font_med, (255, 255, 255))

        self.app.renderer.draw_centered_text(
            "ENTER — назад в меню",
            self.app.surface.get_height() - 40,
            self.app.renderer.font_small, (200, 200, 200))
