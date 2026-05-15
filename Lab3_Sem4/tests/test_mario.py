"""Tests for Mario entity."""
import unittest
from unittest.mock import MagicMock, patch
import pygame

pygame.init()
pygame.display.set_mode((1, 1), pygame.NOFRAME)

from src.entities.mario import Mario, MarioState


def _make_settings():
    return {
        "mario": {
            "speed": 3, "run_speed": 5, "jump_force": -12,
            "lives": 3, "invincibility_duration": 120,
        },
        "gravity": 0.5,
    }


def _make_mario():
    assets = MagicMock()
    assets.get_font.return_value = pygame.font.SysFont("courier", 12)
    assets.play_sound = MagicMock()
    assets.get_surface.return_value = None
    return Mario(100, 100, _make_settings(), assets)


class TestMario(unittest.TestCase):

    def test_initial_state_small(self):
        m = _make_mario()
        self.assertEqual(m.state, MarioState.SMALL)

    def test_initial_lives(self):
        m = _make_mario()
        self.assertEqual(m.lives, 3)

    def test_initial_score_zero(self):
        m = _make_mario()
        self.assertEqual(m.score, 0)

    def test_initial_coins_zero(self):
        m = _make_mario()
        self.assertEqual(m.coins, 0)

    def test_add_score(self):
        m = _make_mario()
        m.add_score(500)
        self.assertEqual(m.score, 500)

    def test_add_coin_increments(self):
        m = _make_mario()
        m.add_coin()
        self.assertEqual(m.coins, 1)
        self.assertEqual(m.score, 200)

    def test_100_coins_gives_extra_life(self):
        m = _make_mario()
        for _ in range(100):
            m.add_coin()
        self.assertEqual(m.coins, 0)
        self.assertEqual(m.lives, 4)

    def test_power_up_grow(self):
        m = _make_mario()
        m.power_up("grow")
        self.assertEqual(m.state, MarioState.BIG)

    def test_power_up_fire(self):
        m = _make_mario()
        m.power_up("fire")
        self.assertEqual(m.state, MarioState.FIRE)

    def test_power_up_invincible(self):
        m = _make_mario()
        m.power_up("invincible")
        self.assertTrue(m.invincible)

    def test_take_damage_small_dies(self):
        m = _make_mario()
        m.take_damage()
        self.assertTrue(m.is_dead)

    def test_take_damage_big_shrinks(self):
        m = _make_mario()
        m.power_up("grow")
        m.take_damage()
        self.assertEqual(m.state, MarioState.SMALL)
        self.assertFalse(m.is_dead)

    def test_invincible_ignores_damage(self):
        m = _make_mario()
        m.invincible = True
        m._inv_timer = 60
        m.take_damage()
        self.assertFalse(m.is_dead)

    def test_die_decrements_lives(self):
        m = _make_mario()
        m.die()
        self.assertEqual(m.lives, 2)
        self.assertTrue(m.is_dead)

    def test_die_twice_no_double_decrement(self):
        m = _make_mario()
        m.die()
        m.die()
        self.assertEqual(m.lives, 2)

    def test_reset_position(self):
        m = _make_mario()
        m.die()
        m._dead_timer = 200
        m.reset_position(50, 50)
        self.assertFalse(m.is_dead)
        self.assertEqual(m.rect.x, 50)

    def test_not_dead_initially(self):
        m = _make_mario()
        self.assertFalse(m.is_dead)

    def test_dead_finished_false_initially(self):
        m = _make_mario()
        self.assertFalse(m.dead_finished)

    def test_grow_increases_height(self):
        m = _make_mario()
        small_h = m.rect.height
        m.power_up("grow")
        self.assertGreater(m.rect.height, small_h)


if __name__ == "__main__":
    unittest.main()
