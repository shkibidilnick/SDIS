"""Главный класс приложения. Управляет сценами и главным циклом."""
import os
import sys

import pygame

from .scenes.game_scene import GameScene
from .scenes.help_scene import HelpScene
from .scenes.highscores_scene import HighScoresScene
from .scenes.menu_scene import MenuScene
from .scenes.name_entry_scene import NameEntryScene
from .utils.audio_manager import AudioManager
from .utils.config_loader import ConfigLoader
from .utils.highscore_manager import HighScoreManager
from .utils.renderer import Renderer


class App:
    """Главный класс игры."""

    def __init__(self, base_path: str = ""):
        # путь, относительно которого ищутся конфиги
        self.base_path = base_path or os.path.dirname(os.path.dirname(
            os.path.abspath(__file__)))
        self.config_loader = ConfigLoader(self.base_path)
        self.config = self.config_loader.load("config/settings.json")
        self.surface = None
        self.clock = None
        self.renderer = None
        self.audio = None
        self.highscores = None
        self.scenes = {}
        self.current_scene = None
        self.running = False
        self.pending_score = 0

    def init_pygame(self) -> None:
        pygame.init()
        screen = self.config["screen"]
        self.surface = pygame.display.set_mode((screen["width"], screen["height"]))
        pygame.display.set_caption(screen["title"])
        self.clock = pygame.time.Clock()
        self.renderer = Renderer(self.surface, screen["width"], screen["height"],
                                 self.config["tile"]["size"])

    def init_systems(self) -> None:
        self.audio = AudioManager(
            music_volume=self.config["audio"]["music_volume"],
            sfx_volume=self.config["audio"]["sfx_volume"],
        )
        sounds_dir = os.path.join(self.base_path, "assets", "sounds")
        self.audio.init(sounds_dir)
        self.highscores = HighScoreManager(
            self.config["files"]["highscores"], self.config_loader)

    def init_scenes(self) -> None:
        self.scenes = {
            "menu": MenuScene(self),
            "game": GameScene(self),
            "help": HelpScene(self),
            "highscores": HighScoresScene(self),
            "name_entry": NameEntryScene(self),
        }

    def run(self) -> None:
        self.init_pygame()
        self.init_systems()
        self.init_scenes()
        self.running = True
        self._switch_to("menu")
        fps = self.config["screen"]["fps"]

        while self.running:
            events = pygame.event.get()
            for e in events:
                if e.type == pygame.QUIT:
                    self.running = False
            if not self.running:
                break

            self.current_scene.handle_events(events)
            self.current_scene.update()
            self.current_scene.draw()

            pygame.display.flip()
            self.clock.tick(fps)

            if self.current_scene.done and self.current_scene.next_scene:
                self._switch_to(self.current_scene.next_scene)

        self.audio.stop_music()
        pygame.quit()

    def _switch_to(self, scene_name: str) -> None:
        if self.current_scene is not None:
            self.current_scene.on_exit()
        scene = self.scenes.get(scene_name)
        if scene is None:
            self.running = False
            return
        self.current_scene = scene
        scene.on_enter()


def main():
    app = App()
    try:
        app.run()
    except KeyboardInterrupt:
        pass
    sys.exit(0)


if __name__ == "__main__":
    main()
