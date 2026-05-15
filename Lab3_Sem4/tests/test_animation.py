"""Tests for Animation and build_colored_frames."""
import unittest
import pygame

pygame.init()
pygame.display.set_mode((1, 1), pygame.NOFRAME)

from src.utils.animation import Animation, build_colored_frames


def _make_frames(n=3):
    return [pygame.Surface((10, 10)) for _ in range(n)]


class TestAnimation(unittest.TestCase):

    def test_initial_frame_is_first(self):
        frames = _make_frames(3)
        anim = Animation(frames, speed=5)
        self.assertIs(anim.current_frame, frames[0])

    def test_advances_after_speed_ticks(self):
        frames = _make_frames(3)
        anim = Animation(frames, speed=3)
        for _ in range(3):
            anim.update()
        self.assertIs(anim.current_frame, frames[1])

    def test_loops_back_to_start(self):
        frames = _make_frames(2)
        anim = Animation(frames, speed=1, loop=True)
        anim.update()   # → frame 1
        anim.update()   # → back to frame 0
        self.assertIs(anim.current_frame, frames[0])

    def test_no_loop_stops_at_last(self):
        frames = _make_frames(2)
        anim = Animation(frames, speed=1, loop=False)
        for _ in range(10):
            anim.update()
        self.assertIs(anim.current_frame, frames[-1])
        self.assertTrue(anim.finished)

    def test_reset_restores_state(self):
        frames = _make_frames(3)
        anim = Animation(frames, speed=1)
        for _ in range(5):
            anim.update()
        anim.reset()
        self.assertIs(anim.current_frame, frames[0])
        self.assertFalse(anim.finished)

    def test_finished_flag_false_initially(self):
        anim = Animation(_make_frames(), speed=5)
        self.assertFalse(anim.finished)

    def test_speed_minimum_one(self):
        anim = Animation(_make_frames(), speed=0)
        # Should not raise; speed clamped to 1
        anim.update()


class TestBuildColoredFrames(unittest.TestCase):

    def test_returns_correct_count(self):
        frames = build_colored_frames((255, 0, 0), (10, 10), 4)
        self.assertEqual(len(frames), 4)

    def test_each_frame_is_surface(self):
        frames = build_colored_frames((0, 255, 0), (8, 8), 2)
        for f in frames:
            self.assertIsInstance(f, pygame.Surface)

    def test_variants_applied(self):
        v = [(255, 0, 0), (0, 255, 0)]
        frames = build_colored_frames((0, 0, 0), (4, 4), 2, variants=v)
        self.assertEqual(len(frames), 2)


if __name__ == "__main__":
    unittest.main()
