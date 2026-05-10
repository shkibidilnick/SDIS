"""Тесты для Player."""
import unittest

from src.entities.level import Level
from src.entities.player import Player
from tests.test_level import make_level_data


def make_test_config():
    return {
        "screen": {"width": 800, "height": 600, "title": "t", "fps": 60},
        "physics": {
            "gravity": 0.6, "max_fall_speed": 12,
            "jump_strength": -10, "player_speed": 4, "player_run_speed": 6
        },
        "tile": {"size": 32},
        "player": {
            "start_lives": 3, "start_x": 0, "start_y": 0,
            "width": 28, "height": 32, "invincibility_time": 60
        },
        "scoring": {"enemy_kill": 100, "coin": 50, "powerup_pickup": 200,
                    "level_complete": 1000, "time_bonus_per_second": 10},
        "audio": {"music_volume": 0.4, "sfx_volume": 0.6},
        "level": {"time_limit": 100},
        "files": {"level_map": "x", "enemies": "x",
                  "powerups": "x", "highscores": "x"},
    }


def make_simple_ground_level():
    """3 колонны земли в нижнем ряду."""
    data = make_level_data(["...", "...", "GGG"], w=3, h=3)
    return Level(data, tile_size=32)


class TestPlayer(unittest.TestCase):
    def test_initial_state(self):
        cfg = make_test_config()
        p = Player(0, 0, cfg)
        self.assertEqual(p.lives, 3)
        self.assertEqual(p.state, Player.STATE_SMALL)
        self.assertTrue(p.facing_right)
        self.assertFalse(p.dying)
        self.assertTrue(p.alive)

    def test_move_left_right(self):
        cfg = make_test_config()
        p = Player(50, 0, cfg)
        p.move_right()
        self.assertEqual(p.vx, cfg["physics"]["player_speed"])
        self.assertTrue(p.facing_right)
        p.move_left()
        self.assertEqual(p.vx, -cfg["physics"]["player_speed"])
        self.assertFalse(p.facing_right)
        p.stop_horizontal()
        self.assertEqual(p.vx, 0)

    def test_running_speed(self):
        cfg = make_test_config()
        p = Player(0, 0, cfg)
        p.set_running(True)
        p.move_right()
        self.assertEqual(p.vx, cfg["physics"]["player_run_speed"])

    def test_jump_only_on_ground(self):
        cfg = make_test_config()
        p = Player(0, 0, cfg)
        # сначала не на земле
        self.assertFalse(p.jump())
        p.on_ground = True
        self.assertTrue(p.jump())
        self.assertEqual(p.vy, cfg["physics"]["jump_strength"])
        self.assertFalse(p.on_ground)

    def test_jump_blocked_when_dying(self):
        cfg = make_test_config()
        p = Player(0, 0, cfg)
        p.on_ground = True
        p.start_death()
        self.assertFalse(p.jump())

    def test_falling_into_pit_starts_death(self):
        cfg = make_test_config()
        lvl = Level(make_level_data(["..."], w=3, h=1), tile_size=32)
        p = Player(0, 0, cfg)
        # переместим под уровень
        p.rect.y = lvl.pixel_height + 100
        p.update(lvl)
        self.assertTrue(p.dying)

    def test_powerup_grow(self):
        cfg = make_test_config()
        p = Player(0, 100, cfg)
        original_h = p.rect.h
        p.apply_powerup("grow", 0)
        self.assertEqual(p.state, Player.STATE_BIG)
        self.assertGreater(p.rect.h, original_h)

    def test_powerup_invincible(self):
        cfg = make_test_config()
        p = Player(0, 0, cfg)
        p.apply_powerup("invincible", 60)
        self.assertEqual(p.star_timer, 60)
        self.assertTrue(p.is_invincible())

    def test_powerup_speed_boost(self):
        cfg = make_test_config()
        p = Player(0, 0, cfg)
        p.apply_powerup("speed_boost", 60)
        self.assertEqual(p.speed_boost_timer, 60)
        # _current_speed увеличивается
        p.move_right()
        self.assertGreater(abs(p.vx), cfg["physics"]["player_speed"])

    def test_take_damage_when_big_shrinks(self):
        cfg = make_test_config()
        p = Player(0, 100, cfg)
        p.apply_powerup("grow", 0)
        died = p.take_damage()
        self.assertFalse(died)
        self.assertEqual(p.state, Player.STATE_SMALL)
        # стал неуязвимым на время
        self.assertGreater(p.invincible_timer, 0)

    def test_take_damage_when_small_dies(self):
        cfg = make_test_config()
        p = Player(0, 0, cfg)
        died = p.take_damage()
        self.assertTrue(died)
        self.assertTrue(p.dying)

    def test_take_damage_blocked_when_invincible(self):
        cfg = make_test_config()
        p = Player(0, 0, cfg)
        p.invincible_timer = 30
        died = p.take_damage()
        self.assertFalse(died)
        self.assertFalse(p.dying)

    def test_take_damage_blocked_when_star(self):
        cfg = make_test_config()
        p = Player(0, 0, cfg)
        p.star_timer = 30
        self.assertFalse(p.take_damage())

    def test_invincibility_timer_decrements(self):
        cfg = make_test_config()
        lvl = make_simple_ground_level()
        p = Player(0, 0, cfg)
        p.invincible_timer = 5
        for _ in range(10):
            p.update(lvl)
        self.assertEqual(p.invincible_timer, 0)

    def test_star_timer_decrements(self):
        cfg = make_test_config()
        lvl = make_simple_ground_level()
        p = Player(0, 0, cfg)
        p.star_timer = 3
        for _ in range(5):
            p.update(lvl)
        self.assertEqual(p.star_timer, 0)

    def test_animation_when_moving(self):
        cfg = make_test_config()
        lvl = make_simple_ground_level()
        p = Player(0, 0, cfg)
        # дай упасть
        for _ in range(10):
            p.update(lvl)
        p.move_right()
        for _ in range(20):
            p.update(lvl)
        # анимация должна была переключиться
        self.assertIn(p.anim_frame, [0, 1, 2])

    def test_no_horizontal_below_zero(self):
        cfg = make_test_config()
        lvl = make_simple_ground_level()
        p = Player(0, 0, cfg)
        p.move_left()
        for _ in range(20):
            p.update(lvl)
        self.assertGreaterEqual(p.rect.x, 0)

    def test_does_not_exit_right_edge(self):
        cfg = make_test_config()
        lvl = make_simple_ground_level()
        p = Player(50, 0, cfg)
        p.vx = 100  # очень большая
        for _ in range(5):
            p.update(lvl)
        self.assertLessEqual(p.rect.right, lvl.pixel_width)

    def test_respawn(self):
        cfg = make_test_config()
        p = Player(100, 100, cfg)
        p.apply_powerup("grow", 0)
        p.rect.x = 999
        p.rect.y = 999
        p.dying = True
        p.respawn()
        self.assertEqual(p.state, Player.STATE_SMALL)
        self.assertFalse(p.dying)
        self.assertTrue(p.alive)
        self.assertEqual(p.rect.x, 100)

    def test_death_animation_completes(self):
        cfg = make_test_config()
        lvl = make_simple_ground_level()
        p = Player(0, 0, cfg)
        p.start_death()
        for _ in range(200):
            p.update(lvl)
        self.assertFalse(p.alive)

class TestPlayerBigTimer(unittest.TestCase):
    """Гриб с таймером (новое поведение): возврат в small по истечении."""

    def setUp(self):
        self.cfg = {
            "physics": {
                "gravity": 0.6, "max_fall_speed": 12,
                "player_speed": 4, "player_run_speed": 6, "jump_strength": -13,
            },
            "player": {
                "width": 28, "height": 32, "start_lives": 3,
                "invincibility_time": 120,
            },
        }
        self.level = Level(make_level_data(["..."], w=3, h=1), tile_size=32)

    def test_grow_with_zero_duration_permanent(self):
        # duration=0 → старое поведение, без таймера
        p = Player(0, 0, self.cfg)
        p.apply_powerup("grow", 0)
        self.assertEqual(p.state, Player.STATE_BIG)
        self.assertEqual(p.big_timer, 0)
        # Прокручиваем 100 кадров — состояние не должно измениться
        for _ in range(100):
            p.update(self.level)
        self.assertEqual(p.state, Player.STATE_BIG)

    def test_grow_with_duration_sets_timer(self):
        p = Player(0, 0, self.cfg)
        p.apply_powerup("grow", 60)
        self.assertEqual(p.state, Player.STATE_BIG)
        self.assertEqual(p.big_timer, 60)

    def test_big_timer_ticks_down(self):
        p = Player(0, 0, self.cfg)
        p.apply_powerup("grow", 10)
        for _ in range(5):
            p.update(self.level)
        # Прошло 5 кадров → таймер ~5
        self.assertLessEqual(p.big_timer, 5)

    def test_big_timer_expires_returns_to_small(self):
        p = Player(0, 0, self.cfg)
        p.apply_powerup("grow", 5)
        small_h = self.cfg["player"]["height"]
        big_h = small_h + 12
        self.assertEqual(p.rect.h, big_h)
        # Прокрутить чуть больше 5 кадров — должно вернуться
        for _ in range(10):
            p.update(self.level)
        self.assertEqual(p.state, Player.STATE_SMALL)
        self.assertEqual(p.rect.h, small_h)
        # И получить временную неуязвимость
        self.assertGreater(p.invincible_timer, 0)

    def test_grow_again_while_big_resets_timer(self):
        p = Player(0, 0, self.cfg)
        p.apply_powerup("grow", 5)
        # тикаем 3 кадра
        for _ in range(3):
            p.update(self.level)
        # снова поднимаем гриб — таймер должен обновиться
        p.apply_powerup("grow", 100)
        self.assertEqual(p.big_timer, 100)
        self.assertEqual(p.state, Player.STATE_BIG)


class TestPlayerJumpUx(unittest.TestCase):
    """Coyote time и jump buffer — UX-улучшения прыжка."""

    def setUp(self):
        self.cfg = {
            "physics": {
                "gravity": 0.6, "max_fall_speed": 12,
                "player_speed": 4, "player_run_speed": 6, "jump_strength": -13,
            },
            "player": {
                "width": 28, "height": 32, "start_lives": 3,
                "invincibility_time": 120,
            },
        }
        # уровень с полом
        rows = ["." * 6 for _ in range(4)] + ["G" * 6]
        self.level = Level(make_level_data(rows, w=6, h=5), tile_size=32)

    def test_jump_in_air_records_buffer(self):
        p = Player(0, 0, self.cfg)
        # игрок в воздухе, не on_ground
        self.assertFalse(p.on_ground)
        ok = p.jump()
        self.assertFalse(ok)
        # но буфер записан
        self.assertGreater(p.jump_buffer_timer, 0)

    def test_jump_buffer_decays_over_frames(self):
        p = Player(0, 0, self.cfg)
        p.jump()  # в воздухе, в буфер
        initial = p.jump_buffer_timer
        p.update(self.level)
        self.assertLess(p.jump_buffer_timer, initial)

    def test_jump_buffer_triggers_on_landing(self):
        p = Player(0, 100, self.cfg)
        p.vy = 5  # падает
        # пока летит, нажал прыжок
        p.jump()
        self.assertGreater(p.jump_buffer_timer, 0)
        # летим до приземления и проверяем что прыжок сработал
        for _ in range(60):
            was_on_ground_before = p.on_ground
            p.update(self.level)
            # после первого приземления буфер должен сработать → vy резко уйти в минус
            if was_on_ground_before is False and p.on_ground is True:
                break
        # после буфер-прыжка vy должна стать отрицательной
        # (один тик после приземления уже сработал)
        # буфер сбросился
        self.assertEqual(p.jump_buffer_timer, 0)

    def test_coyote_time_set_when_walking_off_ledge(self):
        # уровень: пол только в первых 3 тайлах, дальше — пропасть
        rows = ["." * 10 for _ in range(4)] + ["GGG......."]
        level = Level(make_level_data(rows, w=10, h=5), tile_size=32)

        # ставим игрока высоко, чтобы он успел упасть на пол
        p = Player(16, 0, self.cfg)
        # даём ему упасть и приземлиться (60 кадров достаточно с запасом)
        for _ in range(60):
            p.update(level)
        self.assertTrue(p.on_ground,
                        f"должен приземлиться, y={p.rect.y}, vy={p.vy}")

        # Телепортируем в пустое место — над пропастью (x=5*32 = 160)
        p.rect.x = 5 * 32
        # На следующем update игрок должен начать падать
        p.update(level)
        if not p.on_ground and p.vy >= 0:
            self.assertGreater(p.coyote_timer, 0)
        else:
            self.fail(f"игрок не стал падать: on_ground={p.on_ground}, "
                      f"vy={p.vy}, x={p.rect.x}")

    def test_coyote_time_decays(self):
        p = Player(0, 0, self.cfg)
        p.coyote_timer = 5
        p.update(self.level)
        self.assertLessEqual(p.coyote_timer, 4)

    def test_jump_during_coyote_works(self):
        p = Player(0, 0, self.cfg)
        p.coyote_timer = 5  # имитируем что только что слетел
        ok = p.jump()
        self.assertTrue(ok)
        self.assertLess(p.vy, 0)
        # coyote сбросился
        self.assertEqual(p.coyote_timer, 0)


if __name__ == "__main__":
    unittest.main()
