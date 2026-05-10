"""Сцена ввода имени для нового рекорда."""
from typing import List

import pygame

from .scene import Scene


class NameEntryScene(Scene):
    """Поздравление и ввод имени для записи в таблицу рекордов."""

    MAX_LEN = 10

    def __init__(self, app):
        super().__init__(app)
        self.name = ""
        self.blink_timer = 0

    def on_enter(self) -> None:
        super().on_enter()
        self.name = ""

    def handle_events(self, events: List) -> None:
        for e in events:
            if e.type == pygame.KEYDOWN:
                if e.key == pygame.K_RETURN:
                    self._submit()
                elif e.key == pygame.K_BACKSPACE:
                    self.name = self.name[:-1]
                elif e.key == pygame.K_ESCAPE:
                    # отменить — ввести как ANON
                    self.name = ""
                    self._submit()
                else:
                    if e.unicode and len(self.name) < self.MAX_LEN:
                        ch = e.unicode.upper()
                        if ch.isalnum() or ch in "._- ":
                            self.name += ch

    def _submit(self) -> None:
        score = getattr(self.app, "pending_score", 0)
        final_name = self.name.strip() or "ANON"
        self.app.highscores.add_score(final_name, score)
        self.app.pending_score = 0
        self.next_scene = "highscores"
        self.done = True

    def update(self) -> None:
        self.blink_timer += 1

    def draw(self) -> None:
        s = self.app.surface
        s.fill((50, 20, 80))
        self.app.renderer.draw_centered_text(
            "НОВЫЙ РЕКОРД!", 80,
            self.app.renderer.font_huge, (255, 220, 0))

        score = getattr(self.app, "pending_score", 0)
        self.app.renderer.draw_centered_text(
            f"СЧЁТ: {score}", 170,
            self.app.renderer.font_big, (255, 255, 255))

        self.app.renderer.draw_centered_text(
            "ВВЕДИТЕ ИМЯ:", 280,
            self.app.renderer.font_med, (255, 255, 255))

        # каретка
        cursor = "_" if (self.blink_timer // 30) % 2 == 0 else " "
        display_name = (self.name if self.name else "") + cursor
        self.app.renderer.draw_centered_text(
            display_name, 340,
            self.app.renderer.font_huge, (255, 255, 0))

        self.app.renderer.draw_centered_text(
            "ENTER — подтвердить    BACKSPACE — стереть    ESC — пропустить",
            self.app.surface.get_height() - 40,
            self.app.renderer.font_small, (200, 200, 200))
