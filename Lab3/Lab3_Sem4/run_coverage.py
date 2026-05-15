#!/usr/bin/env python3
"""Измеряет покрытие тестами для модулей в src/.

Если установлен `coverage` — использует его (точнее).
Иначе использует встроенный модуль `trace`.

Использование:
    python run_coverage.py
"""
import os
import sys
import unittest


ROOT = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(ROOT, "src")


def _run_with_coverage():
    """Запуск с пакетом coverage (если доступен)."""
    import coverage  # type: ignore
    cov = coverage.Coverage(source=[SRC], omit=[
        "*/__init__.py",
        "*/scenes/*",        # сцены требуют pygame и тестируются интеграционно
        "*/utils/renderer.py",  # рендер требует pygame
        "*/app.py",          # главный цикл с pygame
        "*/main.py",
    ])
    cov.start()
    loader = unittest.TestLoader()
    tests = loader.discover(os.path.join(ROOT, "tests"))
    runner = unittest.TextTestRunner(verbosity=1)
    result = runner.run(tests)
    cov.stop()
    cov.save()
    print("\n" + "=" * 70)
    print("ПОКРЫТИЕ КОДА")
    print("=" * 70)
    total_percent = cov.report(show_missing=False)
    print("\nИтоговое покрытие: {:.1f}%".format(total_percent))
    sys.exit(0 if result.wasSuccessful() else 1)


def _run_with_trace():
    """Запуск с модулем trace (fallback)."""
    import trace
    tracer = trace.Trace(count=True, trace=False, ignoredirs=[sys.prefix])

    def runner():
        loader = unittest.TestLoader()
        tests = loader.discover(os.path.join(ROOT, "tests"))
        unittest.TextTestRunner(verbosity=1).run(tests)

    tracer.runfunc(runner)
    results = tracer.results()

    # Анализируем покрытие только для src/, исключая то же что в _run_with_coverage
    total_lines = 0
    covered_lines = 0
    per_file = []
    for path, lines in results.counts.items():
        # path — кортеж (filename, lineno)
        pass

    # results.counts — dict {(file, line): count}
    # Сначала найдём все исполняемые строки в src/
    src_files = []
    for root_dir, _, files in os.walk(SRC):
        for fn in files:
            if not fn.endswith(".py"):
                continue
            full = os.path.join(root_dir, fn)
            rel = os.path.relpath(full, ROOT)
            # пропускаем сцены, рендер, app.py, main.py
            if (rel.endswith("__init__.py")
                    or "scenes" in rel
                    or rel.endswith("renderer.py")
                    or rel.endswith("app.py")):
                continue
            src_files.append(full)

    for fpath in src_files:
        # выполнимые строки = все строки кода (грубо, без учёта docstrings)
        with open(fpath, encoding="utf-8") as f:
            lines = f.readlines()
        executable = []
        in_docstring = False
        for i, line in enumerate(lines, 1):
            stripped = line.strip()
            if not stripped:
                continue
            if stripped.startswith("#"):
                continue
            # очень грубая проверка docstring
            if stripped.startswith('"""') or stripped.startswith("'''"):
                # счётчик кавычек на строке
                triple_count = stripped.count('"""') + stripped.count("'''")
                if triple_count >= 2:
                    continue  # однострочный docstring
                in_docstring = not in_docstring
                continue
            if in_docstring:
                continue
            executable.append(i)

        executed = set()
        for (file, lineno), count in results.counts.items():
            if os.path.abspath(file) == os.path.abspath(fpath) and count > 0:
                executed.add(lineno)

        file_total = len(executable)
        file_covered = sum(1 for ln in executable if ln in executed)
        per_file.append((os.path.relpath(fpath, ROOT), file_covered, file_total))
        total_lines += file_total
        covered_lines += file_covered

    print("\n" + "=" * 70)
    print("ПОКРЫТИЕ КОДА (через trace)")
    print("=" * 70)
    print(f"{'Файл':<45} {'Покрыто':>12} {'%':>6}")
    print("-" * 70)
    for fname, cov, tot in sorted(per_file):
        pct = (cov / tot * 100) if tot else 100
        print(f"{fname:<45} {cov:>4}/{tot:<7} {pct:>5.1f}%")
    print("-" * 70)
    overall = (covered_lines / total_lines * 100) if total_lines else 0
    print(f"{'ИТОГО':<45} {covered_lines:>4}/{total_lines:<7} {overall:>5.1f}%")
    print("=" * 70)
    if overall < 85:
        print("⚠️  Покрытие ниже 85%")
        sys.exit(1)
    else:
        print("✅ Покрытие выше требуемых 85%")
    sys.exit(0)


def main():
    sys.path.insert(0, ROOT)
    try:
        import coverage  # noqa: F401
        _run_with_coverage()
    except ImportError:
        print("(пакет 'coverage' не найден — использую trace-fallback)\n")
        _run_with_trace()


if __name__ == "__main__":
    main()
