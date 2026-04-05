from __future__ import annotations

import unittest

from app.domain.pagination import Paginator


class PaginatorTests(unittest.TestCase):
    def test_paginate_returns_expected_slice(self) -> None:
        page = Paginator[int]().paginate(list(range(1, 31)), page_number=2, page_size=10)
        self.assertEqual(list(range(11, 21)), page.items)
        self.assertEqual(30, page.total_items)
        self.assertEqual(3, page.total_pages)

    def test_paginate_normalizes_page_number(self) -> None:
        page = Paginator[int]().paginate(list(range(5)), page_number=99, page_size=2)
        self.assertEqual(3, page.page_number)
        self.assertEqual([4], page.items)

    def test_paginate_rejects_non_positive_page_size(self) -> None:
        with self.assertRaises(ValueError):
            Paginator[int]().paginate([1, 2, 3], page_number=1, page_size=0)


if __name__ == "__main__":
    unittest.main()