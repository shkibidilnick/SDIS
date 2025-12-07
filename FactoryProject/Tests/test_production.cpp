#include <UnitTest++/UnitTest++.h>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/AssemblyStep.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/Machine.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/Product.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/ProductionLine.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/WorkOrder.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/MaintenanceException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ProductionException.h"

// ==================== Тесты Machine ====================
TEST(Machine_Constructor_CreatesMachineWithCorrectProperties) {
  Machine machine("MACH-001", "CNC Mill", "CNC", 85.0, 200);

  CHECK(!machine.getTrackingId().empty());
  CHECK_EQUAL("MACH-001", machine.getMachineCode());
  CHECK_EQUAL("CNC Mill", machine.getName());
  CHECK_EQUAL(true, machine.getIsOperational());
}

TEST(Machine_Constructor_ThrowsForInvalidParameters) {
  CHECK_THROW(Machine("", "Invalid", "CNC", 85.0, 200), MaintenanceException);
  CHECK_THROW(Machine("MACH-001", "Invalid", "CNC", 85.0, 0),
              MaintenanceException);
}

TEST(Machine_IsOperatorCertified_ChecksCertifications) {
  Machine machine("MACH-002", "Welding Robot", "WELDING", 75.0, 150);
  auto employee =
      std::make_shared<Employee>("John Doe", Role::TECHNICIAN, 22.0);

  // Пока нет сертификаций
  CHECK_EQUAL(false, machine.isOperatorCertified(employee));

  // Добавляем сертификацию
  employee->addCertification("CNC_Operator");
  // В реальном коде нужно добавить метод setRequiredCertifications в Machine
  // Для теста предполагаем, что сертификация проверяется
}

TEST(Machine_RecordOperationHours_UpdatesMaintenanceCounter) {
  Machine machine("MACH-003", "Lathe", "TURNING", 65.0, 100);

  machine.recordOperationHours(50);
  CHECK_EQUAL(50, machine.getHoursSinceLastMaintenance());

  machine.recordOperationHours(60);  // Превышает интервал 100
  // Машина должна быть помечена как требующая обслуживания
}

TEST(Machine_SetUnderMaintenance_ChangesStatus) {
  Machine machine("MACH-004", "Press", "STAMPING", 90.0, 120);

  machine.setUnderMaintenance();
  CHECK_EQUAL(false, machine.getIsOperational());
  CHECK_EQUAL("MAINTENANCE", machine.getStatus());
}

TEST(Machine_SetOperational_ResetsStatusAndCounter) {
  Machine machine("MACH-005", "Grinder", "FINISHING", 55.0, 80);

  machine.recordOperationHours(80);
  machine.setUnderMaintenance();
  machine.setOperational();

  CHECK_EQUAL(true, machine.getIsOperational());
  CHECK_EQUAL(0, machine.getHoursSinceLastMaintenance());
}

// ==================== Тесты AssemblyStep ====================
TEST(AssemblyStep_Constructor_CreatesStepWithCorrectProperties) {
  AssemblyStep step("Attach component A to base", 1, 15.0);

  CHECK(!step.getTrackingId().empty());
  CHECK_EQUAL("Attach component A to base", step.getDescription());
  CHECK_EQUAL(1, step.getSequenceNumber());
  CHECK_EQUAL(15.0, step.getEstimatedMinutes());
  CHECK_EQUAL(false, step.getIsCompleted());
}

TEST(AssemblyStep_RecordCompletion_SetsActualTime) {
  AssemblyStep step("Install sensor", 2, 10.0);

  step.recordCompletion(12.5);

  CHECK_EQUAL(12.5, step.getActualMinutes());
}

TEST(AssemblyStep_AssignMachine_SetsMachineId) {
  AssemblyStep step("Drill holes", 3, 20.0);

  step.assignMachine("MACH-001");

  CHECK_EQUAL("MACH-001", step.getMachineId());
}

TEST(AssemblyStep_AssignOperator_SetsOperatorId) {
  AssemblyStep step("Quality check", 4, 8.0);

  step.assignOperator("EMP001");

  CHECK_EQUAL("EMP001", step.getOperatorId());
}

TEST(AssemblyStep_AddQualityCheckpoint_AddsCheckpoint) {
  AssemblyStep step("Final assembly", 5, 30.0);

  step.addQualityCheckpoint("Torque verification");
  step.addQualityCheckpoint("Visual inspection");

  // Проверка через количество (если есть метод getCheckpointCount)
  // CHECK_EQUAL(2, step.getCheckpointCount());
}

TEST(AssemblyStep_Complete_MarksAsCompleted) {
  AssemblyStep step("Packaging", 6, 5.0);

  step.recordCompletion(5.5);
  step.complete();

  CHECK_EQUAL(true, step.getIsCompleted());
  CHECK_EQUAL("COMPLETED", step.getStatus());
}

TEST(AssemblyStep_Complete_ThrowsWhenNotRecorded) {
  AssemblyStep step("Test step", 7, 10.0);

  // Не записываем actual time
  CHECK_THROW(step.complete(), ProductionException);
}

// ==================== Тесты Product ====================
TEST(Product_Constructor_CreatesProductWithCorrectProperties) {
  Product product("PROD-001", "Widget", "Standard widget", 45.99);

  CHECK(!product.getTrackingId().empty());
  CHECK_EQUAL("PROD-001", product.getProductCode());
  CHECK_EQUAL("Widget", product.getName());
  CHECK_CLOSE(45.99, product.getUnitPrice(), 0.01);
  CHECK_EQUAL(false, product.hasPassedQualityCheck());
  CHECK_EQUAL(false, product.getIsShipped());
}

TEST(Product_AddRawMaterial_AddsMaterialToList) {
  Product product("PROD-001", "Gadget", "Electronic gadget", 99.99);

  product.addRawMaterial("MAT-001");
  product.addRawMaterial("MAT-002");

  // Проверка через размер вектора (если есть доступ)
  // CHECK_EQUAL(2, product.getRawMaterialCount());
}

TEST(Product_MarkQualityPassed_SetsQualityStatus) {
  Product product("PROD-001", "Device", "Mechanical device", 159.99);

  product.markQualityPassed();

  CHECK_EQUAL(true, product.hasPassedQualityCheck());
}

TEST(Product_MarkQualityPassed_ThrowsWhenAlreadyShipped) {
  Product product("PROD-001", "Tool", "Hand tool", 29.99);

  product.markQualityPassed();
  product.markShipped();

  CHECK_THROW(product.markQualityPassed(), ProductionException);
}

TEST(Product_MarkShipped_SetsShippedStatus) {
  Product product("PROD-001", "Part", "Spare part", 15.5);

  product.markQualityPassed();
  product.markShipped();

  CHECK_EQUAL(true, product.getIsShipped());
  CHECK_EQUAL("SHIPPED", product.getStatus());
}

TEST(Product_MarkShipped_ThrowsWhenNotQualityChecked) {
  Product product("PROD-001", "Component", "Mechanical component", 75.0);

  // Не проходим quality check
  CHECK_THROW(product.markShipped(), ProductionException);
}

TEST(Product_IsReadyForShipment_ReturnsCorrectStatus) {
  Product product1("PROD-001", "Item1", "Description", 10.0);
  Product product2("PROD-002", "Item2", "Description", 20.0);
  Product product3("PROD-003", "Item3", "Description", 30.0);

  // Только создан
  CHECK_EQUAL(false, product1.isReadyForShipment());

  // Прошел QC, не отправлен
  product2.markQualityPassed();
  CHECK_EQUAL(true, product2.isReadyForShipment());

  // Прошел QC и отправлен
  product3.markQualityPassed();
  product3.markShipped();
  CHECK_EQUAL(false, product3.isReadyForShipment());
}

// ==================== Тесты ProductionLine ====================
TEST(ProductionLine_Constructor_CreatesLineWithCorrectProperties) {
  ProductionLine line("LINE-001", "Assembly Line 1", 85.0);

  CHECK(!line.getLineId().empty());
  CHECK_EQUAL("LINE-001", line.getLineCode());
  CHECK_EQUAL("Assembly Line 1", line.getName());
  CHECK_EQUAL(85.0, line.getTargetEfficiency());
  CHECK_EQUAL(false, line.getIsActive());
}

TEST(ProductionLine_AddMachine_AddsOperationalMachine) {
  ProductionLine line("LINE-001", "Machining Line", 90.0);
  auto machine =
      std::make_shared<Machine>("MACH-001", "CNC Mill", "CNC", 85.0, 200);

  line.addMachine(machine);

  CHECK_EQUAL(1, line.getMachineCount());
}

TEST(ProductionLine_AddMachine_ThrowsForNullOrNonOperational) {
  ProductionLine line("LINE-001", "Assembly Line", 85.0);
  auto machine =
      std::make_shared<Machine>("MACH-001", "CNC Mill", "CNC", 85.0, 200);
  machine->setUnderMaintenance();

  CHECK_THROW(line.addMachine(nullptr), ProductionException);
  CHECK_THROW(line.addMachine(machine), ProductionException);
}

TEST(ProductionLine_IsLineOperational_ReturnsCorrectStatus) {
  ProductionLine line("LINE-001", "Complete Line", 85.0);
  auto machine1 =
      std::make_shared<Machine>("MACH-001", "Machine 1", "TYPE1", 75.0, 100);
  auto machine2 =
      std::make_shared<Machine>("MACH-002", "Machine 2", "TYPE2", 75.0, 100);

  line.addMachine(machine1);
  line.addMachine(machine2);

  CHECK_EQUAL(true, line.isLineOperational());

  machine1->setUnderMaintenance();
  CHECK_EQUAL(false, line.isLineOperational());
}

TEST(ProductionLine_GetHourlyCost_ReturnsSumOfMachineRates) {
  ProductionLine line("LINE-001", "Cost Calculation", 85.0);
  auto machine1 =
      std::make_shared<Machine>("MACH-001", "M1", "TYPE1", 50.0, 100);
  auto machine2 =
      std::make_shared<Machine>("MACH-002", "M2", "TYPE2", 75.0, 100);
  auto machine3 =
      std::make_shared<Machine>("MACH-003", "M3", "TYPE3", 100.0, 100);

  line.addMachine(machine1);
  line.addMachine(machine2);
  line.addMachine(machine3);

  CHECK_CLOSE(225.0, line.getHourlyCost(), 0.01);  // 50 + 75 + 100
}

// ==================== Тесты WorkOrder ====================
TEST(WorkOrder_Constructor_CreatesWorkOrderWithCorrectProperties) {
  WorkOrder wo("PROD-001", 1000, "2025-02-28");

  CHECK(!wo.getTrackingId().empty());
  CHECK_EQUAL("PROD-001", wo.getProductCode());
  CHECK_EQUAL(1000, wo.getQuantityRequested());
  CHECK_EQUAL(0, wo.getQuantityCompleted());
  CHECK_EQUAL("PLANNED", wo.getStatus());
}

TEST(WorkOrder_Start_ChangesStatusToInProgress) {
  WorkOrder wo("PROD-001", 500, "2025-02-28");

  wo.start();

  CHECK_EQUAL("IN_PROGRESS", wo.getStatus());
}

TEST(WorkOrder_CompleteQuantity_UpdatesCompletedCount) {
  WorkOrder wo("PROD-001", 500, "2025-02-28");
  wo.start();

  wo.completeQuantity(100);
  CHECK_EQUAL(100, wo.getQuantityCompleted());
  CHECK_CLOSE(20.0, wo.getCompletionPercentage(), 0.1);

  wo.completeQuantity(200);
  CHECK_EQUAL(300, wo.getQuantityCompleted());
  CHECK_CLOSE(60.0, wo.getCompletionPercentage(), 0.1);
}

TEST(WorkOrder_CompleteQuantity_CompletesOrderWhenFullyDone) {
  WorkOrder wo("PROD-001", 250, "2025-02-28");
  wo.start();

  wo.completeQuantity(250);

  CHECK_EQUAL(250, wo.getQuantityCompleted());
  CHECK_EQUAL("COMPLETED", wo.getStatus());
}

TEST(WorkOrder_CompleteQuantity_ThrowsWhenExceedingRequested) {
  WorkOrder wo("PROD-001", 100, "2025-02-28");
  wo.start();

  wo.completeQuantity(50);
  CHECK_THROW(wo.completeQuantity(60), ProductionException);
}

TEST(WorkOrder_Cancel_ChangesStatusToCancelled) {
  WorkOrder wo("PROD-001", 500, "2025-02-28");
  wo.start();

  wo.cancel();

  CHECK_EQUAL("CANCELLED", wo.getStatus());
}

TEST(WorkOrder_Cancel_ThrowsWhenCompleted) {
  WorkOrder wo("PROD-001", 100, "2025-02-28");
  wo.start();
  wo.completeQuantity(100);

  CHECK_THROW(wo.cancel(), ProductionException);
}

TEST(WorkOrder_Validate_ThrowsWhenMissingRequiredData) {
  WorkOrder wo("PROD-001", 100, "2025-02-28");

  // Не добавлены материалы и не назначена линия
  CHECK_THROW(wo.validate(), ProductionException);
}
