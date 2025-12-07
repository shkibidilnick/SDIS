#include <UnitTest++/UnitTest++.h>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Inventory/InventoryItem.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Inventory/PurchaseOrder.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Inventory/RowMaterial.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Inventory/Supplier.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Inventory/Warehouse.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Maintenance/SparePart.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FinanceException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/InventoryException.h"

// ==================== Тесты RawMaterial ====================
TEST(RawMaterial_Constructor_CreatesMaterialWithCorrectProperties) {
  RawMaterial material("Aluminum Bar", "ALU-001", "METAL", "SUP001", 500, 3.5,
                       "WH-A1", 50, 5);

  CHECK(!material.getTrackingId().empty());
  CHECK_EQUAL("METAL", material.getMaterialType());
  CHECK_EQUAL("SUP001", material.getSupplierId());
  CHECK_EQUAL(50, material.getReorderPoint());
  CHECK_EQUAL(5, material.getLeadTimeDays());
}

TEST(RawMaterial_AdjustQuantity_UpdatesQuantity) {
  RawMaterial material("Bolt", "BLT-001", "METAL", "SUP001", 50, 0.5, "WH-B2",
                       10, 3);

  material.adjustQuantity(25);
  CHECK_EQUAL(75, material.getQuantity());

  material.adjustQuantity(-15);
  CHECK_EQUAL(60, material.getQuantity());
}

TEST(RawMaterial_AdjustQuantity_ThrowsWhenInsufficientStock) {
  RawMaterial material("Nut", "NUT-001", "METAL", "SUP001", 10, 0.25, "WH-B2",
                       5, 3);

  CHECK_THROW(material.adjustQuantity(-15), InventoryException);
}

TEST(RawMaterial_GetTotalValue_ReturnsCorrectValue) {
  RawMaterial material("Washer", "WSH-001", "METAL", "SUP001", 200, 0.1,
                       "WH-C3", 20, 3);

  CHECK_CLOSE(20.0, material.getTotalValue(), 0.01);
}

TEST(RawMaterial_NeedsReorder_ReturnsCorrectStatus) {
  RawMaterial material1("Plastic Pellets", "PLS-001", "POLYMER", "SUP002", 60,
                        2.0, "WH-B2", 50, 7);
  RawMaterial material2("Plastic Pellets", "PLS-002", "POLYMER", "SUP002", 40,
                        2.0, "WH-B2", 50, 7);

  CHECK_EQUAL(false, material1.needsReorder());
  CHECK_EQUAL(true, material2.needsReorder());
}

TEST(RawMaterial_DaysUntilStockout_ReturnsCorrectDays) {
  RawMaterial material("Copper Wire", "COP-001", "METAL", "SUP003", 1000, 5.0,
                       "WH-A2", 100, 3);

  CHECK_EQUAL(50, material.daysUntilStockout(20.0));  // 1000 / 20 = 50 days
  CHECK_EQUAL(-1, material.daysUntilStockout(0.0));   // No usage
}

// ==================== Тесты SparePart ====================
TEST(SparePart_Constructor_CreatesPartWithCorrectProperties) {
  SparePart part("Bearing", "BRG-001", "BRG-6205", "SKF", 25, 45.0, "WH-M1",
                 14);

  CHECK_EQUAL("BRG-6205", part.getPartNumber());
  CHECK_EQUAL("SKF", part.getManufacturer());
  CHECK_EQUAL(14, part.getLeadTimeDays());
}

TEST(SparePart_AdjustQuantity_UpdatesQuantity) {
  SparePart part("Motor Belt", "BLT-001", "BLT-3000", "Gates", 10, 25.0,
                 "WH-M2", 7);

  part.adjustQuantity(5);
  CHECK_EQUAL(15, part.getQuantity());

  part.adjustQuantity(-3);
  CHECK_EQUAL(12, part.getQuantity());
}

TEST(SparePart_AddCompatibleMachine_AddsMachineToList) {
  SparePart part("PLC Module", "PLC-001", "PLC-S71200", "Siemens", 2, 1200.0,
                 "WH-E1", 21);

  part.addCompatibleMachine("MACH001");
  part.addCompatibleMachine("MACH002");

  CHECK_EQUAL(true, part.isCompatible("MACH001"));
  CHECK_EQUAL(true, part.isCompatible("MACH002"));
  CHECK_EQUAL(false, part.isCompatible("MACH003"));
}

TEST(SparePart_GetRecommendedStockLevel_ReturnsCorrectLevel) {
  SparePart criticalPart("PLC Module", "PLC-001", "PLC-S71200", "Siemens", 2,
                         1200.0, "WH-E1", 21);
  SparePart standardPart("Fuse", "FUS-001", "FUS-5A", "Bussmann", 20, 2.5,
                         "WH-E2", 3);

  CHECK_EQUAL(10, criticalPart.getRecommendedStockLevel());  // Critical
  CHECK_EQUAL(3, standardPart.getRecommendedStockLevel());   // Standard
}

// ==================== Тесты Warehouse ====================
TEST(Warehouse_Constructor_CreatesWarehouseWithCorrectProperties) {
  Warehouse warehouse("Main Warehouse", 10000);

  CHECK(!warehouse.getWarehouseId().empty());
  CHECK_EQUAL("Main Warehouse", warehouse.getName());
  CHECK_EQUAL(10000, warehouse.getCapacity());
  CHECK_EQUAL(0, warehouse.getCurrentOccupancy());
}

TEST(Warehouse_AddItem_AddsItemToWarehouse) {
  Warehouse warehouse("Raw Materials", 5000);
  auto item = std::make_shared<RawMaterial>(
      "Steel Coil", "STL-COIL", "METAL", "SUP001", 100, 250.0, "WH-A1", 20, 7);

  warehouse.addItem(item);

  CHECK_EQUAL(1, warehouse.getCurrentOccupancy());
  CHECK_EQUAL(item, warehouse.findItem("STL-COIL"));
}

TEST(Warehouse_RemoveItem_ReducesQuantityAndRemovesWhenZero) {
  Warehouse warehouse("Components", 2000);
  auto item1 = std::make_shared<RawMaterial>(
      "Item A", "ITM-A", "METAL", "SUP001", 100, 10.0, "WH-01", 20, 3);

  warehouse.addItem(item1);
  warehouse.removeItem("ITM-A", 30);

  CHECK_EQUAL(70, warehouse.findItem("ITM-A")->getQuantity());

  warehouse.removeItem("ITM-A", 70);
  CHECK(warehouse.findItem("ITM-A") == nullptr);
  CHECK_EQUAL(0, warehouse.getCurrentOccupancy());
}

TEST(Warehouse_GetTotalInventoryValue_ReturnsSumOfAllItems) {
  Warehouse warehouse("Mixed Goods", 3000);
  auto item1 = std::make_shared<RawMaterial>(
      "Item A", "ITM-A", "METAL", "SUP001", 100, 10.0, "WH-01", 20, 3);
  auto item2 = std::make_shared<RawMaterial>(
      "Item B", "ITM-B", "METAL", "SUP001", 50, 20.0, "WH-02", 10, 3);

  warehouse.addItem(item1);
  warehouse.addItem(item2);

  CHECK_CLOSE(2000.0, warehouse.getTotalInventoryValue(),
              0.01);  // (100*10) + (50*20)
}

TEST(Warehouse_CheckAvailability_ReturnsCorrectStatus) {
  Warehouse warehouse("Stock", 1000);
  auto item = std::make_shared<RawMaterial>("Widget", "WID-001", "METAL",
                                            "SUP001", 50, 5.0, "WH-01", 10, 3);

  warehouse.addItem(item);

  CHECK_EQUAL(true, warehouse.checkAvailability("WID-001", 30));
  CHECK_EQUAL(true, warehouse.checkAvailability("WID-001", 50));
  CHECK_EQUAL(false, warehouse.checkAvailability("WID-001", 51));
  CHECK_EQUAL(false, warehouse.checkAvailability("WID-999", 1));
}

// ==================== Тесты Supplier ====================
TEST(Supplier_Constructor_CreatesSupplierWithCorrectProperties) {
  Supplier supplier("Steel Corp", "contact@steelcorp.com", "555-0100");

  CHECK(!supplier.getTrackingId().empty());
  CHECK_EQUAL("Steel Corp", supplier.getName());
  CHECK_EQUAL(85.0, supplier.getReliabilityScore());
  CHECK_EQUAL(false, supplier.getIsApproved());
}

TEST(Supplier_AddMaterial_AddsMaterialToCatalog) {
  Supplier supplier("Fasteners Inc", "orders@fasteners.com", "555-0200");

  supplier.addMaterial("Bolts");
  supplier.addMaterial("Nuts");
  supplier.addMaterial("Washers");

  CHECK_EQUAL(true, supplier.suppliesMaterial("Bolts"));
  CHECK_EQUAL(true, supplier.suppliesMaterial("Nuts"));
  CHECK_EQUAL(false, supplier.suppliesMaterial("Screws"));
}

TEST(Supplier_UpdateReliabilityScore_AdjustsScoreBasedOnPerformance) {
  Supplier supplier("Chemical Supply", "sales@chemsupply.com", "555-0300");

  supplier.updateReliabilityScore(true);  // On time
  CHECK_CLOSE(86.0, supplier.getReliabilityScore(), 0.01);

  supplier.updateReliabilityScore(false);  // Late
  CHECK_CLOSE(81.0, supplier.getReliabilityScore(), 0.01);
}

TEST(Supplier_GetAverageLeadTime_ReturnsDaysBasedOnReliability) {
  Supplier supplier("Electronic Components", "info@eleccomp.com", "555-0400");

  // При 85% надежности: 14 - (0.85 * 7) = 14 - 5.95 = 8.05 → 8 дней
  CHECK_EQUAL(8, supplier.getAverageLeadTime());
}

// ==================== Тесты PurchaseOrder ====================
TEST(PurchaseOrder_Constructor_CreatesPOWithCorrectProperties) {
  PurchaseOrder po("SUP001", "2025-02-15");

  CHECK(!po.getTrackingId().empty());
  CHECK_EQUAL("SUP001", po.getSupplierId());
  CHECK_EQUAL("DRAFT", po.getStatus());
  CHECK_EQUAL(0, po.getTotalQuantity());
}

TEST(PurchaseOrder_AddLineItem_AddsItemAndUpdatesTotals) {
  PurchaseOrder po("SUP001", "2025-02-15");
  auto material = std::make_shared<RawMaterial>(
      "Steel Bar", "STL-BAR", "METAL", "SUP001", 0, 50.0, "WH-A1", 50, 5);

  po.addLineItem(material, 100);

  CHECK_EQUAL(100, po.getTotalQuantity());
}

TEST(PurchaseOrder_CalculateTotals_ComputesTaxAndTotal) {
  PurchaseOrder po("SUP001", "2025-02-15");
  auto material = std::make_shared<RawMaterial>(
      "Steel Plate", "STL-PLT", "METAL", "SUP001", 0, 100.0, "WH-A1", 20, 5);

  po.addLineItem(material, 50);  // 50 * 100 = 5000
  po.calculateTotals(7.5);       // 7.5% tax

  CHECK_CLOSE(5375.0, po.getTotalAmount(), 0.01);  // 5000 + 375 tax
}

TEST(PurchaseOrder_Approve_ChangesStatusToSent) {
  PurchaseOrder po("SUP001", "2025-02-15");
  auto material = std::make_shared<RawMaterial>(
      "Aluminum", "ALU-001", "METAL", "SUP001", 0, 200.0, "WH-A1", 30, 5);

  po.addLineItem(material, 10);
  po.approve();

  CHECK_EQUAL("SENT", po.getStatus());
  CHECK_EQUAL(true, po.getIsApproved());
}

TEST(PurchaseOrder_MarkConfirmed_ChangesStatusToConfirmed) {
  PurchaseOrder po("SUP001", "2025-02-15");
  auto material = std::make_shared<RawMaterial>(
      "Copper", "COP-001", "METAL", "SUP001", 0, 150.0, "WH-A1", 20, 3);

  po.addLineItem(material, 20);
  po.approve();
  po.markConfirmed();

  CHECK_EQUAL("CONFIRMED", po.getStatus());
}

TEST(PurchaseOrder_MarkCompleted_ChangesStatusToCompleted) {
  PurchaseOrder po("SUP001", "2025-02-15");
  auto material = std::make_shared<RawMaterial>(
      "Plastic", "PLS-001", "POLYMER", "SUP001", 0, 10.0, "WH-B1", 100, 7);

  po.addLineItem(material, 500);
  po.approve();
  po.markConfirmed();
  po.recordPartialReceipt("PLS-001", 200);
  po.recordPartialReceipt("PLS-001", 300);
  po.markCompleted();

  CHECK_EQUAL("COMPLETED", po.getStatus());
}

TEST(PurchaseOrder_Cancel_ChangesStatusToCancelled) {
  PurchaseOrder po("SUP001", "2025-02-15");

  po.cancel();

  CHECK_EQUAL("CANCELLED", po.getStatus());
}

TEST(PurchaseOrder_Validate_ThrowsWhenMissingRequiredFields) {
  PurchaseOrder po("SUP001", "2025-02-15");

  // Пустой PO без line items
  CHECK_THROW(po.validate(), FinanceException);
}
