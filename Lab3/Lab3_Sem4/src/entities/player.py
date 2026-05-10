"""Модуль игрока (Марио)."""
from .physics_entity import PhysicsEntity


class Player(PhysicsEntity):
    """Управляемый игрок."""

    STATE_SMALL = "small"
    STATE_BIG = "big"

    # Анимация
    ANIM_FRAME_TIME = 6  # кадров на спрайт-кадр

    def __init__(self, x: float, y: float, config: dict):
        w = config["player"]["width"]
        h = config["player"]["height"]
        super().__init__(x, y, w, h)
        self.config = config
        self.player_speed = config["physics"]["player_speed"]
        self.run_speed = config["physics"]["player_run_speed"]
        self.jump_strength = config["physics"]["jump_strength"]
        self.gravity = config["physics"]["gravity"]
        self.max_fall_speed = config["physics"]["max_fall_speed"]
        self.invincibility_max = config["player"]["invincibility_time"]
        self.lives = config["player"]["start_lives"]

        self.facing_right = True
        self.is_running = False
        self.want_jump = False

        # Состояния и таймеры
        self.state = self.STATE_SMALL
        self.invincible_timer = 0
        self.speed_boost_timer = 0
        self.star_timer = 0  # неуязвимость от звезды
        self.big_timer = 0   # сколько кадров остался "большим" по таймеру
        self.dying = False
        self.death_timer = 0

        # Анимация
        self.anim_timer = 0
        self.anim_frame = 0
        self._spawn_x = x
        self._spawn_y = y

        # «Прощающие» таймеры для прыжка (UX-улучшения)
        # jump_buffer: если игрок нажал прыжок за N кадров до приземления — прыжок состоится
        # coyote_time: после схода с платформы ещё N кадров можно прыгнуть
        self.JUMP_BUFFER_FRAMES = 6
        self.COYOTE_FRAMES = 6
        self.jump_buffer_timer = 0
        self.coyote_timer = 0

    # ----- Управление -----
    def move_left(self) -> None:
        self.vx = -self._current_speed()
        self.facing_right = False

    def move_right(self) -> None:
        self.vx = self._current_speed()
        self.facing_right = True

    def stop_horizontal(self) -> None:
        self.vx = 0

    def jump(self) -> bool:
        """Пытается прыгнуть. Возвращает True, если прыжок состоялся.

        Прыжок состоится, если игрок на земле ИЛИ только что сошёл с платформы
        (coyote time). Иначе нажатие записывается в буфер, и прыжок выполнится
        автоматически при ближайшем приземлении (jump buffer).
        """
        if self.dying:
            return False
        if self.on_ground or self.coyote_timer > 0:
            self.vy = self.jump_strength
            self.on_ground = False
            self.coyote_timer = 0
            self.jump_buffer_timer = 0
            return True
        # Запомнить нажатие — отработает при приземлении
        self.jump_buffer_timer = self.JUMP_BUFFER_FRAMES
        return False

    def set_running(self, running: bool) -> None:
        self.is_running = running

    def _current_speed(self) -> float:
        """Текущая скорость с учётом бега и буста."""
        base = self.run_speed if self.is_running else self.player_speed
        if self.speed_boost_timer > 0:
            base *= 1.5
        return base

    # ----- Бонусы -----
    def apply_powerup(self, effect: str, duration: int) -> None:
        """Применить эффект бонуса."""
        if effect == "grow":
            if self.state != self.STATE_BIG:
                old_bottom = self.rect.bottom
                self.rect.h = self.config["player"]["height"] + 12
                self.rect.y = old_bottom - self.rect.h
            self.state = self.STATE_BIG
            # Если duration > 0 — гигантизм по таймеру, иначе постоянно (как раньше)
            if duration > 0:
                self.big_timer = duration
        elif effect == "invincible":
            self.star_timer = duration
        elif effect == "speed_boost":
            self.speed_boost_timer = duration

    def take_damage(self) -> bool:
        """Получить урон. Возвращает True, если игрок умер."""
        if self.invincible_timer > 0 or self.star_timer > 0 or self.dying:
            return False
        if self.state == self.STATE_BIG:
            self.state = self.STATE_SMALL
            old_bottom = self.rect.bottom
            self.rect.h = self.config["player"]["height"]
            self.rect.y = old_bottom - self.rect.h
            self.invincible_timer = self.invincibility_max
            return False
        # Малый Марио — смерть
        self.start_death()
        return True

    def start_death(self) -> None:
        """Начать анимацию смерти."""
        self.dying = True
        self.death_timer = 90  # 1.5 секунды при 60 fps
        self.vy = -10
        self.vx = 0

    def is_invincible(self) -> bool:
        """Защищён ли игрок от урона."""
        return self.invincible_timer > 0 or self.star_timer > 0

    # ----- Обновление -----
    def update(self, level) -> dict:
        """Обновить состояние и физику игрока."""
        if self.dying:
            self.death_timer -= 1
            self.vy += self.gravity
            self.rect.y += self.vy
            if self.death_timer <= 0:
                self.alive = False
            return {"hit_top": False, "hit_bottom": False,
                    "hit_left": False, "hit_right": False,
                    "tiles_hit_top": [], "tiles_hit_bottom": [],
                    "tiles_hit_left": [], "tiles_hit_right": []}

        # Тики таймеров
        if self.invincible_timer > 0:
            self.invincible_timer -= 1
        if self.star_timer > 0:
            self.star_timer -= 1
        if self.speed_boost_timer > 0:
            self.speed_boost_timer -= 1
        if self.jump_buffer_timer > 0:
            self.jump_buffer_timer -= 1
        if self.coyote_timer > 0:
            self.coyote_timer -= 1
        # Гигантизм истекает по таймеру (если duration был задан)
        if self.big_timer > 0:
            self.big_timer -= 1
            if self.big_timer == 0 and self.state == self.STATE_BIG:
                # Возврат к маленькому без потери жизни
                self.state = self.STATE_SMALL
                old_bottom = self.rect.bottom
                self.rect.h = self.config["player"]["height"]
                self.rect.y = old_bottom - self.rect.h
                self.invincible_timer = self.invincibility_max

        # Запомнить, был ли игрок на земле до движения (для coyote time)
        was_on_ground = self.on_ground

        # Анимация (когда движется)
        if abs(self.vx) > 0.1:
            self.anim_timer += 1
            if self.anim_timer >= self.ANIM_FRAME_TIME:
                self.anim_timer = 0
                self.anim_frame = (self.anim_frame + 1) % 3
        else:
            self.anim_frame = 0

        self.apply_gravity(self.gravity, self.max_fall_speed)
        result = self.move_and_collide(level)

        # Coyote time: если только что слетели с платформы — даём окно на прыжок
        if was_on_ground and not self.on_ground and self.vy >= 0:
            self.coyote_timer = self.COYOTE_FRAMES

        # Jump buffer: если только что приземлились и в буфере было нажатие — прыгаем
        if self.on_ground and self.jump_buffer_timer > 0:
            self.vy = self.jump_strength
            self.on_ground = False
            self.jump_buffer_timer = 0

        # Падение в пропасть

        # Падение в пропасть
        if self.rect.top > level.pixel_height:
            self.start_death()

        # Не выходим за левую границу
        if self.rect.x < 0:
            self.rect.x = 0
        # Правую границу (не должны выйти за карту)
        if self.rect.right > level.pixel_width:
            self.rect.x = level.pixel_width - self.rect.w

        return result

    def respawn(self) -> None:
        """Сброс позиции игрока."""
        self.rect.x = self._spawn_x
        self.rect.y = self._spawn_y
        self.vx = 0
        self.vy = 0
        self.dying = False
        self.alive = True
        self.death_timer = 0
        self.invincible_timer = self.invincibility_max
        self.state = self.STATE_SMALL
        self.rect.h = self.config["player"]["height"]
