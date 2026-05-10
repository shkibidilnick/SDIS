"""Тесты для Rect."""
import unittest

from src.utils.geometry import Rect


class TestRect(unittest.TestCase):
    def test_creation(self):
        r = Rect(1, 2, 10, 20)
        self.assertEqual(r.x, 1)
        self.assertEqual(r.y, 2)
        self.assertEqual(r.w, 10)
        self.assertEqual(r.h, 20)

    def test_edges(self):
        r = Rect(10, 20, 30, 40)
        self.assertEqual(r.left, 10)
        self.assertEqual(r.right, 40)
        self.assertEqual(r.top, 20)
        self.assertEqual(r.bottom, 60)

    def test_center(self):
        r = Rect(0, 0, 10, 20)
        self.assertEqual(r.center, (5, 10))

    def test_intersects_overlap(self):
        a = Rect(0, 0, 10, 10)
        b = Rect(5, 5, 10, 10)
        self.assertTrue(a.intersects(b))
        self.assertTrue(b.intersects(a))

    def test_intersects_touching_not_intersecting(self):
        # Touching at edge — наш inteersects использует строгое "<=" => не пересекаются
        a = Rect(0, 0, 10, 10)
        b = Rect(10, 0, 10, 10)
        self.assertFalse(a.intersects(b))

    def test_intersects_disjoint(self):
        a = Rect(0, 0, 5, 5)
        b = Rect(100, 100, 5, 5)
        self.assertFalse(a.intersects(b))

    def test_intersects_inside(self):
        a = Rect(0, 0, 100, 100)
        b = Rect(40, 40, 5, 5)
        self.assertTrue(a.intersects(b))

    def test_contains_point(self):
        r = Rect(0, 0, 10, 10)
        self.assertTrue(r.contains_point(5, 5))
        self.assertTrue(r.contains_point(0, 0))
        self.assertFalse(r.contains_point(10, 10))  # верхняя граница исключена
        self.assertFalse(r.contains_point(-1, 5))

    def test_copy(self):
        r = Rect(1, 2, 3, 4)
        c = r.copy()
        self.assertEqual(r, c)
        c.x = 100
        self.assertNotEqual(r, c)

    def test_eq(self):
        a = Rect(1, 1, 2, 2)
        b = Rect(1, 1, 2, 2)
        c = Rect(1, 1, 3, 2)
        self.assertEqual(a, b)
        self.assertNotEqual(a, c)
        self.assertNotEqual(a, "not a rect")

    def test_repr(self):
        self.assertIn("Rect", repr(Rect(1, 2, 3, 4)))


if __name__ == "__main__":
    unittest.main()
