from __future__ import annotations

import sys
from pathlib import Path

from PySide6.QtWidgets import QApplication

from app.controllers.main_window_controller import MainWindowController
from app.repositories.sqlite_repository import SQLitePetRecordRepository
from app.services.delete_service import DeleteService
from app.services.import_export_service import ImportExportService
from app.services.record_service import PetRecordService
from app.services.search_service import SearchService
from app.views.main_window import MainWindow


def build_application() -> MainWindowController:
    base_dir = Path(__file__).resolve().parent.parent
    data_dir = base_dir / "data"
    data_dir.mkdir(exist_ok=True)
    db_path = data_dir / "pet_records.sqlite3"

    repository = SQLitePetRecordRepository(db_path)
    record_service = PetRecordService(repository)
    search_service = SearchService(repository)
    delete_service = DeleteService(repository)
    import_export_service = ImportExportService(record_service)

    view = MainWindow()
    return MainWindowController(view, record_service, search_service, delete_service, import_export_service)


def main() -> int:
    app = QApplication(sys.argv)
    controller = build_application()
    controller.show()
    return app.exec()


if __name__ == "__main__":
    raise SystemExit(main())
