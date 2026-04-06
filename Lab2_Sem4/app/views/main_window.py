from __future__ import annotations

from PySide6.QtGui import QAction
from PySide6.QtWidgets import (
    QFileDialog,
    QHeaderView,
    QMainWindow,
    QMessageBox,
    QSplitter,
    QStatusBar,
    QTableWidget,
    QTableWidgetItem,
    QToolBar,
    QTreeWidget,
    QTreeWidgetItem,
    QVBoxLayout,
    QWidget,
)

from app.domain.pagination import Page
from app.views.pagination_widget import PaginationWidget


class MainWindow(QMainWindow):
    def __init__(self) -> None:
        super().__init__()
        self.setWindowTitle("Lab2_Sem4 — Учёт питомцев")
        self.resize(1200, 700)

        self.table = QTableWidget(0, 5)
        self.table.setHorizontalHeaderLabels(
            ["Имя питомца", "Дата рождения", "Дата приема", "ФИО ветеринара", "Диагноз"]
        )

        header = self.table.horizontalHeader()
        header.setSectionResizeMode(0, QHeaderView.ResizeToContents)
        header.setSectionResizeMode(1, QHeaderView.ResizeToContents)
        header.setSectionResizeMode(2, QHeaderView.ResizeToContents)
        header.setSectionResizeMode(3, QHeaderView.Interactive)
        header.setSectionResizeMode(4, QHeaderView.Stretch)
        self.table.setColumnWidth(3, 220)

        self.tree = QTreeWidget()
        self.tree.setHeaderLabels(["Поле", "Значение"])

        splitter = QSplitter()
        splitter.addWidget(self.table)
        splitter.addWidget(self.tree)
        splitter.setSizes([700, 500])

        self.pagination = PaginationWidget()

        central = QWidget()
        central_layout = QVBoxLayout(central)
        central_layout.addWidget(splitter)
        central_layout.addWidget(self.pagination)
        self.setCentralWidget(central)

        self.statusbar = QStatusBar()
        self.setStatusBar(self.statusbar)

        self._create_actions()
        self._create_menu()
        self._create_toolbar()

    def _create_actions(self) -> None:
        self.add_action = QAction("Добавить", self)
        self.search_action = QAction("Поиск", self)
        self.delete_action = QAction("Удалить", self)
        self.refresh_action = QAction("Обновить", self)
        self.clear_all_action = QAction("Очистить все записи", self)
        self.export_xml_action = QAction("Экспорт XML", self)
        self.import_xml_action = QAction("Импорт XML", self)
        self.exit_action = QAction("Выход", self)
        self.about_action = QAction("О программе", self)

    def _create_menu(self) -> None:
        file_menu = self.menuBar().addMenu("Файл")
        file_menu.addAction(self.import_xml_action)
        file_menu.addAction(self.export_xml_action)
        file_menu.addSeparator()
        file_menu.addAction(self.exit_action)

        records_menu = self.menuBar().addMenu("Записи")
        records_menu.addAction(self.add_action)
        records_menu.addAction(self.search_action)
        records_menu.addAction(self.delete_action)
        records_menu.addAction(self.refresh_action)
        records_menu.addSeparator()
        records_menu.addAction(self.clear_all_action)

        help_menu = self.menuBar().addMenu("Справка")
        help_menu.addAction(self.about_action)

    def _create_toolbar(self) -> None:
        toolbar = QToolBar("Основные команды")
        self.addToolBar(toolbar)
        for action in (
            self.add_action,
            self.search_action,
            self.delete_action,
            self.refresh_action,
            self.clear_all_action,
            self.import_xml_action,
            self.export_xml_action,
        ):
            toolbar.addAction(action)

    def render_page(self, page: Page) -> None:
        self.table.setRowCount(len(page.items))
        self.tree.clear()

        for row_index, record in enumerate(page.items):
            values = [
                record.pet_name,
                record.birth_date.strftime("%d.%m.%Y"),
                record.last_visit_date.strftime("%d.%m.%Y"),
                record.veterinarian.full_name(),
                record.diagnosis,
            ]
            for column_index, value in enumerate(values):
                self.table.setItem(row_index, column_index, QTableWidgetItem(value))

            record_root = QTreeWidgetItem(self.tree, [f"Запись #{row_index + 1}", ""])
            QTreeWidgetItem(record_root, ["Имя питомца", record.pet_name])
            QTreeWidgetItem(record_root, ["Дата рождения", record.birth_date.strftime("%d.%m.%Y")])
            QTreeWidgetItem(record_root, ["Дата приема", record.last_visit_date.strftime("%d.%m.%Y")])
            QTreeWidgetItem(record_root, ["ФИО ветеринара", record.veterinarian.full_name()])
            QTreeWidgetItem(record_root, ["Диагноз", record.diagnosis])

        self.tree.expandAll()
        self.pagination.update_state(page.page_number, page.total_pages, page.total_items, page.page_size)
        self.statusbar.showMessage(f"Показано {len(page.items)} записей из {page.total_items}")

    def ask_save_xml_path(self) -> str:
        path, _ = QFileDialog.getSaveFileName(self, "Сохранить XML", filter="XML Files (*.xml)")
        return path

    def ask_open_xml_path(self) -> str:
        path, _ = QFileDialog.getOpenFileName(self, "Загрузить XML", filter="XML Files (*.xml)")
        return path

    def ask_clear_all_confirmation(self) -> bool:
        result = QMessageBox.question(
            self,
            "Подтверждение очистки",
            "Удалить все записи из базы данных?",
            QMessageBox.Yes | QMessageBox.No,
            QMessageBox.No,
        )
        return result == QMessageBox.Yes

    def show_info(self, title: str, message: str) -> None:
        QMessageBox.information(self, title, message)

    def show_error(self, title: str, message: str) -> None:
        QMessageBox.critical(self, title, message)