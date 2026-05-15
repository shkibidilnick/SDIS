"""Tests for Goomba and Koopa enemies."""
import unittest
from unittest.mock import MagicMock
import pygame

pygame.init()
pygame.display.set_mode((1, 1), pygame.NOFRAME)

from src.entities.enemies import Goomba, Koopa, EnemyState


def _assets():
    a = MagicMock()
    a.play_sound = MagicMock()
    a.get_surface.return_value = None
    return a


def _goomba_cfg():
    return {
        "speed": 1.0, "width": 28, "height": 28,
        "death_duration": 30, "walk_animation_speed": 10,
        "stomp_score": 100,
    }


def _koopa_cfg():
    return {
        "speed": 1.2, "width": 28, "height": 32,
        "shell_height": 22, "shell_speed": 6,
        "walk_animation_speed": 12, "stomp_score": 200,
    }


class TestGoomba(unittest.TestCase):

    def test_initial_state_walk(self):
        g = Goomba(0, 0, _goomba_cfg(), _assets(), 0.5)
        self.assertEqual(g.state, EnemyState.WALK)

    def test_initial_alive(self):
        g = Goomba(0, 0, _goomba_cfg(), _assets(), 0.5)
        self.assertTrue(g.alive)

    def test_stomp_changes_state(self):
        g = Goomba(0, 0, _goomba_cfg(), _assets(), 0.5)
        g.stomp(lambda pts: None)
        self.assertEqual(g.state, EnemyState.DYING)

    def test_stomp_calls_score_callback(self):
        g = Goomba(0, 0, _goomba_cfg(), _assets(), 0.5)
        called = []
        g.stomp(lambda pts: called.append(pts))
        self.assertEqual(called, [100])

    def test_death_timer_makes_dead(self):
        g = Goomba(0, 0, _goomba_cfg(), _assets(), 0.5)
        g.stomp(lambda pts: None)
        for _ in range(35):
            g.update([])
        self.assertFalse(g.alive)

    def test_stomp_twice_does_nothing_second_time(self):
        g = Goomba(0, 0, _goomba_cfg(), _assets(), 0.5)
        g.stomp(lambda pts: None)
        called = []
        g.stomp(lambda pts: called.append(pts))
        self.assertEqual(called, [])

    def test_walks_left_initially(self):
        g = Goomba(0, 0, _goomba_cfg(), _assets(), 0.0)
        self.assertLess(g.vel_x, 0)

    def test_reverses_on_wall(self):
        g = Goomba(90, 0, _goomba_cfg(), _assets(), 0.0)
        wall = pygame.Rect(100, 0, 50, 50)
        g.vel_x = 5
        g.update([wall])
        self.assertLess(g.vel_x, 0)


class TestKoopa(unittest.TestCase):

    def test_initial_state_walk(self):
        k = Koopa(0, 0, _koopa_cfg(), _assets(), 0.5)
        self.assertEqual(k.state, EnemyState.WALK)

    def test_stomp_becomes_shell(self):
        k = Koopa(0, 0, _koopa_cfg(), _assets(), 0.5)
        k.stomp(lambda pts: None)
        self.assertEqual(k.state, EnemyState.SHELL)

    def test_kick_shell_starts_moving(self):
        k = Koopa(0, 0, _koopa_cfg(), _assets(), 0.5)
        k.stomp(lambda pts: None)        # → SHELL
        k.stomp(lambda pts: None, from_right=False)   # → SHELL_MOVING
        self.assertEqual(k.state, EnemyState.SHELL_MOVING)

    def test_shell_kick_from_right_moves_left(self):
        k = Koopa(0, 0, _koopa_cfg(), _assets(), 0.5)
        k.stomp(lambda pts: None)
        k.stomp(lambda pts: None, from_right=True)
        self.assertLess(k.vel_x, 0)

    def test_kill_removes_from_alive(self):
        k = Koopa(0, 0, _koopa_cfg(), _assets(), 0.5)
        k.kill()
        self.assertFalse(k.alive)

    def test_shell_height_reduced(self):
        k = Koopa(0, 0, _koopa_cfg(), _assets(), 0.5)
        full_h = k.rect.height
        k.stomp(lambda pts: None)
        self.assertLess(k.rect.height, full_h)


if __name__ == "__main__":
    unittest.main()
