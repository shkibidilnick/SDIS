from __future__ import annotations

from dataclasses import dataclass
from math import ceil
from typing import Generic, Sequence, TypeVar

T = TypeVar("T")


@dataclass(frozen=True)
class Page(Generic[T]):
    items: list[T]
    page_number: int
    page_size: int
    total_items: int

    @property
    def total_pages(self) -> int:
        return max(1, ceil(self.total_items / self.page_size)) if self.page_size > 0 else 1


class Paginator(Generic[T]):
    def paginate(self, items: Sequence[T], page_number: int, page_size: int) -> Page[T]:
        if page_size <= 0:
            raise ValueError("Размер страницы должен быть положительным.")
        total_items = len(items)
        total_pages = max(1, ceil(total_items / page_size))
        normalized_page = min(max(1, page_number), total_pages)
        start = (normalized_page - 1) * page_size
        end = start + page_size
        return Page(list(items[start:end]), normalized_page, page_size, total_items)
