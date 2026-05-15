"""
Super Mario Bros - Lab3_Sem4
Entry point of the application.
"""
import sys
import pygame
from src.game import Game


def main() -> None:
    """Initialize pygame and start the game."""
    pygame.init()
    pygame.mixer.init(frequency=44100, size=-16, channels=2, buffer=512)

    game = Game()
    game.run()

    pygame.quit()
    sys.exit()


if __name__ == "__main__":
    main()
