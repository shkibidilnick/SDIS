"""
NewRecordState — congratulations dialog with name entry.
"""
from __future__ import annotations

import pygame
from src.states.base_state import BaseState


class NewRecordState(BaseState):
    """Shown when player beats the top score."""

    _MAX_NAME = 10

    def __init__(self, game, score: int) -> None:
        super().__init__(game)
        self._score = score
        self._name  = ""
        self._tick  = 0
        self._font_t = game.assets.get_font(32, bold=True)
        self._font   = game.assets.get_font(22)
        self._font_s = game.assets.get_font(16)
        game.assets.play_sound("1up")

    def handle_event(self, event: pygame.event.Event) -> None:
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RETURN and self._name:
                self._save_and_exit()
            elif event.key == pygame.K_BACKSPACE:
                self._name = self._name[:-1]
            elif event.key == pygame.K_ESCAPE:
                # Skip saving
                self._game.pop_state()
                self._game.pop_state()  # also pop gameplay
                self._game.push_state("menu")
            else:
                char = event.unicode
                if char.isprintable() and len(self._name) < self._MAX_NAME:
                    self._name += char.upper()

    def update(self) -> None:
        self._tick += 1

    def draw(self, surface: pygame.Surface) -> None:
        import math
        surface.fill((10, 10, 60))
        w, h = surface.get_size()

        # Stars background
        for i in range(30):
            sx = (i * 73 + self._tick // 2) % w
            sy = (i * 47) % (h - 60)
            r  = 2 if i % 3 == 0 else 1
            pygame.draw.circle(surface, (255, 255, 150), (sx, sy), r)

        bob = int(math.sin(self._tick * 0.07) * 6)
        title = self._font_t.render("🏆 НОВЫЙ РЕКОРД! 🏆", True, (255, 220, 0))
        surface.blit(title, title.get_rect(centerx=w // 2, top=80 + bob))

        score_txt = self._font.render(
            f"Ваш счёт: {self._score:,}", True, (255, 255, 255))
        surface.blit(score_txt, score_txt.get_rect(centerx=w // 2, top=150))

        prompt = self._font.render("Введите имя:", True, (200, 200, 255))
        surface.blit(prompt, prompt.get_rect(centerx=w // 2, top=210))

        # Input box
        box_w, box_h = 280, 44
        box = pygame.Rect(w // 2 - box_w // 2, 250, box_w, box_h)
        pygame.draw.rect(surface, (30, 30, 100), box, border_radius=6)
        pygame.draw.rect(surface, (255, 220, 0), box, 2, border_radius=6)

        cursor = "_" if self._tick % 30 < 15 else ""
        name_surf = self._font.render(
            self._name + cursor, True, (255, 255, 255))
        surface.blit(name_surf, name_surf.get_rect(center=box.center))

        hint = self._font_s.render(
            "Enter — сохранить    Esc — пропустить", True, (140, 140, 200))
        surface.blit(hint, hint.get_rect(centerx=w // 2, bottom=h - 16))

    # ------------------------------------------------------------------ #

    def _save_and_exit(self) -> None:
        self._game.scores_manager.add_score(self._name, self._score)
        self._game.pop_state()  # pop new_record
        self._game.pop_state()  # pop gameplay
        self._game.push_state("highscores")
