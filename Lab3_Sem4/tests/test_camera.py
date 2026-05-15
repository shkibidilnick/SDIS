"""Tests for Camera."""
import unittest
import pygame

from src.utils.camera import Camera

pygame.init()
pygame.display.set_mode((1, 1), pygame.NOFRAME)


def _make_camera(world_w=5000, screen_w=800, screen_h=600,
                 threshold_right=400, threshold_left=200):
    settings = {"camera": {
        "scroll_threshold_right": threshold_right,
        "scroll_threshold_left":  threshold_left,
    }}
    return Camera(world_w, screen_w, screen_h, settings)


class TestCamera(unittest.TestCase):

    def test_initial_offset_zero(self):
        cam = _make_camera()
        self.assertEqual(cam.offset_x, 0)

    def test_no_scroll_within_threshold(self):
        cam = _make_camera()
        rect = pygame.Rect(200, 300, 24, 28)
        cam.update(rect)
        self.assertEqual(cam.offset_x, 0)

    def test_scrolls_right_when_past_threshold(self):
        cam = _make_camera()
        rect = pygame.Rect(500, 300, 24, 28)
        cam.update(rect)
        self.assertGreater(cam.offset_x, 0)

    def test_offset_clamped_to_zero(self):
        cam = _make_camera()
        rect = pygame.Rect(10, 300, 24, 28)
        cam.update(rect)
        self.assertGreaterEqual(cam.offset_x, 0)

    def test_offset_clamped_to_max(self):
        cam = _make_camera(world_w=1000, screen_w=800)
        rect = pygame.Rect(9000, 300, 24, 28)
        cam.update(rect)
        self.assertLessEqual(cam.offset_x, 200)   # 1000 - 800

    def test_apply_shifts_rect(self):
        cam = _make_camera()
        cam.offset_x = 100
        r = pygame.Rect(300, 50, 24, 28)
        sr = cam.apply(r)
        self.assertEqual(sr.x, 200)
        self.assertEqual(sr.y, 50)

    def test_reset(self):
        cam = _make_camera()
        cam.offset_x = 500
        cam.reset()
        self.assertEqual(cam.offset_x, 0)

    def test_apply_does_not_mutate_original(self):
        cam = _make_camera()
        cam.offset_x = 50
        original = pygame.Rect(100, 80, 24, 28)
        cam.apply(original)
        self.assertEqual(original.x, 100)


if __name__ == "__main__":
    unittest.main()
