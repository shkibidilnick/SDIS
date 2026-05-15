"""Tests for bonus entities."""
import unittest
from unittest.mock import MagicMock
import pygame

pygame.init()
pygame.display.set_mode((1, 1), pygame.NOFRAME)

from src.entities.bonuses import Mushroom, FireFlower, Star, Coin
from src.entities.mario import Mario, MarioState


def _assets():
    a = MagicMock()
    a.play_sound = MagicMock()
    a.get_surface.return_value = None
    a.get_font.return_value = pygame.font.SysFont("courier", 12)
    return a


def _mario():
    settings = {
        "mario": {"speed": 3, "run_speed": 5, "jump_force": -12,
                  "lives": 3, "invincibility_duration": 120},
        "gravity": 0.5,
    }
    return Mario(0, 0, settings, _assets())


class TestMushroom(unittest.TestCase):

    def test_collect_grows_mario(self):
        m = _mario()
        mushroom = Mushroom(0, 0, _assets())
        mushroom._emerging = False
        mushroom.collect(m)
        self.assertEqual(m.state, MarioState.BIG)

    def test_collect_marks_dead(self):
        mushroom = Mushroom(0, 0, _assets())
        mushroom._emerging = False
        mushroom.collect(_mario())
        self.assertFalse(mushroom.alive)

    def test_effect_is_grow(self):
        mushroom = Mushroom(0, 0, _assets())
        self.assertEqual(mushroom.effect, "grow")

    def test_moves_after_emergence(self):
        mushroom = Mushroom(100, 100, _assets(), gravity=0.0)
        mushroom._emerging = False
        initial_x = mushroom.rect.x
        mushroom.update([])
        # Should have moved (vel_x != 0)
        self.assertNotEqual(mushroom.rect.x, initial_x)


class TestFireFlower(unittest.TestCase):

    def test_effect_is_fire(self):
        ff = FireFlower(0, 0, _assets())
        self.assertEqual(ff.effect, "fire")

    def test_collect_gives_fire(self):
        m = _mario()
        ff = FireFlower(0, 0, _assets())
        ff._emerging = False
        ff.collect(m)
        self.assertEqual(m.state, MarioState.FIRE)

    def test_collect_marks_dead(self):
        ff = FireFlower(0, 0, _assets())
        ff._emerging = False
        ff.collect(_mario())
        self.assertFalse(ff.alive)


class TestStar(unittest.TestCase):

    def test_effect_is_invincible(self):
        star = Star(0, 0, _assets())
        self.assertEqual(star.effect, "invincible")

    def test_collect_makes_invincible(self):
        m = _mario()
        star = Star(0, 0, _assets(), gravity=0.0)
        star._emerging = False
        star.collect(m)
        self.assertTrue(m.invincible)

    def test_collect_marks_dead(self):
        star = Star(0, 0, _assets(), gravity=0.0)
        star._emerging = False
        star.collect(_mario())
        self.assertFalse(star.alive)


class TestCoin(unittest.TestCase):

    def test_effect_is_coin(self):
        coin = Coin(0, 0, _assets())
        self.assertEqual(coin.effect, "coin")

    def test_collect_adds_score(self):
        m = _mario()
        coin = Coin(0, 0, _assets())
        coin.collect(m)
        self.assertGreater(m.score, 0)

    def test_collect_increments_coins(self):
        m = _mario()
        coin = Coin(0, 0, _assets())
        coin.collect(m)
        self.assertEqual(m.coins, 1)

    def test_collect_marks_dead(self):
        coin = Coin(0, 0, _assets())
        coin.collect(_mario())
        self.assertFalse(coin.alive)

    def test_not_emerging_by_default(self):
        coin = Coin(0, 0, _assets())
        self.assertFalse(coin._emerging)


if __name__ == "__main__":
    unittest.main()
