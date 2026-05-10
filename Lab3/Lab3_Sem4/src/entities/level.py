"""Модуль карты уровня. Преобразует символы из level_map.json в тайлы.

Соблюдает SRP: только хранение и предоставление информации о карте.
"""
from typing import Dict, List, Optional, Tuple

from ..utils.geometry import Rect


# Типы тайлов
TILE_EMPTY = "."
TILE_GROUND = "G"
TILE_BRICK = "B"
TILE_QUESTION = "Q"
TILE_PIPE = "P"
TILE_COIN = "C"
TILE_FLAG = "F"
TILE_STAIR = "S"

# Какие тайлы являются твёрдыми (по ним нельзя проходить)
SOLID_TILES = {TILE_GROUND, TILE_BRICK, TILE_QUESTION, TILE_PIPE, TILE_STAIR}
# Тайлы, с которыми взаимодействует игрок (не твёрдые в смысле коллизии)
INTERACTIVE_TILES = {TILE_COIN, TILE_FLAG}


class Level:
    """Данные одного уровня."""

    def __init__(self, data: Dict, tile_size: int = 32):
        self.tile_size = tile_size
        self.name: str = data.get("name", "Untitled")
        self.width_tiles: int = int(data["width_tiles"])
        self.height_tiles: int = int(data["height_tiles"])
        bg = data.get("background_color", [0, 0, 0])
        self.background_color: Tuple[int, int, int] = (
            int(bg[0]), int(bg[1]), int(bg[2])
        )
        self.spawn: Tuple[int, int] = (
            int(data["spawn"]["x"]), int(data["spawn"]["y"])
        )
        self.goal: Tuple[int, int] = (
            int(data["goal"]["x"]), int(data["goal"]["y"])
        )
        # tiles — список строк, каждая строка длиной width_tiles
        self.tiles: List[str] = list(data["tiles"])
        # Контроль целостности
        if len(self.tiles) != self.height_tiles:
            raise ValueError(
                f"Высота карты не совпадает: {len(self.tiles)} vs {self.height_tiles}"
            )
        for i, row in enumerate(self.tiles):
            if len(row) != self.width_tiles:
                raise ValueError(
                    f"Строка {i} имеет длину {len(row)} вместо {self.width_tiles}"
                )

    @property
    def pixel_width(self) -> int:
        return self.width_tiles * self.tile_size

    @property
    def pixel_height(self) -> int:
        return self.height_tiles * self.tile_size

    def get_tile(self, tx: int, ty: int) -> str:
        """Тайл по индексам. Вне карты возвращает '.'"""
        if 0 <= ty < self.height_tiles and 0 <= tx < self.width_tiles:
            return self.tiles[ty][tx]
        return TILE_EMPTY

    def set_tile(self, tx: int, ty: int, value: str) -> None:
        """Заменить тайл (например, удалить монетку или сломать кирпич)."""
        if 0 <= ty < self.height_tiles and 0 <= tx < self.width_tiles:
            row = self.tiles[ty]
            self.tiles[ty] = row[:tx] + value + row[tx + 1:]

    def is_solid(self, tx: int, ty: int) -> bool:
        """Является ли тайл твёрдым."""
        return self.get_tile(tx, ty) in SOLID_TILES

    def tile_rect(self, tx: int, ty: int) -> Rect:
        """Прямоугольник для тайла в пиксельных координатах."""
        return Rect(tx * self.tile_size, ty * self.tile_size,
                    self.tile_size, self.tile_size)

    def solid_tiles_in_rect(self, rect: Rect) -> List[Tuple[int, int]]:
        """Список координат твёрдых тайлов, пересекающихся с прямоугольником."""
        ts = self.tile_size
        x1 = max(0, int(rect.left // ts))
        x2 = min(self.width_tiles - 1, int((rect.right - 1) // ts))
        y1 = max(0, int(rect.top // ts))
        y2 = min(self.height_tiles - 1, int((rect.bottom - 1) // ts))
        result = []
        for ty in range(y1, y2 + 1):
            for tx in range(x1, x2 + 1):
                if self.is_solid(tx, ty):
                    result.append((tx, ty))
        return result

    def interactive_tiles_in_rect(self, rect: Rect) -> List[Tuple[int, int, str]]:
        """Список интерактивных тайлов, пересекающихся с прямоугольником."""
        ts = self.tile_size
        x1 = max(0, int(rect.left // ts))
        x2 = min(self.width_tiles - 1, int((rect.right - 1) // ts))
        y1 = max(0, int(rect.top // ts))
        y2 = min(self.height_tiles - 1, int((rect.bottom - 1) // ts))
        result = []
        for ty in range(y1, y2 + 1):
            for tx in range(x1, x2 + 1):
                t = self.get_tile(tx, ty)
                if t in INTERACTIVE_TILES:
                    result.append((tx, ty, t))
        return result

    def find_first_tile(self, value: str) -> Optional[Tuple[int, int]]:
        """Найти координаты первого тайла с заданным значением."""
        for ty, row in enumerate(self.tiles):
            for tx, ch in enumerate(row):
                if ch == value:
                    return (tx, ty)
        return None
