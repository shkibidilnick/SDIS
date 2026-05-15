"""
GameOverState — shown when Mario runs out of lives.
"""
from __future__ import annotations

import pygame
from src.states.base_state import BaseState


class GameOverState(BaseState):

    def __init__(self, game, score: int) -> None:
        super().__init__(game)
        self._score = score
        self._tick  = 0
        self._font_t = game.assets.get_font(48, bold=True)
        self._font   = game.assets.get_font(22)
        self._font_s = game.assets.get_font(14)
        game.assets.stop_music()
        game.assets.play_sound("gameover")

    def handle_event(self, event: pygame.event.Event) -> None:
        if event.type == pygame.KEYDOWN and self._tick > 90:
            if event.key in (pygame.K_RETURN, pygame.K_SPACE,
                              pygame.K_ESCAPE):
                self._exit()

    def update(self) -> None:
        self._tick += 1

    def draw(self, surface: pygame.Surface) -> None:
        surface.fill((0, 0, 0))
        w, h = surface.get_size()

        alpha = min(255, self._tick * 4)
        title = self._font_t.render("GAME OVER", True, (220, 30, 30))
        title.set_alpha(alpha)
        surface.blit(title, title.get_rect(centerx=w // 2, top=180))

        score_surf = self._font.render(
            f"Итоговый счёт: {self._score:,}", True, (255, 255, 255))
        score_surf.set_alpha(alpha)
        surface.blit(score_surf, score_surf.get_rect(centerx=w // 2, top=270))

        if self._tick > 90:
            hint = self._font_s.render(
                "Нажми Enter для возврата в меню", True, (180, 180, 180))
            surface.blit(hint, hint.get_rect(centerx=w // 2, top=340))

    # ------------------------------------------------------------------ #

    def _exit(self) -> None:
        self._game.pop_state()  # pop game_over
        self._game.pop_state()  # pop gameplay
        self._game.push_state("menu")
