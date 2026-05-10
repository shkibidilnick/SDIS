"""Тесты для Enemy и EnemyFactory."""
import unittest

from src.entities.enemy import Enemy, EnemyFactory
from src.entities.level import Level
from tests.test_level import make_level_data


def goomba_type():
    return {
        "type": "goomba", "name": "Goomba",
        "width": 30, "height": 30, "speed": 1.0, "hp": 1,
        "color": [139, 69, 19], "score_value": 100,
        "behavior": "walk", "can_be_stomped": True
    }


def koopa_type():
    return {
        "type": "koopa", "name": "Koopa",
        "width": 30, "height": 30, "speed": 1.5, "hp": 1,
        "color": [34, 139, 34], "score_value": 200,
        "behavior": "walk_smart", "can_be_stomped": True
    }


class TestEnemy(unittest.TestCase):
    def test_creation(self):
        e = Enemy(0, 0, goomba_type(), gravity=0.6, max_fall_speed=12)
        self.assertEqual(e.type_name, "goomba")
        self.assertEqual(e.score_value, 100)
        self.assertTrue(e.alive)
        # стартует движется влево
        self.assertLess(e.vx, 0)

    def test_stomp_yields_score_and_dies(self):
        e = Enemy(0, 0, goomba_type(), gravity=0.6, max_fall_speed=12)
        score = e.stomp()
        self.assertEqual(score, 100)
        self.assertTrue(e.dying)

    def test_stomp_twice_returns_zero_second_time(self):
        e = Enemy(0, 0, goomba_type(), gravity=0.6, max_fall_speed=12)
        e.stomp()
        self.assertEqual(e.stomp(), 0)

    def test_kill_by_star(self):
        e = Enemy(0, 0, koopa_type(), gravity=0.6, max_fall_speed=12)
        score = e.kill_by_star()
        self.assertEqual(score, 200)
        self.assertTrue(e.dying)
        # летит вверх
        self.assertLess(e.vy, 0)

    def test_kill_by_star_after_dying(self):
        e = Enemy(0, 0, goomba_type(), gravity=0.6, max_fall_speed=12)
        e.stomp()
        self.assertEqual(e.kill_by_star(), 0)

    def test_dying_animation_finishes(self):
        e = Enemy(0, 0, goomba_type(), gravity=0.6, max_fall_speed=12)
        lvl = Level(make_level_data(["...", "...", "GGG"], w=3, h=3), tile_size=10)
        e.stomp()
        for _ in range(60):
            e.update(lvl)
        self.assertFalse(e.alive)

    def test_walk_reverses_on_wall(self):
        # пол по всей ширине, стенка из 2 блоков справа на x=4
        # враг 8x8 идёт вправо, должен упереться в стенку и развернуться
        import random
        random.seed(42)
        data = make_level_data(
            ["......", "....G.", "....G.", "GGGGGG"], w=6, h=4
        )
        lvl = Level(data, tile_size=10)
        small = goomba_type()
        small["width"] = 8
        small["height"] = 8
        # враг стоит на полу (y=22): rect.y=22, h=8, bottom=30, пол на y=30..40
        e = Enemy(0, 22, small, gravity=0.6, max_fall_speed=12)
        e.speed = 1.0
        e.vx = 1.0
        # после удара в стену должен пойти влево
        for _ in range(120):
            e.update(lvl)
        # стенка на x=40..50, враг шириной 8 должен остановиться вплотную или раньше
        self.assertLessEqual(e.rect.right, 42)
        # и направление должно быть отрицательным после отскока
        # (даём ещё немного, чтобы он сделал шаг назад)
        for _ in range(5):
            e.update(lvl)
        self.assertLess(e.vx, 0)

    def test_walk_smart_avoids_pit(self):
        # 3 тайла платформы слева, потом обрыв на оставшихся 3 тайлах
        import random
        random.seed(42)
        data = make_level_data(["......", "......", "GGG..."], w=6, h=3)
        lvl = Level(data, tile_size=10)
        small = koopa_type()
        small["width"] = 8
        small["height"] = 8
        # враг стоит на платформе: y=12, bottom=20, пол на y=20..30
        e = Enemy(2, 12, small, gravity=0.6, max_fall_speed=12)
        e.speed = 1.0
        e.vx = 1.0
        for _ in range(80):
            e.update(lvl)
        # умный враг развернулся и не упал
        self.assertTrue(e.alive)
        # не вышел за конец платформы (платформа занимает x=0..30)
        self.assertLessEqual(e.rect.right, 32)

    def test_falls_off_world(self):
        lvl = Level(make_level_data(["..."], w=3, h=1), tile_size=10)
        e = Enemy(0, 0, goomba_type(), gravity=0.6, max_fall_speed=12)
        e.rect.y = 1000
        e.update(lvl)
        self.assertFalse(e.alive)


class TestEnemyFactory(unittest.TestCase):
    def test_create_all(self):
        cfg = {
            "enemies": [goomba_type(), koopa_type()],
            "spawns": [
                {"type": "goomba", "x": 5, "y": 10},
                {"type": "koopa", "x": 7, "y": 10},
            ],
        }
        f = EnemyFactory(cfg, gravity=0.6, max_fall_speed=12, tile_size=10)
        enemies = f.create_all()
        self.assertEqual(len(enemies), 2)
        self.assertEqual(enemies[0].type_name, "goomba")
        self.assertEqual(enemies[1].type_name, "koopa")

    def test_get_type(self):
        cfg = {"enemies": [goomba_type()], "spawns": []}
        f = EnemyFactory(cfg, gravity=0.6, max_fall_speed=12, tile_size=10)
        self.assertEqual(f.get_type("goomba")["score_value"], 100)

    def test_create_no_spawns(self):
        f = EnemyFactory({"enemies": [goomba_type()], "spawns": []},
                         gravity=0.6, max_fall_speed=12, tile_size=10)
        self.assertEqual(f.create_all(), [])

def piranha_type():
    return {
        "type": "piranha", "name": "Пиранья",
        "width": 28, "height": 32, "speed": 0, "hp": 1,
        "color": [60, 200, 60], "score_value": 200,
        "behavior": "piranha", "can_be_stomped": False
    }


class TestPiranha(unittest.TestCase):
    """Тесты особого поведения пираньи."""

    def setUp(self):
        # Уровень не нужен для пираньи, но требуется в update()
        self.level = Level(make_level_data(["..."], w=3, h=1), tile_size=10)

    def test_piranha_initial_state_hidden(self):
        e = Enemy(64, 320, piranha_type(), gravity=0.6, max_fall_speed=12)
        self.assertEqual(e._piranha_state, "hidden")
        # начальная позиция — внутри трубы (на 32 ниже верха)
        self.assertEqual(e.rect.y, 320 + 32)

    def test_piranha_emerges_after_initial_delay(self):
        e = Enemy(64, 320, piranha_type(), gravity=0.6, max_fall_speed=12)
        # через 90 кадров (стартовая пауза) должна начать вылезать
        for _ in range(90):
            e.update(self.level)
        # на следующем кадре уже emerging
        e.update(self.level)
        self.assertIn(e._piranha_state, ("emerging", "shown"))

    def test_piranha_full_cycle(self):
        e = Enemy(64, 320, piranha_type(), gravity=0.6, max_fall_speed=12)
        states_seen = set()
        for _ in range(800):  # достаточно для полного цикла + ещё
            e.update(self.level)
            states_seen.add(e._piranha_state)
        # все 4 состояния должны были произойти
        self.assertEqual(states_seen,
                         {"hidden", "emerging", "shown", "retracting"})

    def test_piranha_position_when_shown(self):
        e = Enemy(64, 320, piranha_type(), gravity=0.6, max_fall_speed=12)
        # промотать до состояния shown
        for _ in range(500):
            e.update(self.level)
            if e._piranha_state == "shown":
                break
        self.assertEqual(e._piranha_state, "shown")
        # в shown пиранья находится ВЫШЕ верха трубы (полностью вылезла)
        self.assertEqual(e.rect.y, 320 - 32)

    def test_piranha_animates_mouth(self):
        e = Enemy(64, 320, piranha_type(), gravity=0.6, max_fall_speed=12)
        frames_seen = set()
        for _ in range(400):
            e.update(self.level)
            frames_seen.add(e.anim_frame)
        # должны быть оба кадра анимации рта
        self.assertIn(0, frames_seen)
        self.assertIn(1, frames_seen)

    def test_piranha_no_gravity_no_collisions(self):
        e = Enemy(64, 320, piranha_type(), gravity=0.6, max_fall_speed=12)
        e.update(self.level)
        # vy не должна меняться от гравитации
        self.assertEqual(e.vy, 0)

    def test_piranha_dying_animation(self):
        e = Enemy(64, 320, piranha_type(), gravity=0.6, max_fall_speed=12)
        score = e.kill_by_star()
        self.assertEqual(score, 200)
        self.assertTrue(e.dying)
        for _ in range(60):
            e.update(self.level)
        self.assertFalse(e.alive)

    def test_piranha_cannot_be_stomped(self):
        e = Enemy(64, 320, piranha_type(), gravity=0.6, max_fall_speed=12)
        self.assertFalse(e.can_be_stomped)


class TestEnemyFactoryPiranha(unittest.TestCase):
    """Фабрика правильно создаёт пираний."""

    def test_create_piranha_uses_pipe_top_y(self):
        cfg = {
            "enemies": [piranha_type()],
            "spawns": [{"type": "piranha", "x": 5, "y": 12}],
        }
        f = EnemyFactory(cfg, gravity=0.6, max_fall_speed=12, tile_size=32)
        enemy = f.create_all()[0]
        self.assertEqual(enemy.type_name, "piranha")
        # пиранья запоминает верх трубы как _piranha_pipe_top_y
        self.assertEqual(enemy._piranha_pipe_top_y, 12 * 32)

    def test_create_piranha_centered_x(self):
        cfg = {
            "enemies": [piranha_type()],
            "spawns": [{"type": "piranha", "x": 5, "y": 12}],
        }
        f = EnemyFactory(cfg, gravity=0.6, max_fall_speed=12, tile_size=32)
        enemy = f.create_all()[0]
        # x центрирован: 5*32 + (32-28)//2 = 160 + 2 = 162
        self.assertEqual(enemy.rect.x, 162)

if __name__ == "__main__":
    unittest.main()
