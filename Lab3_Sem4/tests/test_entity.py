"""Tests for Entity base physics."""
import unittest
import pygame

pygame.init()
pygame.display.set_mode((1, 1), pygame.NOFRAME)

from src.entities.entity import Entity


class _ConcreteEntity(Entity):
    """Minimal concrete subclass for testing."""
    def update(self, *a, **kw): pass
    def draw(self, *a, **kw): pass


class TestEntity(unittest.TestCase):

    def test_initial_position(self):
        e = _ConcreteEntity(10, 20, 24, 28)
        self.assertEqual(e.rect.x, 10)
        self.assertEqual(e.rect.y, 20)

    def test_initial_velocity_zero(self):
        e = _ConcreteEntity(0, 0, 10, 10)
        self.assertEqual(e.vel_x, 0)
        self.assertEqual(e.vel_y, 0)

    def test_alive_initially(self):
        e = _ConcreteEntity(0, 0, 10, 10)
        self.assertTrue(e.alive)

    def test_apply_gravity_increases_vel_y(self):
        e = _ConcreteEntity(0, 0, 10, 10)
        e.apply_gravity(0.5)
        self.assertAlmostEqual(e.vel_y, 0.5)

    def test_apply_gravity_capped(self):
        e = _ConcreteEntity(0, 0, 10, 10)
        e.vel_y = 14.0
        e.apply_gravity(0.5, max_fall=15.0)
        self.assertLessEqual(e.vel_y, 15.0)

    def test_move_x_no_collision(self):
        e = _ConcreteEntity(100, 0, 10, 10)
        e.vel_x = 5
        e.move_x([])
        self.assertEqual(e.rect.x, 105)

    def test_move_x_stops_on_wall(self):
        e = _ConcreteEntity(90, 0, 10, 10)
        e.vel_x = 10
        wall = pygame.Rect(100, 0, 50, 50)
        e.move_x([wall])
        self.assertEqual(e.rect.right, wall.left)
        self.assertEqual(e.vel_x, 0)

    def test_move_y_no_collision(self):
        e = _ConcreteEntity(0, 100, 10, 10)
        e.vel_y = 3
        e.move_y([])
        self.assertEqual(e.rect.y, 103)

    def test_move_y_lands_on_ground(self):
        e = _ConcreteEntity(0, 90, 10, 10)
        e.vel_y = 10
        ground = pygame.Rect(0, 100, 200, 50)
        e.move_y([ground])
        self.assertEqual(e.rect.bottom, ground.top)
        self.assertTrue(e.on_ground)
        self.assertEqual(e.vel_y, 0)

    def test_on_ground_false_when_airborne(self):
        e = _ConcreteEntity(0, 0, 10, 10)
        e.vel_y = -5
        e.move_y([])
        self.assertFalse(e.on_ground)

    def test_head_bump_sets_vel_y_zero(self):
        e = _ConcreteEntity(0, 60, 10, 10)
        e.vel_y = -10
        ceiling = pygame.Rect(0, 50, 200, 10)
        e.move_y([ceiling])
        self.assertEqual(e.vel_y, 0)


if __name__ == "__main__":
    unittest.main()
