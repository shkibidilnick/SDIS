"""Модуль управления звуками и фоновой музыкой.

Звуки и музыка генерируются программно (синтезом), чтобы исключить зависимость
от внешних аудиофайлов. Этого достаточно для требования "присутствуют звуки и
музыка". Если pygame.mixer недоступен (нет аудиоустройства), все методы
становятся no-op, чтобы игра запускалась и в headless-режиме (для тестов).
"""
import math
import os
import struct
import wave
from typing import Dict, Optional

try:
    import pygame  # type: ignore
except ImportError:  # pragma: no cover
    pygame = None  # тесты могут запускаться без pygame


class AudioManager:
    """Создаёт и проигрывает звуки/музыку. KISS: всё минимально необходимое."""

    SAMPLE_RATE = 22050

    def __init__(self, music_volume: float = 0.4, sfx_volume: float = 0.6):
        self.music_volume = music_volume
        self.sfx_volume = sfx_volume
        self._sounds: Dict[str, "pygame.mixer.Sound"] = {}
        self._available = False
        self._music_path: Optional[str] = None

    def init(self, cache_dir: str = "assets/sounds") -> None:
        """Инициализировать миксер и создать звуки.

        :param cache_dir: каталог, где сохранять сгенерированные WAV.
        """
        if pygame is None:
            return
        try:
            pygame.mixer.init(frequency=self.SAMPLE_RATE, size=-16, channels=1)
            self._available = True
        except pygame.error:
            self._available = False
            return

        os.makedirs(cache_dir, exist_ok=True)
        # Генерируем простые wav-файлы для каждого события
        sfx_specs = {
            "jump": (520.0, 0.12, "square"),
            "coin": (988.0, 0.10, "sine"),
            "powerup": (660.0, 0.25, "sine"),
            "stomp": (180.0, 0.10, "square"),
            "death": (220.0, 0.45, "saw"),
            "level_complete": (784.0, 0.40, "sine"),
            "menu_select": (440.0, 0.06, "square"),
        }
        for name, (freq, dur, wave_type) in sfx_specs.items():
            path = os.path.join(cache_dir, f"{name}.wav")
            if not os.path.exists(path):
                self._generate_wav(path, freq, dur, wave_type)
            try:
                snd = pygame.mixer.Sound(path)
                snd.set_volume(self.sfx_volume)
                self._sounds[name] = snd
            except pygame.error:
                pass

        # фоновая музыка — простая мелодия
        music_path = os.path.join(cache_dir, "music.wav")
        if not os.path.exists(music_path):
            self._generate_music(music_path)
        self._music_path = music_path

    @classmethod
    def _generate_wav(cls, path: str, freq: float, duration: float,
                      wave_type: str = "sine") -> None:
        """Сгенерировать простой WAV-файл одной волны."""
        n_samples = int(cls.SAMPLE_RATE * duration)
        amplitude = 16000
        with wave.open(path, "w") as wav:
            wav.setnchannels(1)
            wav.setsampwidth(2)
            wav.setframerate(cls.SAMPLE_RATE)
            frames = bytearray()
            for i in range(n_samples):
                t = i / cls.SAMPLE_RATE
                # огибающая (плавное затухание)
                env = 1.0 - (i / max(1, n_samples))
                if wave_type == "sine":
                    val = math.sin(2 * math.pi * freq * t)
                elif wave_type == "square":
                    val = 1.0 if math.sin(2 * math.pi * freq * t) > 0 else -1.0
                elif wave_type == "saw":
                    val = 2 * (t * freq - math.floor(t * freq + 0.5))
                else:
                    val = math.sin(2 * math.pi * freq * t)
                sample = int(amplitude * val * env)
                frames += struct.pack("<h", sample)
            wav.writeframes(bytes(frames))

    @classmethod
    def _generate_music(cls, path: str) -> None:
        """Сгенерировать короткую зацикленную мелодию (8 секунд)."""
        # Гамма для весёлой мелодии (C major)
        notes = [262, 330, 392, 523, 392, 330, 262, 392,
                 294, 349, 440, 587, 440, 349, 294, 440]
        note_dur = 0.5
        amplitude = 8000
        with wave.open(path, "w") as wav:
            wav.setnchannels(1)
            wav.setsampwidth(2)
            wav.setframerate(cls.SAMPLE_RATE)
            frames = bytearray()
            for note in notes:
                n_samples = int(cls.SAMPLE_RATE * note_dur)
                for i in range(n_samples):
                    t = i / cls.SAMPLE_RATE
                    # короткая атака + затухание
                    env = min(1.0, i / 500) * (1.0 - i / n_samples * 0.7)
                    val = (math.sin(2 * math.pi * note * t)
                           + 0.3 * math.sin(2 * math.pi * note * 2 * t))
                    sample = int(amplitude * val * env)
                    frames += struct.pack("<h", sample)
            wav.writeframes(bytes(frames))

    def play_sfx(self, name: str) -> None:
        """Проиграть звуковой эффект. Если недоступно — no-op."""
        if not self._available:
            return
        snd = self._sounds.get(name)
        if snd is not None:
            snd.play()

    def play_music(self, loops: int = -1) -> None:
        """Запустить фоновую музыку (по умолчанию зациклено)."""
        if not self._available or self._music_path is None or pygame is None:
            return
        try:
            pygame.mixer.music.load(self._music_path)
            pygame.mixer.music.set_volume(self.music_volume)
            pygame.mixer.music.play(loops)
        except pygame.error:
            pass

    def stop_music(self) -> None:
        """Остановить фоновую музыку."""
        if not self._available or pygame is None:
            return
        try:
            pygame.mixer.music.stop()
        except pygame.error:
            pass

    def is_available(self) -> bool:
        """Доступно ли аудио."""
        return self._available
