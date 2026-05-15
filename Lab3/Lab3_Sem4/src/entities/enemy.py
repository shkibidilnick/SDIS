"""Модуль врагов."""
from typing import Dict, Tuple

from .physics_entity import PhysicsEntity


class Enemy(PhysicsEntity):
    """Враг (Гумба, Купа, Пиранья). Поведение задаётся типом."""

    DEATH_FRAMES = 30  # длительность анимации смерти (0.5 сек)

    def __init__(self, x: float, y: float, enemy_type: Dict, gravity: float,
                 max_fall_speed: float):
        w = enemy_type["width"]
        h = enemy_type["height"]
        super().__init__(x, y, w, h)
        self.type_name: str = enemy_type["type"]
        self.display_name: str = enemy_type.get("name", self.type_name)
        self.speed: float = enemy_type["speed"]
        self.hp: int = enemy_type["hp"]
        self.color: Tuple[int, int, int] = tuple(enemy_type["color"])
        self.score_value: int = enemy_type["score_value"]
        self.behavior: str = enemy_type.get("behavior", "walk")
        self.can_be_stomped: bool = enemy_type.get("can_be_stomped", True)
        self.gravity = gravity
        self.max_fall_speed = max_fall_speed

        # Лёгкая случайная вариация скорости (±15%) — для разнообразия
        import random
        speed_variation = random.uniform(0.85, 1.15)
        self.speed = self.speed * speed_variation
        self.vx = -self.speed  # стартует, идя влево
        self.dying = False
        self.death_timer = 0

        # Анимация
        self.anim_timer = 0
        self.anim_frame = 0

        # Параметры пираньи (используются только для behavior == "piranha")
        if self.behavior == "piranha":
            self._piranha_pipe_top_y = y
            self._piranha_emerge_height = 32
            self._piranha_state = "hidden"
            self._piranha_timer = 90
            self._piranha_shown_frames = 120
            self._piranha_hidden_frames = 120
            self._piranha_anim_speed = 1.2
            # В скрытом состоянии пиранья сидит ВНУТРИ трубы
            self.rect.y = self._piranha_pipe_top_y + self._piranha_emerge_height
            
    def stomp(self) -> int:
        """Игрок наступил на врага. Возвращает количество очков."""
        if self.dying:
            return 0
        self.dying = True
        self.death_timer = self.DEATH_FRAMES
        self.vx = 0
        self.vy = -4
        return self.score_value

    def kill_by_star(self) -> int:
        """Убит звездой/неуязвимым Марио. Падает вниз с очками."""
        if self.dying:
            return 0
        self.dying = True
        self.death_timer = self.DEATH_FRAMES
        self.vy = -8
        return self.score_value

    def update(self, level) -> None:
        """Обновить врага."""
        # Пиранья — особое поведение, без гравитации и коллизий
        if self.behavior == "piranha":
            self._update_piranha()
            return

        if self.dying:
            self.death_timer -= 1
            self.vy += self.gravity
            if self.vy > self.max_fall_speed:
                self.vy = self.max_fall_speed
            self.rect.y += self.vy
            if self.death_timer <= 0:
                self.alive = False
            return

        # Анимация ходьбы
        self.anim_timer += 1
        if self.anim_timer >= 8:
            self.anim_timer = 0
            self.anim_frame = (self.anim_frame + 1) % 2

        self.apply_gravity(self.gravity, self.max_fall_speed)

        # Поведение перед движением ("walk" внесено временно, чтобы Гумба не падал с пропасти)
        if self.behavior in ("walk", "walk_smart"):
            self._check_ledge(level)

        # Запоминаем направление до коллизии — move_and_collide обнулит vx при ударе
        prev_direction = 1 if self.vx > 0 else (-1 if self.vx < 0 else 0)
        result = self.move_and_collide(level)

        # Развернуться при столкновении со стеной
        if result["hit_left"] or result["hit_right"]:
            if prev_direction != 0:
                self.vx = -prev_direction * self.speed
            else:
                self.vx = -self.speed

        # Если стоит и не движется (после сброса), пойти в текущем направлении
        if self.vx == 0 and not result["hit_left"] and not result["hit_right"]:
            self.vx = -self.speed

        # Падение в пропасть = смерть без очков
        if self.rect.top > level.pixel_height:
            self.alive = False

    def _check_ledge(self, level) -> None:
        """Разворачивается перед обрывом (для walk и walk_smart)."""
        ts = level.tile_size
        # точка под ногами впереди
        if self.vx > 0:
            check_x = self.rect.right + 1
        elif self.vx < 0:
            check_x = self.rect.left - 1
        else:
            return
        check_y = self.rect.bottom + 1
        tx = int(check_x // ts)
        ty = int(check_y // ts)
        if not level.is_solid(tx, ty) and self.on_ground:
            self.vx = -self.vx

    def _update_piranha(self) -> None:
        """Цикл: спрятана → выходит → видна → уходит → спрятана."""
        if self.dying:
            self.death_timer -= 1
            if self.death_timer <= 0:
                self.alive = False
            return

        top_y = self._piranha_pipe_top_y
        bottom_y = top_y + self._piranha_emerge_height

        if self._piranha_state == "hidden":
            self.rect.y = bottom_y
            self._piranha_timer -= 1
            if self._piranha_timer <= 0:
                self._piranha_state = "emerging"

        elif self._piranha_state == "emerging":
            self.rect.y -= self._piranha_anim_speed
            if self.rect.y <= top_y - self._piranha_emerge_height:
                self.rect.y = top_y - self._piranha_emerge_height
                self._piranha_state = "shown"
                self._piranha_timer = self._piranha_shown_frames

        elif self._piranha_state == "shown":
            self._piranha_timer -= 1
            if self._piranha_timer <= 0:
                self._piranha_state = "retracting"

        elif self._piranha_state == "retracting":
            self.rect.y += self._piranha_anim_speed
            if self.rect.y >= bottom_y:
                self.rect.y = bottom_y
                self._piranha_state = "hidden"
                self._piranha_timer = self._piranha_hidden_frames

        # Анимация рта (открыт/закрыт)
        self.anim_timer += 1
        if self.anim_timer >= 12:
            self.anim_timer = 0
            self.anim_frame = (self.anim_frame + 1) % 2

class EnemyFactory:
    """Создаёт врагов из конфига. Соблюдает SRP."""

    def __init__(self, enemies_config: Dict, gravity: float, max_fall_speed: float,
                 tile_size: int):
        self.enemies_config = enemies_config
        self.gravity = gravity
        self.max_fall_speed = max_fall_speed
        self.tile_size = tile_size
        self._types: Dict[str, Dict] = {
            e["type"]: e for e in enemies_config.get("enemies", [])
        }

    def create_from_spawn(self, spawn: Dict) -> Enemy:
        """Создать врага по записи спавна (x/y в тайлах)."""
        enemy_type = self._types[spawn["type"]]
        x = spawn["x"] * self.tile_size + (self.tile_size
                                           - enemy_type["width"]) // 2
        if enemy_type.get("behavior") == "piranha":
            # Пиранья: y = верх трубы (тайл с верхом трубы)
            y = spawn["y"] * self.tile_size
        else:
            y = spawn["y"] * self.tile_size + (self.tile_size
                                               - enemy_type["height"])
        return Enemy(x, y, enemy_type, self.gravity, self.max_fall_speed)

    def create_all(self) -> list:
        """Создать всех врагов из секции 'spawns'."""
        return [self.create_from_spawn(s)
                for s in self.enemies_config.get("spawns", [])]

    def get_type(self, name: str) -> Dict:
        """Получить описание типа врага."""
        return self._types[name]
