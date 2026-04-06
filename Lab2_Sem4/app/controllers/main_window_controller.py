from __future__ import annotations

from app.controllers.delete_dialog_controller import DeleteDialogController
from app.controllers.search_dialog_controller import SearchDialogController
from app.services.delete_service import DeleteService
from app.services.import_export_service import ImportExportService
from app.services.record_service import PetRecordService
from app.services.search_service import SearchService
from app.views.delete_dialog import DeleteDialog
from app.views.main_window import MainWindow
from app.views.record_dialog import RecordDialog
from app.views.search_dialog import SearchDialog


class MainWindowController:
    def __init__(
        self,
        view: MainWindow,
        record_service: PetRecordService,
        search_service: SearchService,
        delete_service: DeleteService,
        import_export_service: ImportExportService,
    ) -> None:
        self._view = view
        self._record_service = record_service
        self._search_service = search_service
        self._delete_service = delete_service
        self._import_export_service = import_export_service
        self._page_size = 10

        self._wire_events()
        self.refresh_records()

    def _wire_events(self) -> None:
        self._view.add_action.triggered.connect(self.open_add_dialog)
        self._view.search_action.triggered.connect(self.open_search_dialog)
        self._view.delete_action.triggered.connect(self.open_delete_dialog)
        self._view.refresh_action.triggered.connect(self.refresh_records)
        self._view.clear_all_action.triggered.connect(self.clear_all_records)
        self._view.export_xml_action.triggered.connect(self.export_xml)
        self._view.import_xml_action.triggered.connect(self.import_xml)
        self._view.exit_action.triggered.connect(self._view.close)
        self._view.about_action.triggered.connect(self.show_about)
        self._view.pagination.page_changed.connect(self._on_page_changed)

    def show(self) -> None:
        self._view.show()

    def open_add_dialog(self) -> None:
        dialog = RecordDialog(self._view)
        if dialog.exec():
            try:
                self._record_service.add_record(dialog.get_record())
                self.refresh_records()
                self._view.show_info("Успех", "Запись успешно добавлена.")
            except ValueError as error:
                self._view.show_error("Ошибка", str(error))

    def open_search_dialog(self) -> None:
        dialog = SearchDialog(self._view)
        controller = SearchDialogController(dialog, self._search_service)
        controller.show()

    def open_delete_dialog(self) -> None:
        dialog = DeleteDialog(self._view)
        controller = DeleteDialogController(dialog, self._delete_service, self.refresh_records)
        controller.show()

    def refresh_records(self, page_number: int = 1) -> None:
        page = self._record_service.get_page(page_number, self._page_size)
        self._view.render_page(page)

    def _on_page_changed(self, page_number: int, page_size: int) -> None:
        self._page_size = page_size
        self.refresh_records(page_number)

    def clear_all_records(self) -> None:
        if not self._view.ask_clear_all_confirmation():
            return

        self._record_service.clear_all()
        self.refresh_records()
        self._view.show_info("Очистка завершена", "Все записи были удалены.")

    def export_xml(self) -> None:
        path = self._view.ask_save_xml_path()
        if not path:
            return
        self._import_export_service.export_to_xml(path)
        self._view.show_info("Экспорт завершен", f"Данные сохранены в файл:\n{path}")

    def import_xml(self) -> None:
        path = self._view.ask_open_xml_path()
        if not path:
            return
        try:
            records = self._import_export_service.import_from_xml(path)
        except Exception as error:
            self._view.show_error("Ошибка импорта", str(error))
            return
        self.refresh_records()
        self._view.show_info("Импорт завершен", f"Загружено записей: {len(records)}")

    def show_about(self) -> None:
        self._view.show_info(
            "О программе",
            "Lab2_Sem4\n"
            "Учёт питомцев и ветеринарных приемов\n"
            "MVC + SQLite + XML DOM/SAX",
        )