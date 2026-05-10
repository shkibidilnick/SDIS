"""Модуль подбираемых бонусов (powerup'ов).

Реализует не менее 2 типов (ТЗ: усложнённое требование). У нас 3 типа:
- mushroom: рост (доп. жизнь / возможность выдержать удар)
- star: неуязвимость на 6 секунд
- fireflower: ускорение Марио на 8 секунд
"""
from typing import Dict, List, Tuple

from .physics_entity import PhysicsEntity


class PowerUp(PhysicsEntity):
    """Подбираемый бонус. Когда выходит из блока — двигается, иначе стоит."""

    EMERGE_FRAMES = 30  # анимация появления из блока

    def __init__(self, x: float, y: float, config: Dict, gravity: float,
                 max_fall_speed: float):
        w = config["width"]
        h = config["height"]
        super().__init__(x, y, w, h)
        self.type_name: str = config["type"]
        self.display_name: str = config.get("name", self.type_name)
        self.color: Tuple[int, int, int] = tuple(config["color"])
        self.secondary_color: Tuple[int, int, int] = tuple(
            config.get("secondary_color", config["color"])
        )
        self.speed: float = config.get("speed", 1.0)
        self.score_value: int = config["score_value"]
        self.effect: str = config["effect"]
        self.duration: int = config.get("duration", 0)
        self.gravity = gravity
        self.max_fall_speed = max_fall_speed

        self.emerging = True
        self.emerge_timer = self.EMERGE_FRAMES
        self.emerge_start_y = y
        self.emerge_target_y = y - h  # выезжает на одну клетку вверх

        if self.speed > 0:
            self.vx = self.speed  # после появления едет вправо
        # звезда прыгает
        self._is_star = (self.effect == "invincible")

    def update(self, level) -> None:
        """Обновить бонус."""
        if self.emerging:
            # плавный подъём
            progress = 1 - self.emerge_timer / self.EMERGE_FRAMES
            self.rect.y = self.emerge_start_y + (self.emerge_target_y
                                                 - self.emerge_start_y) * progress
            self.emerge_timer -= 1
            if self.emerge_timer <= 0:
                self.emerging = False
                self.rect.y = self.emerge_target_y
            return

        # Звезда подпрыгивает
        if self._is_star and self.on_ground:
            self.vy = -8

        self.apply_gravity(self.gravity, self.max_fall_speed)
        # Сохраняем направление движения до столкновения, т.к. move_and_collide
        # обнуляет vx при ударе в стену.
        prev_direction = 1 if self.vx > 0 else (-1 if self.vx < 0 else 0)
        result = self.move_and_collide(level)
        if (result["hit_left"] or result["hit_right"]) and prev_direction != 0:
            self.vx = -prev_direction * self.speed

        # вылетел за карту
        if self.rect.top > level.pixel_height:
            self.alive = False


class PowerUpFactory:
    """Создаёт бонусы из конфига."""

    def __init__(self, powerups_config: Dict, gravity: float,
                 max_fall_speed: float, tile_size: int):
        self.powerups_config = powerups_config
        self.gravity = gravity
        self.max_fall_speed = max_fall_speed
        self.tile_size = tile_size
        self._types: Dict[str, Dict] = {
            p["type"]: p for p in powerups_config.get("powerups", [])
        }
        # {(tx, ty): "mushroom"}
        self._block_to_powerup: Dict[Tuple[int, int], str] = {
            (b["x"], b["y"]): b["powerup"]
            for b in powerups_config.get("spawn_blocks", [])
        }

    def get_powerup_for_block(self, tx: int, ty: int) -> str:
        """Какой powerup в этом блоке (по умолчанию 'mushroom')."""
        return self._block_to_powerup.get((tx, ty), "mushroom")

    def create_at_block(self, tx: int, ty: int, powerup_type: str = None) -> PowerUp:
        """Создать бонус из блока ?."""
        if powerup_type is None:
            powerup_type = self.get_powerup_for_block(tx, ty)
        cfg = self._types[powerup_type]
        x = tx * self.tile_size + (self.tile_size - cfg["width"]) / 2
        y = ty * self.tile_size  # ниже блока, чтобы потом подняться
        return PowerUp(x, y, cfg, self.gravity, self.max_fall_speed)

    def known_types(self) -> List[str]:
        return list(self._types.keys())
