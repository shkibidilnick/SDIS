"""
HUD — renders the in-game heads-up display.
Single Responsibility: display only; reads from Mario and game state.
"""
from __future__ import annotations

from typing import TYPE_CHECKING
import pygame

if TYPE_CHECKING:
    from src.entities.mario import Mario
    from src.utils.asset_manager import AssetManager


class HUD:
    """Draws score, lives, coins and timer each frame."""

    _BG_COLOR  = (0, 0, 0, 160)
    _TEXT_COLOR = (255, 255, 255)
    _WARN_COLOR = (255, 100, 0)

    def __init__(self, assets: "AssetManager", screen_w: int) -> None:
        self._assets = assets
        self._screen_w = screen_w
        self._font = assets.get_font(20, bold=True)
        self._small = assets.get_font(14)

    def draw(self, surface: pygame.Surface, mario: "Mario",
             time_left: int, level_name: str) -> None:
        # Semi-transparent top bar
        bar = pygame.Surface((self._screen_w, 40), pygame.SRCALPHA)
        bar.fill((0, 0, 0, 140))
        surface.blit(bar, (0, 0))

        score_txt  = f"SCORE  {mario.score:>07}"
        coins_txt  = f"🪙 x{mario.coins:02}"
        lives_txt  = f"MARIO x{mario.lives}"
        time_color = self._WARN_COLOR if time_left <= 100 else self._TEXT_COLOR
        time_txt   = f"TIME {time_left:>3}"
        world_txt  = level_name

        # Column positions
        self._blit(surface, score_txt, 10, 10, self._TEXT_COLOR)
        self._blit(surface, coins_txt, self._screen_w // 2 - 60, 10,
                   (255, 220, 0))
        self._blit(surface, world_txt, self._screen_w // 2 + 20, 10,
                   self._TEXT_COLOR)
        self._blit(surface, lives_txt, self._screen_w - 190, 10,
                   self._TEXT_COLOR)
        self._blit(surface, time_txt, self._screen_w - 90, 10, time_color)

    # ------------------------------------------------------------------ #

    def _blit(self, surface: pygame.Surface, text: str, x: int, y: int,
              color: tuple) -> None:
        rendered = self._font.render(text, True, color)
        surface.blit(rendered, (x, y))
