"""Tests for TileMap."""
import unittest
import pygame

pygame.init()
pygame.display.set_mode((1, 1), pygame.NOFRAME)

from src.tilemap import TileMap, Tile, TILE_SOLID


def _minimal_cfg(map_rows=None, tiles=None, flagpole_x=10):
    return {
        "background_color": [92, 148, 252],
        "music": "overworld",
        "time_limit": 300,
        "flagpole_x": flagpole_x,
        "tiles": tiles or {"G": "ground", "B": "brick", "Q": "question"},
        "map": map_rows or ["GGGG", "    "],
    }


class TestTileMapFromConfig(unittest.TestCase):

    def test_creates_tilemap(self):
        tm = TileMap.from_config(_minimal_cfg())
        self.assertIsInstance(tm, TileMap)

    def test_background_color(self):
        tm = TileMap.from_config(_minimal_cfg())
        self.assertEqual(tm.background_color, (92, 148, 252))

    def test_tiles_created(self):
        tm = TileMap.from_config(_minimal_cfg(["GGGG"]))
        self.assertEqual(len(tm.tiles), 4)

    def test_spaces_ignored(self):
        tm = TileMap.from_config(_minimal_cfg(["G G"]))
        # Only 2 tiles (indices 0 and 2)
        self.assertEqual(len(tm.tiles), 2)

    def test_solid_rects_match_ground(self):
        tm = TileMap.from_config(_minimal_cfg(["GGGG"]))
        self.assertEqual(len(tm.solid_rects), 4)

    def test_width_px(self):
        tm = TileMap.from_config(_minimal_cfg(["GGGG"]), tile_size=32)
        self.assertEqual(tm.width_px, 4 * 32)

    def test_height_px(self):
        tm = TileMap.from_config(_minimal_cfg(["GG", "GG"]), tile_size=32)
        self.assertEqual(tm.height_px, 2 * 32)

    def test_question_block_flagged(self):
        tm = TileMap.from_config(_minimal_cfg(["Q"]))
        self.assertTrue(tm.tiles[0].question)

    def test_ground_not_question(self):
        tm = TileMap.from_config(_minimal_cfg(["G"]))
        self.assertFalse(tm.tiles[0].question)

    def test_flagpole_rect_created(self):
        tm = TileMap.from_config(_minimal_cfg(flagpole_x=5))
        self.assertIsNotNone(tm.flagpole_rect)

    def test_music_stored(self):
        tm = TileMap.from_config(_minimal_cfg())
        self.assertEqual(tm.music, "overworld")

    def test_time_limit_stored(self):
        tm = TileMap.from_config(_minimal_cfg())
        self.assertEqual(tm.time_limit, 300)


class TestTileMapHitBlock(unittest.TestCase):

    def _tm_with_question_at(self, col, row, ts=32):
        cfg = {
            "background_color": [0, 0, 0],
            "music": "x", "time_limit": 100,
            "flagpole_x": 50,
            "tiles": {"Q": "question", " ": "air"},
            "map": [" " * col + "Q"],
        }
        return TileMap.from_config(cfg, tile_size=ts)

    def test_hit_question_block_marks_used(self):
        ts = 32
        tm = self._tm_with_question_at(0, 0, ts)
        hit_rect = pygame.Rect(2, ts, 20, 4)  # just below the block
        tile = tm.hit_block_at(hit_rect)
        self.assertIsNotNone(tile)
        self.assertTrue(tile.used)

    def test_hit_used_block_returns_none(self):
        ts = 32
        tm = self._tm_with_question_at(0, 0, ts)
        hit_rect = pygame.Rect(2, ts, 20, 4)
        tm.hit_block_at(hit_rect)     # first hit → used
        tile2 = tm.hit_block_at(hit_rect)  # second hit → None
        self.assertIsNone(tile2)

    def test_hit_empty_space_returns_none(self):
        tm = TileMap.from_config(_minimal_cfg(["GGG"]))
        hit_rect = pygame.Rect(500, 500, 20, 4)
        self.assertIsNone(tm.hit_block_at(hit_rect))


class TestTileMapRemoveTile(unittest.TestCase):

    def test_remove_tile_deletes_it(self):
        tm = TileMap.from_config(_minimal_cfg(["B"]))
        tile = tm.tiles[0]
        tm.remove_tile(tile)
        self.assertNotIn(tile, tm.tiles)

    def test_remove_tile_removes_solid_rect(self):
        tm = TileMap.from_config(_minimal_cfg(["B"]))
        tile = tm.tiles[0]
        rect = tile.rect
        tm.remove_tile(tile)
        self.assertNotIn(rect, tm.solid_rects)


if __name__ == "__main__":
    unittest.main()
