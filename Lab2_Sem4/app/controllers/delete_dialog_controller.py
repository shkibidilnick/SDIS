from __future__ import annotations

from app.services.delete_service import DeleteService
from app.views.delete_dialog import DeleteDialog


class DeleteDialogController:
    def __init__(self, dialog: DeleteDialog, delete_service: DeleteService, on_deleted_callback) -> None:
        self._dialog = dialog
        self._delete_service = delete_service
        self._on_deleted_callback = on_deleted_callback
        self._dialog.delete_button.clicked.connect(self._on_delete_clicked)

    def show(self) -> None:
        self._dialog.exec()

    def _on_delete_clicked(self) -> None:
        try:
            payload = self._dialog.build_criteria_payload()
            kind = payload["kind"]

            if kind == 0:
                deleted = self._delete_service.delete_by_pet_name_and_birth_date(
                    payload["pet_name"],
                    payload["birth_date"],
                )
            elif kind == 1:
                deleted = self._delete_service.delete_by_visit_date_and_veterinarian(
                    payload["last_visit_date"],
                    payload["veterinarian_full_name"],
                )
            else:
                deleted = self._delete_service.delete_by_diagnosis_phrase(payload["phrase"])

            self._dialog.show_result(deleted)
            self._on_deleted_callback()
        except ValueError as error:
            self._dialog.show_validation_error(str(error))