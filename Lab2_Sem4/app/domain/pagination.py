from __future__ import annotations

from dataclasses import dataclass
from math import ceil
from typing import Generic, Sequence, TypeVar

T = TypeVar("T")


def _calculate_total_pages(total_items: int, page_size: int) -> int:
    if page_size <= 0:
        return 1
    return max(1, ceil(total_items / page_size))


@dataclass(frozen=True)
class Page(Generic[T]):
    items: list[T]
    page_number: int
    page_size: int
    total_items: int

    @property
    def total_pages(self) -> int:
        return _calculate_total_pages(self.total_items, self.page_size)


class Paginator(Generic[T]):
    @staticmethod
    def paginate(items: Sequence[T], page_number: int, page_size: int) -> Page[T]:
        if page_size <= 0:
            raise ValueError("Размер страницы должен быть положительным.")

        total_items = len(items)
        total_pages = _calculate_total_pages(total_items, page_size)
        normalized_page = min(max(1, page_number), total_pages)

        start = (normalized_page - 1) * page_size
        end = start + page_size

        return Page(
            items=list(items[start:end]),
            page_number=normalized_page,
            page_size=page_size,
            total_items=total_items,
        )