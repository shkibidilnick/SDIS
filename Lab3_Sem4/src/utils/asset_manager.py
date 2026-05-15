"""
AssetManager — centralised loader for all game assets.
Single Responsibility: locate, load and cache pygame surfaces/sounds.
"""
from __future__ import annotations

import os
import pygame
from typing import Dict, Optional


class AssetManager:
    """Load and cache game assets (images, sounds, fonts)."""

    def __init__(self) -> None:
        self._surfaces: Dict[str, pygame.Surface] = {}
        self._sounds: Dict[str, Optional[pygame.mixer.Sound]] = {}
        self._fonts: Dict[tuple, pygame.font.Font] = {}
        self._base = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))

    # ------------------------------------------------------------------ #
    # Surfaces                                                             #
    # ------------------------------------------------------------------ #

    def get_surface(self, name: str) -> Optional[pygame.Surface]:
        """Return cached surface or None if asset is missing."""
        if name in self._surfaces:
            return self._surfaces[name]
        path = os.path.join(self._base, "assets", "images", f"{name}.png")
        if os.path.exists(path):
            surf = pygame.image.load(path).convert_alpha()
            self._surfaces[name] = surf
            return surf
        return None

    def get_or_create_surface(self, name: str, size: tuple,
                              color: tuple) -> pygame.Surface:
        """Return cached surface or create a solid-color placeholder."""
        surf = self.get_surface(name)
        if surf is not None:
            return surf
        placeholder = pygame.Surface(size, pygame.SRCALPHA)
        placeholder.fill(color)
        self._surfaces[name] = placeholder
        return placeholder

    # ------------------------------------------------------------------ #
    # Sounds                                                               #
    # ------------------------------------------------------------------ #

    def get_sound(self, name: str) -> Optional[pygame.mixer.Sound]:
        """Return cached sound or None if asset is missing / mixer off."""
        if name in self._sounds:
            return self._sounds[name]
        path = os.path.join(self._base, "assets", "sounds", f"{name}.wav")
        if os.path.exists(path):
            try:
                snd = pygame.mixer.Sound(path)
                self._sounds[name] = snd
                return snd
            except pygame.error:
                pass
        self._sounds[name] = None
        return None

    def play_sound(self, name: str, volume: float = 0.7) -> None:
        snd = self.get_sound(name)
        if snd:
            snd.set_volume(volume)
            snd.play()

    # ------------------------------------------------------------------ #
    # Music                                                                #
    # ------------------------------------------------------------------ #

    def play_music(self, name: str, volume: float = 0.5,
                   loops: int = -1) -> None:
        path = os.path.join(self._base, "assets", "music", f"{name}.ogg")
        if not os.path.exists(path):
            # fallback wav
            path = os.path.join(self._base, "assets", "music", f"{name}.wav")
        if os.path.exists(path):
            try:
                pygame.mixer.music.load(path)
                pygame.mixer.music.set_volume(volume)
                pygame.mixer.music.play(loops)
            except pygame.error:
                pass

    def stop_music(self) -> None:
        try:
            pygame.mixer.music.stop()
        except pygame.error:
            pass

    # ------------------------------------------------------------------ #
    # Fonts                                                                #
    # ------------------------------------------------------------------ #

    def get_font(self, size: int, bold: bool = False) -> pygame.font.Font:
        key = (size, bold)
        if key not in self._fonts:
            font_path = os.path.join(self._base, "assets", "fonts",
                                     "mario.ttf")
            if os.path.exists(font_path):
                self._fonts[key] = pygame.font.Font(font_path, size)
            else:
                self._fonts[key] = pygame.font.SysFont(
                    "couriernew" if bold else "courier", size, bold=bold
                )
        return self._fonts[key]
