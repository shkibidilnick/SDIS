"""
TileMap — parses a level config and provides collision rects + rendering.
Single Responsibility: map data, tile geometry, and world drawing only.
"""
from __future__ import annotations

from dataclasses import dataclass, field
from typing import List, Dict, Optional, Tuple, TYPE_CHECKING
import pygame

if TYPE_CHECKING:
    from src.utils.asset_manager import AssetManager
    from src.utils.camera import Camera

# ---------------------------------------------------------------------------
# Tile type registry
# ---------------------------------------------------------------------------
TILE_SOLID = {"ground", "brick", "stone", "pipe_top_left", "pipe_top_right",
              "pipe_left", "pipe_right", "platform", "question_used"}
TILE_QUESTION = {"question", "coin_block"}
TILE_PIPE = {"pipe_top_left", "pipe_top_right", "pipe_left", "pipe_right"}

# Colors for placeholder rendering
TILE_COLORS: Dict[str, Tuple[int, int, int]] = {
    "ground":        (139, 90, 43),
    "brick":         (180, 100, 40),
    "question":      (255, 200, 0),
    "coin_block":    (255, 220, 50),
    "question_used": (120, 100, 80),
    "stone":         (100, 100, 100),
    "pipe_top_left": (0, 160, 0),
    "pipe_top_right":(0, 160, 0),
    "pipe_left":     (0, 130, 0),
    "pipe_right":    (0, 130, 0),
    "platform":      (180, 130, 80),
    "flag":          (200, 200, 200),
}


@dataclass
class Tile:
    """A single map tile."""
    rect: pygame.Rect
    tile_type: str
    solid: bool
    question: bool = False
    used: bool = False          # question block already triggered
    hidden_item: Optional[str] = None  # "mushroom" | "coin" | etc.


@dataclass
class TileMap:
    """Parsed level data — tiles, background color, metadata."""

    tiles: List[Tile] = field(default_factory=list)
    solid_rects: List[pygame.Rect] = field(default_factory=list)
    width_px: int = 0
    height_px: int = 0
    background_color: Tuple[int, int, int] = (92, 148, 252)
    tile_size: int = 32
    flagpole_x: int = 0
    flagpole_rect: Optional[pygame.Rect] = None
    music: str = "overworld"
    time_limit: int = 400

    # ------------------------------------------------------------------ #
    # Factory                                                              #
    # ------------------------------------------------------------------ #

    @classmethod
    def from_config(cls, cfg: dict, tile_size: int = 32) -> "TileMap":
        tm = cls()
        tm.tile_size = tile_size
        tm.background_color = tuple(cfg.get("background_color", [92, 148, 252]))
        tm.music = cfg.get("music", "overworld")
        tm.time_limit = cfg.get("time_limit", 400)
        tm.flagpole_x = cfg.get("flagpole_x", 240) * tile_size

        char_map: Dict[str, str] = cfg.get("tiles", {})
        grid: List[str] = cfg.get("map", [])
        map_w = max(len(row) for row in grid) if grid else 0
        map_h = len(grid)

        tm.width_px  = map_w * tile_size
        tm.height_px = map_h * tile_size

        for row_idx, row in enumerate(grid):
            for col_idx, char in enumerate(row):
                if char == " ":
                    continue
                tile_type = char_map.get(char, "ground")
                rect = pygame.Rect(col_idx * tile_size, row_idx * tile_size,
                                   tile_size, tile_size)
                solid = tile_type in TILE_SOLID or tile_type in TILE_QUESTION
                t = Tile(rect=rect, tile_type=tile_type, solid=solid,
                         question=(tile_type in TILE_QUESTION))
                tm.tiles.append(t)
                if solid:
                    tm.solid_rects.append(rect)

        # Flagpole rect
        fp_x = tm.flagpole_x
        fp_rect = pygame.Rect(fp_x, 0, tile_size // 2,
                              map_h * tile_size)
        tm.flagpole_rect = fp_rect

        return tm

    # ------------------------------------------------------------------ #
    # Rendering                                                            #
    # ------------------------------------------------------------------ #

    def draw(self, surface: pygame.Surface, camera: "Camera") -> None:
        screen_w = surface.get_width()
        for tile in self.tiles:
            sr = camera.apply(tile.rect)
            if sr.right < 0 or sr.left > screen_w:
                continue  # cull off-screen tiles

            color = TILE_COLORS.get(
                "question_used" if tile.used and tile.question else tile.tile_type,
                (80, 80, 80)
            )
            pygame.draw.rect(surface, color, sr)

            # Tile detail lines
            if tile.tile_type == "ground":
                pygame.draw.rect(surface, (100, 60, 20), sr, 1)
            elif tile.tile_type in ("brick",):
                pygame.draw.rect(surface, (140, 70, 20), sr, 1)
                mid_y = sr.y + sr.height // 2
                pygame.draw.line(surface, (140, 70, 20),
                                 (sr.x, mid_y), (sr.right, mid_y), 1)
            elif tile.tile_type == "question" and not tile.used:
                pygame.draw.rect(surface, (200, 150, 0), sr, 2)
                font = pygame.font.SysFont("courier", self.tile_size - 4, bold=True)
                lbl = font.render("?", True, (255, 255, 255))
                surface.blit(lbl, lbl.get_rect(center=sr.center))
            elif tile.tile_type in TILE_PIPE:
                pygame.draw.rect(surface, (0, 100, 0), sr, 2)

        # Draw flagpole
        if self.flagpole_rect:
            sr = camera.apply(self.flagpole_rect)
            pole_rect = pygame.Rect(sr.x + sr.width // 2 - 2,
                                    sr.top, 4, sr.height)
            pygame.draw.rect(surface, (200, 200, 200), pole_rect)
            flag_rect = pygame.Rect(sr.x + sr.width // 2 + 2,
                                    sr.top + 20, 20, 14)
            pygame.draw.rect(surface, (220, 30, 30), flag_rect)

    # ------------------------------------------------------------------ #
    # Block interaction                                                    #
    # ------------------------------------------------------------------ #

    def hit_block_at(self, hit_rect: pygame.Rect) -> Optional[Tile]:
        """
        Called when Mario bumps a block from below.
        Returns the tile if it is a question/brick block that can be triggered.
        """
        for tile in self.tiles:
            if not tile.question or tile.used:
                continue
            if tile.rect.colliderect(hit_rect):
                tile.used = True
                # Update solid_rects to use the 'used' version
                # (it remains solid, just visually different)
                return tile
        # Check bricks (breakable when big)
        for tile in self.tiles:
            if tile.tile_type == "brick" and not tile.used:
                if tile.rect.colliderect(hit_rect):
                    return tile
        return None

    def remove_tile(self, tile: Tile) -> None:
        """Remove a tile entirely (brick break)."""
        if tile in self.tiles:
            self.tiles.remove(tile)
        if tile.rect in self.solid_rects:
            self.solid_rects.remove(tile.rect)
