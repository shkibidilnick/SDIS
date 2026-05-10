"""Тесты для PowerUp и PowerUpFactory."""
import os
import sys
import unittest

# гарантируем доступность пакета src без установки
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from src.entities.powerup import PowerUp, PowerUpFactory  # noqa: E402
from src.entities.level import Level  # noqa: E402
from tests.test_level import make_level_data  # noqa: E402


def _mushroom_cfg():
    return {
        "type": "mushroom",
        "name": "Гриб",
        "width": 28,
        "height": 28,
        "color": [220, 40, 40],
        "secondary_color": [255, 255, 255],
        "speed": 1.5,
        "score_value": 200,
        "effect": "grow",
        "duration": 0,
    }


def _star_cfg():
    return {
        "type": "star",
        "name": "Звезда",
        "width": 26,
        "height": 26,
        "color": [255, 215, 0],
        "secondary_color": [255, 255, 0],
        "speed": 1.5,
        "score_value": 500,
        "effect": "invincible",
        "duration": 360,
    }


def _flower_cfg():
    return {
        "type": "fireflower",
        "name": "Огненный цветок",
        "width": 28,
        "height": 28,
        "color": [240, 120, 0],
        "secondary_color": [255, 220, 0],
        "speed": 0,
        "score_value": 300,
        "effect": "speed_boost",
        "duration": 480,
    }


def _empty_level(width=20, height=15):
    rows = ["." * width for _ in range(height)]
    return Level(make_level_data(rows, w=width, h=height))


def _level_with_floor(width=20, height=15, floor_row=14):
    rows = ["." * width for _ in range(height)]
    rows[floor_row] = "G" * width
    return Level(make_level_data(rows, w=width, h=height))


class TestPowerUpEmerge(unittest.TestCase):
    """Анимация появления бонуса из блока."""

    def setUp(self):
        self.gravity = 0.6
        self.max_fall = 12.0

    def test_emerges_upward_over_frames(self):
        p = PowerUp(96, 320, _mushroom_cfg(), self.gravity, self.max_fall)
        self.assertTrue(p.emerging)
        start_y = p.rect.y
        level = _empty_level()
        for _ in range(PowerUp.EMERGE_FRAMES):
            p.update(level)
        self.assertFalse(p.emerging)
        self.assertLess(p.rect.y, start_y)
        self.assertAlmostEqual(p.rect.y, start_y - p.rect.h, delta=1)

    def test_emerging_does_not_apply_gravity(self):
        p = PowerUp(96, 320, _mushroom_cfg(), self.gravity, self.max_fall)
        level = _empty_level()
        p.update(level)
        self.assertEqual(p.vy, 0.0)


class TestPowerUpMovement(unittest.TestCase):
    """Движение и поведение после эмерджа."""

    def setUp(self):
        self.gravity = 0.6
        self.max_fall = 12.0

    def test_mushroom_walks_horizontally(self):
        p = PowerUp(96, 32 * 12, _mushroom_cfg(), self.gravity, self.max_fall)
        level = _level_with_floor()
        for _ in range(PowerUp.EMERGE_FRAMES):
            p.update(level)
        x_before = p.rect.x
        for _ in range(20):
            p.update(level)
        self.assertGreater(p.rect.x, x_before)

    def test_star_bounces_when_on_ground(self):
        p = PowerUp(96, 32 * 12, _star_cfg(), self.gravity, self.max_fall)
        level = _level_with_floor()
        for _ in range(PowerUp.EMERGE_FRAMES):
            p.update(level)
        bounced = False
        for _ in range(200):
            p.update(level)
            if p.vy < 0:
                bounced = True
                break
        self.assertTrue(bounced, "Звезда должна подпрыгивать")

    def test_dies_when_falls_below_world(self):
        level = _empty_level(width=20, height=5)
        p = PowerUp(96, 32, _mushroom_cfg(), self.gravity, self.max_fall)
        for _ in range(PowerUp.EMERGE_FRAMES):
            p.update(level)
        for _ in range(200):
            p.update(level)
            if not p.alive:
                break
        self.assertFalse(p.alive)

    def test_reverses_on_wall_hit(self):
        rows = ["." * 20 for _ in range(15)]
        rows[14] = "G" * 20
        rows[12] = "." * 8 + "G" + "." * 11
        rows[13] = "." * 8 + "G" + "." * 11
        level = Level(make_level_data(rows, w=20, h=15))

        p = PowerUp(32 * 4, 32 * 12, _mushroom_cfg(), self.gravity, self.max_fall)
        for _ in range(PowerUp.EMERGE_FRAMES):
            p.update(level)
        initial_vx = p.vx
        reversed_dir = False
        for _ in range(500):
            p.update(level)
            if p.vx * initial_vx < 0:
                reversed_dir = True
                break
        self.assertTrue(reversed_dir, "Бонус должен разворачиваться от стены")


class TestPowerUpFactory(unittest.TestCase):
    """Фабрика бонусов из конфига."""

    def setUp(self):
        self.cfg = {
            "powerups": [_mushroom_cfg(), _star_cfg(), _flower_cfg()],
            "spawn_blocks": [
                {"x": 10, "y": 8, "powerup": "mushroom"},
                {"x": 25, "y": 7, "powerup": "star"},
                {"x": 40, "y": 8, "powerup": "fireflower"},
            ],
        }
        self.factory = PowerUpFactory(self.cfg, gravity=0.6,
                                      max_fall_speed=12.0, tile_size=32)

    def test_known_types(self):
        types = self.factory.known_types()
        self.assertIn("mushroom", types)
        self.assertIn("star", types)
        self.assertIn("fireflower", types)
        self.assertEqual(len(types), 3)

    def test_get_powerup_for_block_known(self):
        self.assertEqual(self.factory.get_powerup_for_block(10, 8), "mushroom")
        self.assertEqual(self.factory.get_powerup_for_block(25, 7), "star")
        self.assertEqual(self.factory.get_powerup_for_block(40, 8), "fireflower")

    def test_get_powerup_for_block_unknown_default(self):
        self.assertEqual(self.factory.get_powerup_for_block(999, 999), "mushroom")

    def test_create_at_block_uses_mapping(self):
        p = self.factory.create_at_block(25, 7)
        self.assertEqual(p.type_name, "star")
        self.assertEqual(p.score_value, 500)
        self.assertEqual(p.effect, "invincible")

    def test_create_at_block_explicit_type(self):
        p = self.factory.create_at_block(10, 8, powerup_type="fireflower")
        self.assertEqual(p.type_name, "fireflower")

    def test_create_at_block_position_centered(self):
        p = self.factory.create_at_block(10, 8)
        ts = 32
        cfg_w = _mushroom_cfg()["width"]
        expected_x = 10 * ts + (ts - cfg_w) / 2
        self.assertAlmostEqual(p.rect.x, expected_x)
        self.assertAlmostEqual(p.rect.y, 8 * ts)

    def test_factory_with_empty_spawn_blocks(self):
        cfg = {"powerups": [_mushroom_cfg()], "spawn_blocks": []}
        f = PowerUpFactory(cfg, 0.6, 12.0, 32)
        self.assertEqual(f.get_powerup_for_block(0, 0), "mushroom")


if __name__ == "__main__":
    unittest.main()
