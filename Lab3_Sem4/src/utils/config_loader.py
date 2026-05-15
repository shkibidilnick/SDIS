"""
ConfigLoader — loads JSON/YAML configs from disk.
Single Responsibility: only reads and caches config files.
"""
import json
import os
from typing import Any


class ConfigLoader:
    """Loads and caches configuration files (JSON / YAML)."""

    _cache: dict[str, Any] = {}

    @classmethod
    def load(cls, path: str) -> Any:
        """Return parsed config dict, using in-memory cache."""
        if path in cls._cache:
            return cls._cache[path]

        if not os.path.exists(path):
            raise FileNotFoundError(f"Config not found: {path}")

        ext = os.path.splitext(path)[1].lower()
        with open(path, "r", encoding="utf-8") as fh:
            if ext == ".json":
                data = json.load(fh)
            elif ext in (".yaml", ".yml"):
                try:
                    import yaml
                    data = yaml.safe_load(fh)
                except ImportError as exc:
                    raise ImportError("PyYAML is required for YAML configs.") from exc
            else:
                raise ValueError(f"Unsupported config format: {ext}")

        cls._cache[path] = data
        return data

    @classmethod
    def save_json(cls, path: str, data: Any) -> None:
        """Persist data back to a JSON file and update cache."""
        with open(path, "w", encoding="utf-8") as fh:
            json.dump(data, fh, indent=2, ensure_ascii=False)
        cls._cache[path] = data

    @classmethod
    def clear_cache(cls) -> None:
        """Clear the in-memory cache (useful in tests)."""
        cls._cache.clear()
