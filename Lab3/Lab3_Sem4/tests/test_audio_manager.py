"""Тесты AudioManager.

Так как pygame.mixer может быть недоступен в тестовой среде (нет аудиоустройства),
в основном тестируем что менеджер не падает (no-op) и WAV-генератор работает
самостоятельно.
"""
import os
import struct
import sys
import tempfile
import unittest
import wave

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from src.utils.audio_manager import AudioManager  # noqa: E402


class TestAudioManagerInit(unittest.TestCase):
    """Проверяем поведение конструктора и поля volumes."""

    def test_default_volumes(self):
        am = AudioManager()
        self.assertAlmostEqual(am.music_volume, 0.4)
        self.assertAlmostEqual(am.sfx_volume, 0.6)

    def test_custom_volumes(self):
        am = AudioManager(music_volume=0.1, sfx_volume=0.9)
        self.assertAlmostEqual(am.music_volume, 0.1)
        self.assertAlmostEqual(am.sfx_volume, 0.9)

    def test_initially_unavailable(self):
        am = AudioManager()
        # до init() звук недоступен
        self.assertFalse(am.is_available())

    def test_no_sounds_initially(self):
        am = AudioManager()
        self.assertEqual(am._sounds, {})
        self.assertIsNone(am._music_path)


class TestAudioManagerNoOp(unittest.TestCase):
    """Все методы должны быть безопасны при недоступном миксере."""

    def setUp(self):
        self.am = AudioManager()
        # _available по умолчанию False, ничего не инициализируем

    def test_play_sfx_no_op(self):
        # не должно падать
        self.am.play_sfx("jump")
        self.am.play_sfx("nonexistent_sound")

    def test_play_music_no_op(self):
        self.am.play_music()
        self.am.play_music(loops=0)

    def test_stop_music_no_op(self):
        self.am.stop_music()


class TestWavGeneration(unittest.TestCase):
    """Генератор WAV не зависит от pygame и должен работать всегда."""

    def test_generate_sine_wav(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = os.path.join(tmp, "test_sine.wav")
            AudioManager._generate_wav(path, freq=440.0, duration=0.05,
                                       wave_type="sine")
            self.assertTrue(os.path.exists(path))
            with wave.open(path, "rb") as w:
                self.assertEqual(w.getnchannels(), 1)
                self.assertEqual(w.getsampwidth(), 2)
                self.assertEqual(w.getframerate(), AudioManager.SAMPLE_RATE)
                # длина файла соответствует длительности
                expected_frames = int(AudioManager.SAMPLE_RATE * 0.05)
                self.assertEqual(w.getnframes(), expected_frames)

    def test_generate_square_wav(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = os.path.join(tmp, "test_square.wav")
            AudioManager._generate_wav(path, freq=300.0, duration=0.05,
                                       wave_type="square")
            self.assertTrue(os.path.exists(path))
            with wave.open(path, "rb") as w:
                # есть кадры
                self.assertGreater(w.getnframes(), 0)

    def test_generate_saw_wav(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = os.path.join(tmp, "test_saw.wav")
            AudioManager._generate_wav(path, freq=200.0, duration=0.05,
                                       wave_type="saw")
            self.assertTrue(os.path.exists(path))

    def test_generate_unknown_wave_type_falls_back_to_sine(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = os.path.join(tmp, "test_unknown.wav")
            # неизвестный тип не должен падать (fallback на sine)
            AudioManager._generate_wav(path, freq=440.0, duration=0.05,
                                       wave_type="triangle")
            self.assertTrue(os.path.exists(path))

    def test_generated_wav_amplitude_in_int16_range(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = os.path.join(tmp, "amp.wav")
            AudioManager._generate_wav(path, freq=440.0, duration=0.05,
                                       wave_type="sine")
            with wave.open(path, "rb") as w:
                frames = w.readframes(w.getnframes())
            n = len(frames) // 2
            samples = struct.unpack(f"<{n}h", frames)
            for s in samples:
                self.assertGreaterEqual(s, -32768)
                self.assertLessEqual(s, 32767)

    def test_generate_music(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = os.path.join(tmp, "music.wav")
            AudioManager._generate_music(path)
            self.assertTrue(os.path.exists(path))
            with wave.open(path, "rb") as w:
                self.assertEqual(w.getnchannels(), 1)
                self.assertEqual(w.getframerate(), AudioManager.SAMPLE_RATE)
                # 16 нот по 0.5с = 8с
                expected_frames = int(AudioManager.SAMPLE_RATE * 0.5 * 16)
                self.assertEqual(w.getnframes(), expected_frames)


class TestAudioManagerInitWithoutPygame(unittest.TestCase):
    """init() не должен ничего ломать даже если pygame недоступен."""

    def test_init_safe_without_pygame(self):
        am = AudioManager()
        with tempfile.TemporaryDirectory() as tmp:
            cache = os.path.join(tmp, "sounds")
            # Просто не должно бросать исключение
            try:
                am.init(cache_dir=cache)
            except Exception as e:
                self.fail(f"init() бросил исключение без аудио: {e}")
        # после неудачного init аудио всё ещё недоступно
        # (либо доступно, если повезло — это тоже валидно)
        self.assertIsInstance(am.is_available(), bool)


if __name__ == "__main__":
    unittest.main()
