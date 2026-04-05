from __future__ import annotations

from app.services.search_service import SearchService
from app.views.search_dialog import SearchDialog


class SearchDialogController:
    def __init__(self, dialog: SearchDialog, search_service: SearchService) -> None:
        self._dialog = dialog
        self._search_service = search_service
        self._page_size = 10
        self._last_payload: dict | None = None
        self._last_kind: int = 0

        self._dialog.search_button.clicked.connect(self._on_search_clicked)
        self._dialog.pagination.page_changed.connect(self._on_page_changed)

    def show(self) -> None:
        self._dialog.exec()

    def _on_search_clicked(self) -> None:
        payload = self._dialog.build_criteria_payload()
        self._last_payload = payload
        self._last_kind = self._dialog.current_criteria_kind()
        self._render_page(1, self._page_size)

    def _on_page_changed(self, page_number: int, page_size: int) -> None:
        self._page_size = page_size
        if self._last_payload is not None:
            self._render_page(page_number, page_size)

    def _render_page(self, page_number: int, page_size: int) -> None:
        payload = self._last_payload or {}
        if self._last_kind == 0:
            page = self._search_service.search_by_pet_name_and_birth_date(
                payload["pet_name"], payload["birth_date"], page_number, page_size
            )
        elif self._last_kind == 1:
            page = self._search_service.search_by_visit_date_and_veterinarian(
                payload["last_visit_date"], payload["veterinarian_full_name"], page_number, page_size
            )
        else:
            page = self._search_service.search_by_diagnosis_phrase(payload["phrase"], page_number, page_size)
        self._dialog.render_page(page)
