"""Главное меню."""
from typing import List

import pygame

from .scene import Scene


class MenuScene(Scene):
    """Главное меню: 'начать игру', 'таблица рекордов', 'справка', 'выход'."""

    OPTIONS = [
        ("НАЧАТЬ ИГРУ", "game"),
        ("ТАБЛИЦА РЕКОРДОВ", "highscores"),
        ("СПРАВКА", "help"),
        ("ВЫХОД", "quit"),
    ]

    def __init__(self, app):
        super().__init__(app)
        self.selected = 0
        self.title_anim = 0

    def on_enter(self) -> None:
        super().on_enter()
        self.app.audio.play_music()

    def handle_events(self, events: List) -> None:
        for e in events:
            if e.type == pygame.KEYDOWN:
                if e.key in (pygame.K_UP, pygame.K_w):
                    self.selected = (self.selected - 1) % len(self.OPTIONS)
                    self.app.audio.play_sfx("menu_select")
                elif e.key in (pygame.K_DOWN, pygame.K_s):
                    self.selected = (self.selected + 1) % len(self.OPTIONS)
                    self.app.audio.play_sfx("menu_select")
                elif e.key in (pygame.K_RETURN, pygame.K_SPACE):
                    label, target = self.OPTIONS[self.selected]
                    self.app.audio.play_sfx("menu_select")
                    if target == "quit":
                        self.app.running = False
                    else:
                        self.next_scene = target
                        self.done = True

    def update(self) -> None:
        self.title_anim += 1

    def draw(self) -> None:
        s = self.app.surface
        s.fill((92, 148, 252))
        # декоративные облака
        self.app.renderer.draw_clouds(0)

        # Заголовок (тень + цвет)
        title = "SUPER MARIO BROS"
        sub = "Lab 3, Sem 4"
        offset = int((self.title_anim % 60) / 60 * 4 - 2)
        self.app.renderer.draw_centered_text(
            title, 110 + offset, self.app.renderer.font_huge, (255, 240, 0))
        self.app.renderer.draw_centered_text(
            sub, 160, self.app.renderer.font_med, (255, 255, 255))

        # пункты меню
        for i, (label, _) in enumerate(self.OPTIONS):
            color = (255, 255, 0) if i == self.selected else (255, 255, 255)
            prefix = "> " if i == self.selected else "  "
            self.app.renderer.draw_centered_text(
                prefix + label, 260 + i * 50,
                self.app.renderer.font_med, color)

        # подсказка
        self.app.renderer.draw_centered_text(
            "↑/↓ — выбор   ENTER — подтвердить",
            self.app.surface.get_height() - 40,
            self.app.renderer.font_small, (220, 220, 220))
