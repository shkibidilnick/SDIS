"""Тесты для Level."""
import unittest

from src.entities.level import (Level, TILE_BRICK, TILE_COIN, TILE_EMPTY,
                                TILE_GROUND, TILE_QUESTION)
from src.utils.geometry import Rect


def make_level_data(tiles, w=None, h=None):
    if w is None:
        w = max(len(r) for r in tiles)
    # дополним строки до общей ширины
    tiles = [r.ljust(w, ".") for r in tiles]
    if h is None:
        h = len(tiles)
    return {
        "name": "test",
        "width_tiles": w,
        "height_tiles": h,
        "background_color": [0, 0, 0],
        "spawn": {"x": 0, "y": 0},
        "goal": {"x": w - 1, "y": h - 1},
        "tiles": tiles,
    }


class TestLevel(unittest.TestCase):
    def test_basic_dimensions(self):
        data = make_level_data(["...", "GGG"])
        lvl = Level(data, tile_size=32)
        self.assertEqual(lvl.width_tiles, 3)
        self.assertEqual(lvl.height_tiles, 2)
        self.assertEqual(lvl.pixel_width, 96)
        self.assertEqual(lvl.pixel_height, 64)

    def test_invalid_height_raises(self):
        data = make_level_data(["...", "GGG"])
        data["height_tiles"] = 5  # неверно
        with self.assertRaises(ValueError):
            Level(data)

    def test_invalid_row_length_raises(self):
        data = {
            "name": "x", "width_tiles": 5, "height_tiles": 1,
            "background_color": [0, 0, 0],
            "spawn": {"x": 0, "y": 0}, "goal": {"x": 0, "y": 0},
            "tiles": ["GGG"],  # длина 3, а не 5
        }
        with self.assertRaises(ValueError):
            Level(data)

    def test_get_tile(self):
        data = make_level_data(["GBQ"])
        lvl = Level(data)
        self.assertEqual(lvl.get_tile(0, 0), TILE_GROUND)
        self.assertEqual(lvl.get_tile(1, 0), TILE_BRICK)
        self.assertEqual(lvl.get_tile(2, 0), TILE_QUESTION)
        # out of range
        self.assertEqual(lvl.get_tile(-1, 0), TILE_EMPTY)
        self.assertEqual(lvl.get_tile(99, 99), TILE_EMPTY)

    def test_set_tile(self):
        data = make_level_data(["GG"])
        lvl = Level(data)
        lvl.set_tile(0, 0, TILE_EMPTY)
        self.assertEqual(lvl.get_tile(0, 0), TILE_EMPTY)
        self.assertEqual(lvl.get_tile(1, 0), TILE_GROUND)

    def test_set_tile_out_of_bounds_no_error(self):
        lvl = Level(make_level_data(["G"]))
        # Не должно ни сломаться, ни изменить
        lvl.set_tile(99, 99, TILE_EMPTY)
        self.assertEqual(lvl.get_tile(0, 0), TILE_GROUND)

    def test_is_solid(self):
        lvl = Level(make_level_data(["GBC."]))
        self.assertTrue(lvl.is_solid(0, 0))   # G
        self.assertTrue(lvl.is_solid(1, 0))   # B
        self.assertFalse(lvl.is_solid(2, 0))  # C — interactive, не solid
        self.assertFalse(lvl.is_solid(3, 0))  # .

    def test_solid_tiles_in_rect(self):
        # 3x3 уровень: одна твёрдая в центре
        data = make_level_data(["...", ".B.", "..."])
        lvl = Level(data, tile_size=10)
        # рамка 5..15 по обеим осям — пересекает только центральный тайл (10..20)
        rect = Rect(5, 5, 10, 10)
        tiles = lvl.solid_tiles_in_rect(rect)
        self.assertEqual(tiles, [(1, 1)])

    def test_solid_tiles_in_rect_outside(self):
        lvl = Level(make_level_data([".."]), tile_size=10)
        rect = Rect(5, 5, 5, 5)
        self.assertEqual(lvl.solid_tiles_in_rect(rect), [])

    def test_interactive_tiles_in_rect(self):
        lvl = Level(make_level_data(["CFG"]), tile_size=10)
        rect = Rect(0, 0, 30, 10)
        result = lvl.interactive_tiles_in_rect(rect)
        types = [t[2] for t in result]
        self.assertIn(TILE_COIN, types)
        # G не интерактивный
        self.assertNotIn(TILE_GROUND, types)

    def test_find_first_tile(self):
        lvl = Level(make_level_data(["...", "..F"]))
        self.assertEqual(lvl.find_first_tile("F"), (2, 1))
        self.assertIsNone(lvl.find_first_tile("Z"))

    def test_tile_rect(self):
        lvl = Level(make_level_data(["G"]), tile_size=32)
        r = lvl.tile_rect(0, 0)
        self.assertEqual((r.x, r.y, r.w, r.h), (0, 0, 32, 32))
        r = lvl.tile_rect(2, 3)
        self.assertEqual((r.x, r.y), (64, 96))


if __name__ == "__main__":
    unittest.main()
