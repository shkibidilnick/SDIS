"""Тесты для класса Particle."""
import os
import sys
import unittest

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from src.entities.particle import Particle  # noqa: E402


class TestParticle(unittest.TestCase):
    """Простая физика частиц для анимаций."""

    def test_initial_state(self):
        p = Particle(10, 20, 1.0, -2.0, (255, 0, 0), lifetime=30, size=4)
        self.assertEqual(p.x, 10)
        self.assertEqual(p.y, 20)
        self.assertEqual(p.vx, 1.0)
        self.assertEqual(p.vy, -2.0)
        self.assertEqual(p.color, (255, 0, 0))
        self.assertEqual(p.lifetime, 30)
        self.assertEqual(p.max_lifetime, 30)
        self.assertEqual(p.size, 4)
        self.assertTrue(p.alive)

    def test_default_values(self):
        p = Particle(0, 0, 0, 0, (0, 0, 0))
        self.assertEqual(p.lifetime, 30)
        self.assertEqual(p.size, 3)
        self.assertEqual(p.gravity, 0.3)

    def test_update_moves_particle(self):
        p = Particle(0, 0, 2.0, -3.0, (255, 255, 255), lifetime=10, gravity=0.0)
        p.update()
        # x += vx, y += vy
        self.assertEqual(p.x, 2.0)
        self.assertEqual(p.y, -3.0)

    def test_update_applies_gravity_to_vy(self):
        p = Particle(0, 0, 0, 0, (0, 0, 0), lifetime=10, gravity=0.5)
        p.update()
        # vy сначала += gravity, потом y += vy
        self.assertEqual(p.vy, 0.5)
        self.assertEqual(p.y, 0.5)

    def test_lifetime_decrements_each_update(self):
        p = Particle(0, 0, 0, 0, (0, 0, 0), lifetime=5, gravity=0.0)
        p.update()
        self.assertEqual(p.lifetime, 4)
        p.update()
        self.assertEqual(p.lifetime, 3)

    def test_dies_when_lifetime_zero(self):
        p = Particle(0, 0, 0, 0, (0, 0, 0), lifetime=3, gravity=0.0)
        for _ in range(3):
            p.update()
        self.assertFalse(p.alive)

    def test_no_movement_when_dead(self):
        p = Particle(10, 10, 5, 5, (0, 0, 0), lifetime=1, gravity=0.0)
        p.update()  # после update lifetime=0, alive=False
        x_after_death, y_after_death = p.x, p.y
        # последующие апдейты не должны менять позицию
        p.update()
        p.update()
        self.assertEqual(p.x, x_after_death)
        self.assertEqual(p.y, y_after_death)

    def test_alpha_starts_at_max(self):
        p = Particle(0, 0, 0, 0, (0, 0, 0), lifetime=10)
        self.assertEqual(p.alpha, 255)

    def test_alpha_decreases_over_time(self):
        p = Particle(0, 0, 0, 0, (0, 0, 0), lifetime=10, gravity=0.0)
        a0 = p.alpha
        p.update()
        a1 = p.alpha
        self.assertLess(a1, a0)

    def test_alpha_zero_when_dead(self):
        p = Particle(0, 0, 0, 0, (0, 0, 0), lifetime=2, gravity=0.0)
        p.update()
        p.update()
        self.assertEqual(p.alpha, 0)

    def test_alpha_zero_lifetime_handles_division(self):
        # Корректная защита от деления на ноль (max_lifetime = 0)
        p = Particle(0, 0, 0, 0, (0, 0, 0), lifetime=0, gravity=0.0)
        # ничего не должно упасть
        self.assertEqual(p.alpha, 0)

    def test_alpha_in_valid_range(self):
        p = Particle(0, 0, 0, 0, (0, 0, 0), lifetime=10, gravity=0.0)
        for _ in range(15):
            self.assertGreaterEqual(p.alpha, 0)
            self.assertLessEqual(p.alpha, 255)
            p.update()


if __name__ == "__main__":
    unittest.main()
