"""
HighScoresState — leaderboard display.
"""
from __future__ import annotations

import pygame
from src.states.base_state import BaseState


class HighScoresState(BaseState):

    def __init__(self, game) -> None:
        super().__init__(game)
        self._mgr   = game.scores_manager
        self._font_t = game.assets.get_font(34, bold=True)
        self._font   = game.assets.get_font(22)
        self._font_s = game.assets.get_font(14)
        self._tick   = 0

    def handle_event(self, event: pygame.event.Event) -> None:
        if event.type == pygame.KEYDOWN:
            if event.key in (pygame.K_ESCAPE, pygame.K_BACKSPACE,
                              pygame.K_RETURN, pygame.K_SPACE):
                self._game.pop_state()

    def update(self) -> None:
        self._tick += 1

    def draw(self, surface: pygame.Surface) -> None:
        import math
        surface.fill((10, 10, 60))
        w, h = surface.get_size()

        title = self._font_t.render("🏆  ТАБЛИЦА РЕКОРДОВ  🏆", True,
                                    (255, 220, 0))
        surface.blit(title, title.get_rect(centerx=w // 2, top=30))

        # Decorative line
        pygame.draw.line(surface, (255, 200, 0),
                         (60, 90), (w - 60, 90), 2)

        scores = self._mgr.scores
        col_colors = [(255, 215, 0), (192, 192, 192), (205, 127, 50)]

        for rank, entry in enumerate(scores[:10]):
            medal = "🥇" if rank == 0 else ("🥈" if rank == 1
                    else ("🥉" if rank == 2 else f"{rank + 1}."))
            text = f"{medal:4}  {entry['name']:<12}  {entry['score']:>8}"
            color = col_colors[rank] if rank < 3 else (200, 200, 200)

            bob = int(math.sin(self._tick * 0.06 + rank * 0.5) * 3) \
                  if rank < 3 else 0
            rendered = self._font.render(text, True, color)
            surface.blit(rendered,
                         rendered.get_rect(centerx=w // 2,
                                           top=110 + rank * 38 + bob))

        hint = self._font_s.render(
            "Нажми любую клавишу для возврата", True, (140, 140, 200))
        surface.blit(hint, hint.get_rect(centerx=w // 2, bottom=h - 12))
