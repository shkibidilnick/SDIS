"""Тесты для PhysicsEntity и базовой физики."""
import os
import sys
import unittest

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from src.entities.level import Level  # noqa: E402
from src.entities.physics_entity import PhysicsEntity  # noqa: E402
from tests.test_level import make_level_data  # noqa: E402


class TestPhysicsEntity(unittest.TestCase):
    def test_apply_gravity_increments_velocity(self):
        e = PhysicsEntity(0, 0, 10, 10)
        e.apply_gravity(1.0, 100)
        self.assertEqual(e.vy, 1.0)
        e.apply_gravity(1.0, 100)
        self.assertEqual(e.vy, 2.0)

    def test_apply_gravity_clamps(self):
        e = PhysicsEntity(0, 0, 10, 10)
        e.vy = 20
        e.apply_gravity(5, 12)
        self.assertEqual(e.vy, 12)

    def test_collision_horizontal_right(self):
        # столбец твёрдой стены справа
        data = make_level_data(["..G", "..G", "..G"], w=3, h=3)
        lvl = Level(data, tile_size=10)
        # объект 0..10, скорость 15 → попадёт в стену x=20..30
        e = PhysicsEntity(0, 0, 10, 10)
        e.vx = 15
        result = e.move_and_collide(lvl)
        self.assertTrue(result["hit_right"])
        self.assertEqual(e.rect.right, 20)
        self.assertEqual(e.vx, 0)

    def test_collision_horizontal_left(self):
        data = make_level_data(["G..", "G..", "G.."], w=3, h=3)
        lvl = Level(data, tile_size=10)
        # объект x=15..25, скорость -10 → упрётся в стену x=0..10
        e = PhysicsEntity(15, 0, 10, 10)
        e.vx = -10
        result = e.move_and_collide(lvl)
        self.assertTrue(result["hit_left"])
        self.assertEqual(e.rect.left, 10)
        self.assertEqual(e.vx, 0)

    def test_collision_landing(self):
        data = make_level_data(["...", "...", "GGG"], w=3, h=3)
        lvl = Level(data, tile_size=10)
        e = PhysicsEntity(5, 0, 10, 10)
        e.vy = 15
        result = e.move_and_collide(lvl)
        self.assertTrue(result["hit_bottom"])
        self.assertTrue(e.on_ground)
        self.assertEqual(e.rect.bottom, 20)
        self.assertEqual(e.vy, 0)

    def test_collision_hit_ceiling(self):
        data = make_level_data(["GGG", "...", "..."], w=3, h=3)
        lvl = Level(data, tile_size=10)
        e = PhysicsEntity(5, 15, 10, 10)
        e.vy = -10
        result = e.move_and_collide(lvl)
        self.assertTrue(result["hit_top"])
        self.assertEqual(e.rect.top, 10)
        self.assertEqual(e.vy, 0)

    def test_no_collision_in_empty(self):
        lvl = Level(make_level_data(["...", "...", "..."], w=3, h=3), tile_size=10)
        e = PhysicsEntity(5, 5, 10, 10)
        e.vx = 1
        e.vy = 1
        r = e.move_and_collide(lvl)
        self.assertFalse(r["hit_left"])
        self.assertFalse(r["hit_right"])
        self.assertFalse(r["hit_top"])
        self.assertFalse(r["hit_bottom"])
        self.assertEqual(e.rect.x, 6)
        self.assertEqual(e.rect.y, 6)


if __name__ == "__main__":
    unittest.main()
