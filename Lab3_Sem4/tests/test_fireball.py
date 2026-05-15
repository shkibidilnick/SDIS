"""Tests for Fireball projectile."""
import unittest
from unittest.mock import MagicMock
import pygame

pygame.init()
pygame.display.set_mode((1, 1), pygame.NOFRAME)

from src.entities.mario import Fireball


def _assets():
    a = MagicMock()
    a.play_sound = MagicMock()
    a.get_surface.return_value = None
    return a


class TestFireball(unittest.TestCase):

    def test_moves_right(self):
        fb = Fireball(100, 100, 1, _assets())
        initial_x = fb.rect.x
        fb.update([])
        self.assertGreater(fb.rect.x, initial_x)

    def test_moves_left(self):
        fb = Fireball(100, 100, -1, _assets())
        initial_x = fb.rect.x
        fb.update([])
        self.assertLess(fb.rect.x, initial_x)

    def test_alive_initially(self):
        fb = Fireball(0, 0, 1, _assets())
        self.assertTrue(fb.alive)

    def test_dies_on_wall(self):
        fb = Fireball(90, 50, 1, _assets())
        wall = pygame.Rect(100, 0, 50, 200)
        fb.update([wall])
        self.assertFalse(fb.alive)

    def test_bounces_off_ground(self):
        fb = Fireball(0, 90, 1, _assets())
        ground = pygame.Rect(0, 100, 500, 50)
        fb.update([ground])
        self.assertLess(fb.vel_y, 0)  # bounced up

    def test_dies_after_max_bounces(self):
        fb = Fireball(0, 0, 1, _assets())
        fb._bounces = 5
        ground = pygame.Rect(0, 30, 500, 50)
        fb.update([ground])
        self.assertFalse(fb.alive)

    def test_size(self):
        fb = Fireball(0, 0, 1, _assets())
        self.assertEqual(fb.rect.width, 10)
        self.assertEqual(fb.rect.height, 10)


if __name__ == "__main__":
    unittest.main()
