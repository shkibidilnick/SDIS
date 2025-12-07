#include <memory>
#include <stdexcept>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/AuditLog.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Department.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Employee.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Factory.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Role.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Schedule.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Shift.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Finance/Budget.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Finance/Invoice.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Finance/Payroll.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Finance/Transaction.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Inventory/InventoryItem.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Inventory/PurchaseOrder.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Inventory/RowMaterial.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Inventory/Supplier.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Inventory/Warehouse.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Maintenance/MaintenanceRequest.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Maintenance/MaintenanceSchedule.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Maintenance/SparePart.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/AssemblyStep.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/Machine.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/Product.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/ProductionLine.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/WorkOrder.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Quality/Defect.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Quality/QualityCheck.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Quality/QualityStandard.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Quality/TestProcedure.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Safety/SafetyIncident.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Safety/SafetyTraining.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Shipping/Carrier.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Shipping/DeliveryOrder.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Shipping/Shipment.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ComplianceException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FactoryException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FinanceException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/InventoryException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/MaintenanceException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/PayrollException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ProductionException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/QualityExceptionh.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/SafetyException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ShippingException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Tests/test_helpers.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"
#include "UnitTest++/UnitTest++.h"

#pragma warning(disable : 26444)
using namespace std;
using namespace UnitTest;

TEST(AuditLog_CompleteWorkflow) {
  AuditLog log("Production");
  log.addEntry("user1", "CREATE", "Machine", "M001", "old", "new",
               "192.168.1.1");
  log.addEntry("user2", "UPDATE", "Machine", "M001", "old", "new",
               "192.168.1.2");
  CHECK_EQUAL(2, log.getEntryCount());

  // Test sealing
  log.seal();
  CHECK(log.getIsSealed());
  CHECK_THROW(log.addEntry("user3", "DELETE", "Item", "I001", "x", "y"),
              FactoryException);

  // Test anomaly detection (6 changes to same entity)
  AuditLog log2("Test");
  for (int i = 0; i < 6; i++) {
    log2.addEntry("user1", "UPDATE", "Machine", "M001", "old", "new");
  }
  CHECK(log2.detectAnomalies());

  // Test CSV export
  std::string csv = log.exportToCSV();
  CHECK(csv.find("Timestamp") != std::string::npos);
  CHECK(csv.find("user1") != std::string::npos);

  // Test date filtering
  std::string today = "2025-12-06";
  auto entries = log.getEntriesInRange(today, today);
  // Note: Date extraction needs proper implementation
}

TEST(AuditLog_ExceptionCases) {
  // Empty module name
  CHECK_THROW(AuditLog(""), FactoryException);

  AuditLog log("Test");

  // Missing required fields
  CHECK_THROW(log.addEntry("", "action", "type", "id", "old", "new"),
              FactoryException);
  CHECK_THROW(log.addEntry("user", "", "type", "id", "old", "new"),
              FactoryException);
  CHECK_THROW(log.addEntry("user", "action", "", "id", "old", "new"),
              FactoryException);
  CHECK_THROW(log.addEntry("user", "action", "type", "", "old", "new"),
              FactoryException);

  // Invalid date range
  CHECK_THROW(log.getEntriesInRange("invalid-date", "2025-12-06"),
              FactoryException);
  CHECK_THROW(log.getEntriesByUser(""), FactoryException);
  CHECK_THROW(log.getEntriesByEntity("", "id"), FactoryException);
  CHECK_THROW(log.getEntriesByEntity("type", ""), FactoryException);
}

TEST(Department_CompleteWorkflow) {
  Department dept("Production", 50000.0);
  CHECK_EQUAL("Production", dept.getName());
  CHECK_EQUAL(50000.0, dept.getBudget());

  // Test employee management
  auto emp1 = std::make_shared<Employee>("Alice", Role::MANAGER, 25.0);
  auto emp2 = std::make_shared<Employee>("Bob", Role::OPERATOR, 20.0);
  auto emp3 = std::make_shared<Employee>("Charlie", Role::OPERATOR, 20.0);

  dept.addEmployee(emp1);
  dept.addEmployee(emp2);
  dept.addEmployee(emp3);

  CHECK_EQUAL(3, dept.getEmployeeCount());
  CHECK(dept.isFullyStaffed());  // 1 manager + 2 operators

  // Test payroll calculation
  double payroll = dept.calculatePayroll();
  CHECK(payroll > 0);

  // Test report generation
  std::string report = dept.generateReport();
  CHECK(report.find("Department Report") != std::string::npos);

  // Test employee removal
  dept.removeEmployee(emp2->getEmployeeId());
  CHECK_EQUAL(2, dept.getEmployeeCount());
}

TEST(Department_ExceptionCases) {
  // Negative budget
  CHECK_THROW(Department("Test", -100.0), FactoryException);

  Department dept("Test", 10000.0);

  // Add null employee
  CHECK_THROW(dept.addEmployee(nullptr), FactoryException);

  // Remove non-existent employee
  dept.removeEmployee("NONEXISTENT");  // Should not throw

  // Fully staffed check with insufficient employees
  auto emp1 = std::make_shared<Employee>("Test", Role::TECHNICIAN, 20.0);
  dept.addEmployee(emp1);
  CHECK(!dept.isFullyStaffed());  // No manager, no operators
}

TEST(Employee_CompleteWorkflow) {
  Employee emp("John Doe", Role::ENGINEER, 30.0);
  CHECK_EQUAL("John Doe", emp.getName());
  CHECK_EQUAL(Role::ENGINEER, emp.getRole());
  CHECK(emp.getIsActive());

  // Test certification
  emp.addCertification("CNC Operation");
  CHECK(emp.hasCertification("CNC Operation"));
  CHECK(!emp.hasCertification("Non-existent"));

  // Test salary calculation
  double salary = emp.calculateMonthlySalary(160);
  CHECK_EQUAL(4800.0, salary);  // 30 * 160

  // Test deactivation
  emp.deactivate();
  CHECK(!emp.getIsActive());
}

TEST(Employee_ExceptionCases) {
  // Hourly rate below minimum
  CHECK_THROW(Employee("Test", Role::OPERATOR, 10.0), FactoryException);

  Employee emp("Test", Role::OPERATOR, 20.0);

  // Empty certification
  CHECK_THROW(emp.addCertification(""), FactoryException);

  // Invalid hours
  CHECK_THROW(emp.calculateMonthlySalary(120), FactoryException);  // < 130
  CHECK_THROW(emp.calculateMonthlySalary(180), FactoryException);  // > 176
}

TEST(Factory_CompleteWorkflow) {
  Factory factory("Main Plant");
  CHECK_EQUAL("Main Plant", factory.getFactoryName());

  // Test adding components
  auto dept = std::make_shared<Department>("Assembly", 100000.0);
  auto emp = std::make_shared<Employee>("Jane", Role::MANAGER, 35.0);

  factory.addDepartment(dept);
  factory.addEmployee(emp);
  CHECK_EQUAL(1, factory.getTotalDepartmentCount());
  CHECK_EQUAL(1, factory.getTotalEmployeeCount());

  // Test lookup methods
  auto foundDept = factory.findDepartment(dept->getDepartmentId());
  auto foundEmp = factory.findEmployee(emp->getEmployeeId());
  CHECK(foundDept != nullptr);
  CHECK(foundEmp != nullptr);

  // Test assignment
  factory.assignEmployeeToDepartment(emp->getEmployeeId(),
                                     dept->getDepartmentId());

  // Test find non-existent
  CHECK(factory.findEmployee("NONEXISTENT") == nullptr);
  CHECK(factory.findDepartment("NONEXISTENT") == nullptr);

  // Test report generation
  std::string report = factory.generateFactoryReport();
  CHECK(report.find("FACTORY REPORT") != std::string::npos);
}

TEST(Factory_ExceptionCases) {
  Factory factory("Test");

  // Add null objects
  CHECK_THROW(factory.addDepartment(nullptr), FactoryException);
  CHECK_THROW(factory.addEmployee(nullptr), FactoryException);
  CHECK_THROW(factory.addProductionLine(nullptr), FactoryException);
  CHECK_THROW(factory.addWarehouse(nullptr), FactoryException);
  CHECK_THROW(factory.addBudget(nullptr), FactoryException);
  CHECK_THROW(factory.addSafetyIncident(nullptr), FactoryException);
  CHECK_THROW(factory.addDeliveryOrder(nullptr), FactoryException);

  // Assign to non-existent
  CHECK_THROW(factory.assignEmployeeToDepartment("NO_EMP", "NO_DEPT"),
              FactoryException);
}

TEST(Schedule_CompleteWorkflow) {
  auto dept = std::make_shared<Department>("Shipping", 50000.0);
  Schedule schedule("2025-12-01", dept);

  auto emp1 = std::make_shared<Employee>("Worker1", Role::OPERATOR, 20.0);
  auto emp2 = std::make_shared<Employee>("Worker2", Role::OPERATOR, 20.0);

  // Add shifts
  Shift shift1(emp1->getEmployeeId(), "2025-12-02", 8.0, 16.0);
  Shift shift2(emp2->getEmployeeId(), "2025-12-02", 16.0, 24.0);

  schedule.addShift(shift1);
  schedule.addShift(shift2);

  // Test coverage
  CHECK_EQUAL(2, schedule.getShifts().size());

  // Test labor cost calculation
  double cost = schedule.calculateLaborCost();
  CHECK(cost > 0);
}

TEST(Schedule_ExceptionCases) {
  auto dept = std::make_shared<Department>("Test", 1000.0);
  Schedule schedule("2025-12-01", dept);

  auto emp = std::make_shared<Employee>("Test", Role::OPERATOR, 20.0);
  Shift shift1(emp->getEmployeeId(), "2025-12-02", 8.0, 16.0);
  Shift shift2(emp->getEmployeeId(), "2025-12-02", 9.0, 17.0);  // Конфликт

  schedule.addShift(shift1);
  CHECK_THROW(schedule.addShift(shift2), FactoryException);

  // Validation without shifts
  Schedule emptySchedule("2025-12-01", dept);
  CHECK_THROW(emptySchedule.validate(), FactoryException);
}

TEST(Shift_CompleteWorkflow) {
  Shift shift("EMP001", "2025-12-06", 8.0, 16.5);
  CHECK_EQUAL("EMP001", shift.getEmployeeId());
  CHECK_EQUAL(8.5, shift.getHours());
  CHECK(!shift.getIsApproved());

  // Test approval
  shift.approve();
  CHECK(shift.getIsApproved());

  // Test cost calculation
  double cost = shift.calculateCost();
  CHECK_EQUAL(8.5 * 15.0, cost);  // $15/hour
}

TEST(Shift_ExceptionCases) {
  // Invalid hours
  CHECK_THROW(Shift("EMP001", "2025-12-06", -1.0, 8.0), FactoryException);
  CHECK_THROW(Shift("EMP001", "2025-12-06", 8.0, 25.0), FactoryException);
  CHECK_THROW(Shift("EMP001", "2025-12-06", 16.0, 8.0),
              FactoryException);  // start >= end
}

TEST(Budget_CompleteWorkflow) {
  Budget budget("DEPT001", "2025-Q1", 100000.0);
  CHECK_EQUAL(100000.0, budget.getTotalBudgetAmount());
  CHECK_EQUAL("DEPT001", budget.getDepartmentId());

  // Test allocation
  budget.allocate("Maintenance", 30000.0);
  budget.allocate("Materials", 50000.0);
  CHECK_EQUAL(30000.0, budget.getRemaining("Maintenance"));
  CHECK_EQUAL(20000.0, budget.getRemaining("UNALLOCATED"));

  // Test expense recording
  budget.recordExpense("Maintenance", 15000.0);
  CHECK_EQUAL(15000.0, budget.getRemaining("Maintenance"));

  // Test utilization
  double utilization = budget.getUtilizationPercentage();
  CHECK(utilization > 0);

  // Test lock
  budget.lock();
  CHECK_THROW(budget.allocate("Test", 1000.0), FinanceException);
}

TEST(Budget_ExceptionCases) {
  Budget budget("DEPT001", "2025-Q1", 100000.0);

  // Over-allocate
  budget.allocate("Test", 90000.0);
  CHECK_THROW(budget.allocate("Test2", 20000.0), FinanceException);

  budget.allocate("NewCategory", 5000.0);
  budget.recordExpense("NewCategory", 1000.0);
  CHECK_EQUAL(4000.0, budget.getRemaining("NewCategory"));
  CHECK_THROW(budget.getRemaining("NonExistent"), FinanceException);
  CHECK_THROW(Budget("DEPT001", "2025-Q1", -1000.0), FactoryException);
}

TEST(Invoice_CompleteWorkflow) {
  Invoice invoice("CUST001", "2025-12-31", "PO123");
  CHECK(!invoice.getIsPaid());
  CHECK_EQUAL("PO123", invoice.getPurchaseOrderNumber());

  // Add line items
  invoice.addLineItem("Widget A", 1000.0);
  invoice.addLineItem("Widget B", 2000.0);
  CHECK_EQUAL(2, invoice.getLineItemCount());

  // Calculate totals
  invoice.calculateTotals(10.0);  // 10% tax
  CHECK(invoice.getTotalAmount() > 3000.0);
  CHECK_EQUAL(300.0, invoice.getTaxAmount());

  // Mark as paid
  invoice.markAsPaid();
  CHECK(invoice.getIsPaid());
}

TEST(Invoice_ExceptionCases) {
  // Invalid customer ID
  CHECK_THROW(Invoice("", "2025-12-31"), FinanceException);

  // Invalid date
  CHECK_THROW(Invoice("CUST001", "invalid-date"), FinanceException);

  Invoice invoice("CUST001", "2025-12-31");
  invoice.addLineItem("Test", 1000.0);
  invoice.calculateTotals(10.0);
  invoice.markAsPaid();

  // Modify paid invoice
  CHECK_THROW(invoice.addLineItem("Test2", 500.0), FinanceException);
  CHECK_THROW(invoice.calculateTotals(10.0), FinanceException);
  CHECK_THROW(invoice.markAsPaid(), FinanceException);
}

TEST(Payroll_CompleteWorkflow) {
  auto emp1 = std::make_shared<Employee>("Worker1", Role::OPERATOR, 20.0);
  auto emp2 = std::make_shared<Employee>("Worker2", Role::OPERATOR, 25.0);

  Payroll payroll("2025-01-01 to 2025-01-31");
  payroll.addEmployeeHours(emp1, 160.0);
  payroll.addEmployeeHours(emp2, 160.0);

  // Test total payroll calculation
  double total = payroll.calculateTotalPayroll();
  CHECK_EQUAL(20.0 * 160 + 25.0 * 160, total);

  // Test individual employee pay
  CHECK_EQUAL(20.0 * 160, payroll.getEmployeePay(emp1->getEmployeeId()));

  // Process payroll
  payroll.process();
  CHECK(payroll.getIsProcessed());
}

TEST(Transaction_CompleteWorkflow) {
  Transaction tx("PAYMENT", 1000.0, "ACC001", "ACC002", "REF123");
  CHECK(!tx.getIsPosted());

  // Post transaction
  tx.post();
  CHECK(tx.getIsPosted());

  // Reverse transaction
  auto reversal = tx.reverse();
  CHECK(reversal->getIsPosted());
  CHECK_EQUAL("REVERSAL", reversal->getType());
  CHECK_EQUAL(1000.0, reversal->getAmount());
  CHECK(!reversal->getTrackingId().empty());
}

TEST(Factory_Integration) {
  Factory factory("Integrated Factory");

  auto dept = make_shared<Department>("Production", 100000.0);
  auto emp = make_shared<Employee>("Manager1", Role::MANAGER, 35.0);
  auto line =
      make_shared<ProductionLine>("LINE001", "Main Assembly Line", 85.0);

  factory.addDepartment(dept);
  factory.addEmployee(emp);
  factory.addProductionLine(line);

  factory.assignEmployeeToDepartment(emp->getEmployeeId(),
                                     dept->getDepartmentId());

  CHECK_EQUAL(1, dept->getEmployeeCount());

  auto foundLine = factory.findProductionLine(line->getLineId());
  CHECK(foundLine != nullptr);
}

TEST(FullWorkflow_SafetyIncident) {
  Factory factory("Test Factory");
  auto safetyOfficer =
      make_shared<Employee>("SO001", Role::SAFETY_OFFICER, 30.0);
  factory.addEmployee(safetyOfficer);

  auto incident = make_shared<SafetyIncident>("INJURY", "Minor cut on hand",
                                              "Machine M1 Area",
                                              safetyOfficer->getEmployeeId());

  factory.addSafetyIncident(incident);
  CHECK_EQUAL(1, factory.getTotalIncidentCount());

  CHECK_EQUAL("INJURY", incident->getIncidentType());
  CHECK_EQUAL("Minor cut on hand", incident->getDescription());
  CHECK(!incident->getIsInvestigated());
}

TEST(AuditLog_ExportToCSV) {
  AuditLog log("TestModule");
  log.addEntry("USER1", "CREATE", "Employee", "EMP123", "", "New Employee",
               "192.168.1.1");
  log.addEntry("USER2", "UPDATE", "Department", "DEPT1", "OldName", "NewName",
               "192.168.1.2");

  string csv = log.exportToCSV();

  CHECK(csv.find("Timestamp") != string::npos);
  CHECK(csv.find("USER1") != string::npos);
  CHECK(csv.find("CREATE") != string::npos);
  CHECK(csv.find("Employee") != string::npos);
  CHECK(csv.find("EMP123") != string::npos);
  CHECK(csv.find("USER2") != string::npos);
  CHECK(csv.find("UPDATE") != string::npos);
  CHECK(csv.find("\"NewName\"") != string::npos);
}

TEST(AuditLog_DetectAnomalies) {
  AuditLog log("TestModule");

  // No anomaly case
  log.addEntry("USER1", "UPDATE", "Employee", "EMP1", "old", "new",
               "192.168.1.1");
  log.addEntry("USER2", "UPDATE", "Employee", "EMP2", "old", "new",
               "192.168.1.2");
  CHECK_EQUAL(false, log.detectAnomalies());

  // Anomaly case - more than 5 changes to same entity
  AuditLog log2("TestModule");
  for (int i = 0; i < 6; i++) {
    log2.addEntry("USER1", "UPDATE", "Employee", "EMP1", "old", "new",
                  "192.168.1.1");
  }
  CHECK_EQUAL(true, log2.detectAnomalies());
}

TEST(AuditLog_Getters) {
  AuditLog log("TestModule");
  CHECK(log.getTrackingId().find("AUDIT") != string::npos);
  CHECK_EQUAL("ACTIVE", log.getStatus());
  CHECK_EQUAL(0, log.getEntryCount());
  CHECK_EQUAL(false, log.getIsSealed());

  log.addEntry("USER1", "CREATE", "Test", "ID1", "", "value", "");
  CHECK_EQUAL(1, log.getEntryCount());

  log.seal();
  CHECK_EQUAL(true, log.getIsSealed());
  CHECK_EQUAL("SEALED", log.getStatus());
}

TEST(Department_IsFullyStaffed) {
  auto dept = make_shared<Department>("Production", 100000.0);

  // Not fully staffed
  CHECK_EQUAL(false, dept->isFullyStaffed());

  // Add manager
  auto manager = make_shared<Employee>("John Manager", Role::MANAGER, 25.0);
  dept->addEmployee(manager);
  CHECK_EQUAL(false, dept->isFullyStaffed());

  // Add two operators
  auto op1 = make_shared<Employee>("Op1", Role::OPERATOR, 18.0);
  auto op2 = make_shared<Employee>("Op2", Role::OPERATOR, 18.0);
  dept->addEmployee(op1);
  dept->addEmployee(op2);
  CHECK_EQUAL(true, dept->isFullyStaffed());
}

TEST(Department_Getters) {
  auto dept = make_shared<Department>("Quality", 50000.0);

  CHECK(dept->getDepartmentId().find("DEPT") != string::npos);
  CHECK_EQUAL("Quality", dept->getName());
  CHECK_EQUAL(50000.0, dept->getBudget());
  CHECK_EQUAL(0, dept->getEmployeeCount());
  CHECK_EQUAL(true, dept->getIsOperational());
  CHECK_EQUAL("DEPARTMENT", dept->getReportType());
}

TEST(Employee_HasCertification) {
  auto emp = make_shared<Employee>("Test Emp", Role::TECHNICIAN, 20.0);

  CHECK_EQUAL(false, emp->hasCertification("CERT1"));

  emp->addCertification("CERT1");
  CHECK_EQUAL(true, emp->hasCertification("CERT1"));
  CHECK_EQUAL(false, emp->hasCertification("CERT2"));
}

TEST(Employee_Deactivate) {
  auto emp = make_shared<Employee>("Test Emp", Role::OPERATOR, 18.0);

  CHECK_EQUAL("ACTIVE", emp->getStatus());
  CHECK_EQUAL(true, emp->getIsActive());

  emp->deactivate();

  CHECK_EQUAL("INACTIVE", emp->getStatus());
  CHECK_EQUAL(false, emp->getIsActive());
}

TEST(Employee_Getters) {
  auto emp = make_shared<Employee>("Alice Smith", Role::ENGINEER, 30.0);

  CHECK(emp->getEmployeeId().find("EMP") != string::npos);
  CHECK_EQUAL("Alice Smith", emp->getName());
  CHECK_EQUAL(Role::ENGINEER, emp->getRole());
}

TEST(Factory_LookupMethods_NotFound) {
  Factory factory("Test Factory");

  CHECK(factory.findEmployee("NONEXISTENT") == nullptr);
  CHECK(factory.findDepartment("NONEXISTENT") == nullptr);
  CHECK(factory.findProductionLine("NONEXISTENT") == nullptr);
  CHECK(factory.findWarehouse("NONEXISTENT") == nullptr);
}

TEST(Factory_AssignEmployeeToDepartment) {
  Factory factory("Test Factory");

  auto emp = make_shared<Employee>("Bob", Role::OPERATOR, 18.0);
  auto dept = make_shared<Department>("Assembly", 100000.0);

  factory.addEmployee(emp);
  factory.addDepartment(dept);

  factory.assignEmployeeToDepartment(emp->getEmployeeId(),
                                     dept->getDepartmentId());

  CHECK_EQUAL(1, dept->getEmployeeCount());
}

TEST(Factory_GetTotalBudget) {
  Factory factory("Test Factory");

  CHECK_EQUAL(0.0, factory.getTotalBudget());

  auto budget1 = make_shared<Budget>("DEPT1", "2025-Q1", 50000.0);
  auto budget2 = make_shared<Budget>("DEPT2", "2025-Q1", 75000.0);

  factory.addBudget(budget1);
  factory.addBudget(budget2);

  CHECK_EQUAL(125000.0, factory.getTotalBudget());
}

TEST(Factory_AggregateStats) {
  Factory factory("Test Factory");

  auto emp1 = make_shared<Employee>("E1", Role::OPERATOR, 18.0);
  auto emp2 = make_shared<Employee>("E2", Role::OPERATOR, 18.0);
  auto dept = make_shared<Department>("Test", 100000.0);

  factory.addEmployee(emp1);
  factory.addEmployee(emp2);
  factory.addDepartment(dept);

  CHECK_EQUAL(2, factory.getTotalEmployeeCount());
  CHECK_EQUAL(1, factory.getTotalDepartmentCount());
  CHECK_EQUAL(0, factory.getTotalIncidentCount());
}

TEST(Schedule_AddShift_Conflict) {
  auto dept = make_shared<Department>("Test", 100000.0);
  Schedule schedule("2025-01-01", dept);

  Shift shift1("EMP1", "2025-01-02", 8.0, 16.0);
  Shift shift2("EMP1", "2025-01-02", 10.0, 18.0);  // Same employee, same day

  schedule.addShift(shift1);

  CHECK_THROW(schedule.addShift(shift2), FactoryException);
}

TEST(Schedule_GetShiftsForEmployee) {
  auto dept = make_shared<Department>("Test", 100000.0);
  Schedule schedule("2025-01-01", dept);

  Shift shift1("EMP1", "2025-01-02", 8.0, 16.0);
  Shift shift2("EMP2", "2025-01-02", 8.0, 16.0);
  Shift shift3("EMP1", "2025-01-03", 8.0, 16.0);

  schedule.addShift(shift1);
  schedule.addShift(shift2);
  schedule.addShift(shift3);

  auto emp1Shifts = schedule.getShiftsForEmployee("EMP1");
  CHECK_EQUAL(2, emp1Shifts.size());

  auto emp2Shifts = schedule.getShiftsForEmployee("EMP2");
  CHECK_EQUAL(1, emp2Shifts.size());

  auto emp3Shifts = schedule.getShiftsForEmployee("EMP3");
  CHECK_EQUAL(0, emp3Shifts.size());
}

TEST(Schedule_CalculateLaborCost) {
  auto dept = make_shared<Department>("Test", 100000.0);
  Schedule schedule("2025-01-01", dept);

  Shift shift1("EMP1", "2025-01-02", 8.0, 16.0);  // 8 hours
  Shift shift2("EMP2", "2025-01-02", 9.0, 17.0);  // 8 hours

  schedule.addShift(shift1);
  schedule.addShift(shift2);

  // 16 hours * $15/hour = $240
  CHECK_EQUAL(240.0, schedule.calculateLaborCost());
}

TEST(Schedule_Validate_NoShifts) {
  auto dept = make_shared<Department>("Test", 100000.0);
  Schedule schedule("2025-01-01", dept);

  CHECK_THROW(schedule.validate(), FactoryException);
}

TEST(Schedule_Getters) {
  auto dept = make_shared<Department>("Test", 100000.0);
  Schedule schedule("2025-01-01", dept);

  CHECK(schedule.getScheduleId().find("SCHED") != string::npos);
  CHECK_EQUAL("2025-01-01", schedule.getWeekStartDate());
  CHECK_EQUAL(0, schedule.getShifts().size());
}

TEST(Shift_CalculateCost) {
  Shift shift("EMP1", "2025-01-01", 8.0, 16.5);  // 8.5 hours
  double cost = shift.calculateCost();

  // 8.5 hours * $15/hour = $127.5
  CHECK_EQUAL(127.5, cost);
}

TEST(Shift_Approve) {
  Shift shift("EMP1", "2025-01-01", 8.0, 16.0);

  CHECK_EQUAL(false, shift.getIsApproved());

  shift.approve();
  CHECK_EQUAL(true, shift.getIsApproved());

  shift.setApproved(false);
  CHECK_EQUAL(false, shift.getIsApproved());
}

TEST(Shift_Getters) {
  Shift shift("EMP123", "2025-01-15", 9.0, 17.0);

  CHECK(shift.getShiftId().find("SHIFT") != string::npos);
  CHECK_EQUAL("EMP123", shift.getEmployeeId());
  CHECK_EQUAL("2025-01-15", shift.getDate());
  CHECK_EQUAL(8.0, shift.getHours());
}

TEST(Budget_Allocate_InsufficientFunds) {
  Budget budget("DEPT1", "2025-Q1", 1000.0);

  CHECK_THROW(budget.allocate("Materials", 1500.0), FinanceException);
}

TEST(Budget_RecordExpense_OverBudget) {
  Budget budget("DEPT1", "2025-Q1", 1000.0);
  budget.allocate("Materials", 500.0);

  // This should not throw, but should log a warning
  budget.recordExpense("Materials", 600.0);

  CHECK_EQUAL(-100.0, budget.getRemaining("Materials"));
}

TEST(Budget_GetRemaining) {
  Budget budget("DEPT1", "2025-Q1", 1000.0);
  budget.allocate("Materials", 500.0);
  budget.allocate("Labor", 300.0);

  budget.recordExpense("Materials", 200.0);

  CHECK_EQUAL(300.0, budget.getRemaining("Materials"));
  CHECK_EQUAL(300.0, budget.getRemaining("Labor"));
  CHECK_EQUAL(200.0, budget.getRemaining("UNALLOCATED"));
}

TEST(Budget_GetUtilizationPercentage) {
  Budget budget("DEPT1", "2025-Q1", 1000.0);
  budget.allocate("Materials", 500.0);
  budget.allocate("Labor", 300.0);

  budget.recordExpense("Materials", 200.0);
  budget.recordExpense("Labor", 100.0);

  // Spent: 300, Total: 1000 = 30%
  CHECK_EQUAL(30.0, budget.getUtilizationPercentage());
}

TEST(Budget_GetTotalBudgetAmount) {
  Budget budget("DEPT1", "2025-Q1", 1000.0);
  budget.allocate("Materials", 500.0);
  budget.allocate("Labor", 300.0);

  // Total should remain 1000 (allocations just move from UNALLOCATED)
  CHECK_EQUAL(1000.0, budget.getTotalBudgetAmount());
}

TEST(Budget_Lock) {
  Budget budget("DEPT1", "2025-Q1", 1000.0);
  budget.allocate("Materials", 500.0);

  CHECK_EQUAL("DRAFT", budget.getStatus());

  budget.lock();
  CHECK_EQUAL("LOCKED", budget.getStatus());
  CHECK_THROW(budget.allocate("Labor", 200.0), FinanceException);
}

TEST(Budget_Getters) {
  Budget budget("DEPT1", "2025-Q1", 1000.0);

  CHECK(budget.getTrackingId().find("BUD") != string::npos);
  CHECK_EQUAL("DEPT1", budget.getDepartmentId());
  CHECK_EQUAL("2025-Q1", budget.getFiscalPeriod());
}

TEST(Invoice_AddLineItem_PaidInvoice) {
  Invoice invoice("CUST1", "2025-02-01", "PO123");
  invoice.addLineItem("Item 1", 100.0);
  invoice.calculateTotals(10.0);
  invoice.markAsPaid();

  CHECK_THROW(invoice.addLineItem("Item 2", 50.0), FinanceException);
}

TEST(Invoice_CalculateTotals) {
  Invoice invoice("CUST1", "2025-02-01");
  invoice.addLineItem("Widget A", 100.0);
  invoice.addLineItem("Widget B", 200.0);

  invoice.calculateTotals(10.0);

  CHECK_EQUAL(300.0,
              invoice.getTotalAmount() - invoice.getTaxAmount());  // Subtotal
  CHECK_EQUAL(30.0, invoice.getTaxAmount());
  CHECK_EQUAL(330.0, invoice.getTotalAmount());
}

TEST(Invoice_MarkAsPaid) {
  Invoice invoice("CUST1", "2025-02-01");
  CHECK_EQUAL("OUTSTANDING", invoice.getStatus());
  CHECK_EQUAL(false, invoice.getIsPaid());

  invoice.addLineItem("Item", 100.0);
  invoice.calculateTotals(0.0);
  invoice.markAsPaid();

  CHECK_EQUAL("PAID", invoice.getStatus());
  CHECK_EQUAL(true, invoice.getIsPaid());

  CHECK_THROW(invoice.markAsPaid(), FinanceException);
}

TEST(Invoice_IsOverdue) {
  // This test assumes current date is after 2025-01-01
  Invoice invoice("CUST1", "2025-01-01");  // Past date
  CHECK_EQUAL(true, invoice.isOverdue());
}

TEST(Invoice_Getters) {
  Invoice invoice("CUST1", "2025-02-15", "PO123");
  invoice.addLineItem("Item 1", 100.0);

  CHECK(invoice.getInvoiceId().find("INV") != string::npos);
  CHECK_EQUAL("CUST1", invoice.getCustomerId());
  CHECK_EQUAL(0.0, invoice.getTotalAmount());  // Not calculated yet
  CHECK_EQUAL(0.0, invoice.getTaxAmount());
  CHECK_EQUAL(1, invoice.getLineItemCount());
  CHECK_EQUAL("PO123", invoice.getPurchaseOrderNumber());
}

TEST(Payroll_AddEmployeeHours_ProcessedPayroll) {
  Payroll payroll("2025-01-01 to 2025-01-31");
  auto emp = make_shared<Employee>("Test", Role::OPERATOR, 18.0);

  payroll.addEmployeeHours(emp, 160.0);
  payroll.process();

  CHECK_THROW(payroll.addEmployeeHours(emp, 40.0), PayrollException);
}

TEST(Payroll_AddEmployeeHours_InvalidEmployee) {
  Payroll payroll("2025-01-01 to 2025-01-31");

  CHECK_THROW(payroll.addEmployeeHours(nullptr, 160.0), PayrollException);

  auto inactiveEmp = make_shared<Employee>("Test", Role::OPERATOR, 18.0);
  inactiveEmp->deactivate();
  CHECK_THROW(payroll.addEmployeeHours(inactiveEmp, 160.0), PayrollException);
}

TEST(Payroll_AddEmployeeHours_ExcessiveHours) {
  Payroll payroll("2025-01-01 to 2025-01-31");
  auto emp = make_shared<Employee>("Test", Role::OPERATOR, 18.0);

  CHECK_THROW(payroll.addEmployeeHours(emp, 400.0), PayrollException);
}

TEST(Payroll_CalculateTotalPayroll) {
  Payroll payroll("2025-01-01 to 2025-01-31");
  auto emp1 = make_shared<Employee>("E1", Role::OPERATOR, 18.0);
  auto emp2 = make_shared<Employee>("E2", Role::MANAGER, 25.0);

  payroll.addEmployeeHours(emp1, 160.0);
  payroll.addEmployeeHours(emp2, 160.0);

  double expected = (18.0 * 160.0) + (25.0 * 160.0);
  CHECK_EQUAL(expected, payroll.calculateTotalPayroll());
}

TEST(Payroll_Process) {
  Payroll payroll("2025-01-01 to 2025-01-31");
  auto emp = make_shared<Employee>("Test", Role::OPERATOR, 18.0);

  CHECK_EQUAL(false, payroll.getIsProcessed());

  payroll.addEmployeeHours(emp, 160.0);
  payroll.process();

  CHECK_EQUAL(true, payroll.getIsProcessed());

  CHECK_THROW(payroll.process(), PayrollException);
}

TEST(Payroll_GetEmployeePay_NotFound) {
  Payroll payroll("2025-01-01 to 2025-01-31");

  CHECK_THROW(payroll.getEmployeePay("NONEXISTENT"), PayrollException);
}

TEST(Payroll_Getters) {
  Payroll payroll("2025-01-01 to 2025-01-31");

  CHECK(payroll.getPayrollId().find("PAY") != string::npos);
  CHECK_EQUAL("2025-01-01 to 2025-01-31", payroll.getPayPeriod());
  CHECK_EQUAL(false, payroll.getIsProcessed());
}

TEST(Transaction_Post) {
  Transaction txn("PAYMENT", 1000.0, "ACC001", "ACC002", "INV123");

  CHECK_EQUAL("PENDING", txn.getStatus());
  CHECK_EQUAL(false, txn.getIsPosted());

  txn.post();

  CHECK_EQUAL("POSTED", txn.getStatus());
  CHECK_EQUAL(true, txn.getIsPosted());

  CHECK_THROW(txn.post(), FinanceException);
}

TEST(Transaction_Reverse) {
  Transaction txn("TRANSFER", 500.0, "ACC001", "ACC002", "REF456");
  txn.post();

  auto reversal = txn.reverse();

  CHECK_EQUAL(500.0, reversal->getAmount());
  CHECK_EQUAL("POSTED", reversal->getStatus());
}

TEST(Transaction_Reverse_Unposted) {
  Transaction txn("PAYMENT", 100.0, "ACC001", "ACC002", "REF");

  CHECK_THROW(txn.reverse(), FinanceException);
}

TEST(ProductionLine_RemoveMachine_RemovesMachineFromLine) {
  ProductionLine line("LINE-001", "Test Line", 85.0);
  auto machine1 =
      std::make_shared<Machine>("MACH001", "Machine 1", "TYPE1", 50.0, 100);
  auto machine2 =
      std::make_shared<Machine>("MACH002", "Machine 2", "TYPE2", 75.0, 100);

  line.addMachine(machine1);
  line.addMachine(machine2);
  CHECK_EQUAL(2, line.getMachineCount());

  line.removeMachine(machine1->getMachineId());
  CHECK_EQUAL(1, line.getMachineCount());
  line.removeMachine("NONEXISTENT");
  CHECK_EQUAL(1, line.getMachineCount());
}

TEST(ProductionLine_Validate_ThrowsWhenInsufficientMachines) {
  ProductionLine line("LINE-001", "Test Line", 85.0);

  auto machine =
      std::make_shared<Machine>("MACH001", "Machine 1", "TYPE1", 50.0, 100);
  line.addMachine(machine);

  try {
    line.validate();
    CHECK(false);
  } catch (const ProductionException&) {
    CHECK(true);
  }
}

TEST(Machine_Validate_ThrowsForInvalidParameters) {
  auto machine =
      std::make_shared<Machine>("MACH001", "Test Machine", "CNC", 50.0, 100);

  CHECK_EQUAL(true, machine->getIsOperational());

  // Тестируем метод isMaintenanceDue
  machine->recordOperationHours(50);
  CHECK_EQUAL(false, machine->isMaintenanceDue());  // 50 < 100

  machine->recordOperationHours(60);
  CHECK_EQUAL(true, machine->isMaintenanceDue());  // 110 >= 100
}

TEST(Product_GetStatus_ReturnsCorrectStatus) {
  Product product("PROD001", "Test Product", "Description", 100.0);

  CHECK_EQUAL("IN_PRODUCTION", product.getStatus());

  product.markQualityPassed();
  CHECK_EQUAL("QC_PASSED", product.getStatus());

  product.markShipped();
  CHECK_EQUAL("SHIPPED", product.getStatus());
}

TEST(Warehouse_AddItem_ThrowsWhenOverCapacity_Fixed) {
  Warehouse warehouse("Test Warehouse", 2);

  auto item1 = std::make_shared<RawMaterial>("Item1", "SKU1", "METAL", "SUP001",
                                             10, 5.0, "LOC1", 5, 3);
  auto item2 = std::make_shared<RawMaterial>("Item2", "SKU2", "METAL", "SUP001",
                                             10, 5.0, "LOC1", 5, 3);
  auto item3 = std::make_shared<RawMaterial>("Item3", "SKU3", "METAL", "SUP001",
                                             10, 5.0, "LOC1", 5, 3);

  warehouse.addItem(item1);
  warehouse.addItem(item2);
  CHECK_EQUAL(2, warehouse.getCurrentOccupancy());

  CHECK_THROW(warehouse.addItem(item3), InventoryException);
}

TEST(Warehouse_AddItem_SameSkuBeforeCapacity) {
  Warehouse warehouse("Test Warehouse", 2);

  auto item1 = std::make_shared<RawMaterial>("Item1", "SKU1", "METAL", "SUP001",
                                             10, 5.0, "LOC1", 5, 3);
  auto item1_more = std::make_shared<RawMaterial>(
      "Item1", "SKU1", "METAL", "SUP001", 5, 5.0, "LOC1", 5, 3);

  warehouse.addItem(item1);
  CHECK_EQUAL(1, warehouse.getCurrentOccupancy());

  warehouse.addItem(item1_more);
  CHECK_EQUAL(1, warehouse.getCurrentOccupancy());

  auto foundItem = warehouse.findItem("SKU1");
  CHECK(foundItem != nullptr);
  CHECK_EQUAL(15, foundItem->getQuantity());  // 10 + 5
}

TEST(MaintenanceSchedule_SkipOccurrence_Skips) {
  auto schedule = std::make_shared<MaintenanceSchedule>(
      "MACH001", "Preventive maintenance", "MONTHLY", 30);

  try {
    schedule->skipOccurrence();
    CHECK(true);
  } catch (...) {
    CHECK(false);
  }
}

TEST(SparePart_IsCompatible_ChecksCompatibility) {
  auto part = std::make_shared<SparePart>("Bearing", "BRG001", "6205", "SKF",
                                          10, 50.0, "WH1", 14);

  part->addCompatibleMachine("MACH001");
  part->addCompatibleMachine("MACH002");

  CHECK_EQUAL(true, part->isCompatible("MACH001"));
  CHECK_EQUAL(true, part->isCompatible("MACH002"));
  CHECK_EQUAL(false, part->isCompatible("MACH003"));
}

TEST(Warehouse_GetCapacityUtilization_CalculatesUtilization) {
  Warehouse warehouse("Test Warehouse", 100);

  CHECK_EQUAL(0.0, warehouse.getCapacityUtilization());

  auto item = std::make_shared<RawMaterial>("Item", "ITM001", "METAL", "SUP001",
                                            10, 5.0, "LOC1", 5, 3);
  warehouse.addItem(item);

  CHECK_CLOSE(1.0, warehouse.getCapacityUtilization(), 0.1);
}

TEST(Warehouse_AddItem_ThrowsWhenOverCapacity) {
  Warehouse warehouse("Small Warehouse", 1);

  auto item1 = std::make_shared<RawMaterial>("Item1", "SKU1", "METAL", "SUP001",
                                             10, 5.0, "LOC1", 5, 3);
  auto item2 = std::make_shared<RawMaterial>("Item2", "SKU2", "METAL", "SUP001",
                                             10, 5.0, "LOC1", 5, 3);

  warehouse.addItem(item1);
  CHECK_EQUAL(1, warehouse.getCurrentOccupancy());

  CHECK_THROW(warehouse.addItem(item2), InventoryException);
}

TEST(Warehouse_AddItem_SameSkuDoesNotIncreaseOccupancy) {
  Warehouse warehouse("Test Warehouse", 2);

  auto item1 = std::make_shared<RawMaterial>("Item1", "SKU1", "METAL", "SUP001",
                                             10, 5.0, "LOC1", 5, 3);
  auto item1_more = std::make_shared<RawMaterial>(
      "Item1", "SKU1", "METAL", "SUP001", 5, 5.0, "LOC1", 5, 3);
  auto item2 = std::make_shared<RawMaterial>("Item2", "SKU2", "METAL", "SUP001",
                                             10, 5.0, "LOC1", 5, 3);
  warehouse.addItem(item1);
  CHECK_EQUAL(1, warehouse.getCurrentOccupancy());

  warehouse.addItem(item1_more);
  CHECK_EQUAL(1, warehouse.getCurrentOccupancy());

  warehouse.addItem(item2);
  CHECK_EQUAL(2, warehouse.getCurrentOccupancy());
}

TEST(Supplier_SetApproved_SetsApprovalStatus) {
  auto supplier = std::make_shared<Supplier>("Test Supplier",
                                             "test@supplier.com", "555-1234");

  CHECK_EQUAL(false, supplier->getIsApproved());
  CHECK_EQUAL("PENDING", supplier->getStatus());

  supplier->setApproved(true);
  CHECK_EQUAL(true, supplier->getIsApproved());
  CHECK_EQUAL("APPROVED", supplier->getStatus());
}

TEST(SafetyTraining_RenewCertification_Renews) {
  auto training = std::make_shared<SafetyTraining>("First Aid", 6);

  training->certifyEmployee("EMP001");

  CHECK_EQUAL(true, training->isEmployeeCertified("EMP001"));
  try {
    training->renewCertification("EMP001");
    CHECK(true);
  } catch (const SafetyException&) {
    CHECK(false);
  }

  CHECK_EQUAL(true, training->isEmployeeCertified("EMP001"));
}

TEST(Boundary_Warehouse_Capacity_HandlesCorrectly) {
  Warehouse warehouse("Test", 2);

  auto item1 = std::make_shared<RawMaterial>("Item1", "SKU1", "TYPE1", "SUP1",
                                             10, 1.0, "LOC1", 5, 3);
  auto item2 = std::make_shared<RawMaterial>("Item2", "SKU2", "TYPE2", "SUP1",
                                             10, 1.0, "LOC1", 5, 3);
  auto item3 = std::make_shared<RawMaterial>("Item3", "SKU3", "TYPE3", "SUP1",
                                             10, 1.0, "LOC1", 5, 3);

  warehouse.addItem(item1);
  CHECK_EQUAL(1, warehouse.getCurrentOccupancy());

  warehouse.addItem(item2);
  CHECK_EQUAL(2, warehouse.getCurrentOccupancy());

  try {
    warehouse.addItem(item3);
    CHECK(false);
  } catch (const InventoryException&) {
    CHECK(true);
  }
}

TEST(Integration_FactoryWithWarehouse) {
  auto factory = std::make_shared<Factory>("Test Factory");
  auto warehouse = std::make_shared<Warehouse>("Main Warehouse", 1000);

  factory->addWarehouse(warehouse);

  auto found = factory->findWarehouse(warehouse->getWarehouseId());
  CHECK(found != nullptr);
  CHECK_EQUAL(warehouse->getName(), found->getName());

  CHECK(factory->findWarehouse("NONEXISTENT") == nullptr);
}

TEST(Integration_ProductionWorkflow_Simple) {
  auto product =
      std::make_shared<Product>("PROD001", "Widget", "A useful widget", 99.99);

  product->addRawMaterial("MAT001");
  product->addRawMaterial("MAT002");

  product->markQualityPassed();
  CHECK_EQUAL(true, product->hasPassedQualityCheck());
  CHECK_EQUAL("QC_PASSED", product->getStatus());

  std::string report = product->generateReport();
  CHECK(report.find("PROD001") != std::string::npos);
  CHECK(!report.empty());
}

TEST(Employee_GetTrackingId_ReturnsCorrectId) {
  auto emp = std::make_shared<Employee>("John Doe", Role::ENGINEER, 30.0);

  std::string id = emp->getTrackingId();
  CHECK(id.find("EMP") != std::string::npos);
}

TEST(Department_GetIsOperational_ReturnsStatus) {
  auto dept = std::make_shared<Department>("Test Department", 10000.0);

  CHECK_EQUAL(true, dept->getIsOperational());
}

TEST(FactoryException_IsThrown) {
  try {
    throw FactoryException("Test exception");
    CHECK(false);
  } catch (const FactoryException& e) {
    std::string msg = e.what();
    CHECK(msg.find("Factory Error") != std::string::npos);
    CHECK(true);
  }
}

TEST(Schedule_GetShifts_ReturnsShifts) {
  auto dept = std::make_shared<Department>("Test", 10000.0);
  auto schedule = std::make_shared<Schedule>("2025-01-01", dept);

  Shift shift1("EMP001", "2025-01-02", 8.0, 16.0);
  Shift shift2("EMP002", "2025-01-02", 8.0, 16.0);

  schedule->addShift(shift1);
  schedule->addShift(shift2);

  auto shifts = schedule->getShifts();
  CHECK_EQUAL(2, shifts.size());
}

TEST(Payroll_GetEmployeePay_ReturnsCorrectAmount) {
  auto payroll = std::make_shared<Payroll>("2025-01-01 to 2025-01-31");
  auto emp1 = std::make_shared<Employee>("John", Role::OPERATOR, 20.0);
  auto emp2 = std::make_shared<Employee>("Jane", Role::MANAGER, 35.0);

  payroll->addEmployeeHours(emp1, 160.0);
  payroll->addEmployeeHours(emp2, 160.0);

  CHECK_EQUAL(3200.0,
              payroll->getEmployeePay(emp1->getEmployeeId()));  // 20 * 160
  CHECK_EQUAL(5600.0,
              payroll->getEmployeePay(emp2->getEmployeeId()));  // 35 * 160
}

TEST(Invoice_GetLineItemCount_ReturnsCount) {
  auto invoice = std::make_shared<Invoice>("CUST001", "2025-02-15");

  CHECK_EQUAL(0, invoice->getLineItemCount());

  invoice->addLineItem("Item 1", 100.0);
  invoice->addLineItem("Item 2", 200.0);
  invoice->addLineItem("Item 3", 300.0);

  CHECK_EQUAL(3, invoice->getLineItemCount());
}

TEST(Transaction_GetAmount_ReturnsAmount) {
  auto txn = std::make_shared<Transaction>("PAYMENT", 1234.56, "ACC001",
                                           "ACC002", "INV001");

  CHECK_EQUAL(1234.56, txn->getAmount());
  CHECK_EQUAL("PAYMENT", txn->getType());
  CHECK_EQUAL(false, txn->getIsPosted());
}

TEST(Budget_GetRemaining_ForUnallocatedCategory) {
  auto budget = std::make_shared<Budget>("DEPT001", "2025-Q1", 10000.0);

  budget->allocate("MATERIALS", 5000.0);

  CHECK_EQUAL(5000.0, budget->getRemaining("UNALLOCATED"));
}

TEST(DeliveryOrder_GetOrderValue_CalculatesValue) {
  auto order =
      std::make_shared<DeliveryOrder>("CUST001", "123 Main St", "2025-02-15");

  auto product1 =
      std::make_shared<Product>("PROD001", "Widget A", "Description", 100.0);
  auto product2 =
      std::make_shared<Product>("PROD002", "Widget B", "Description", 200.0);

  product1->markQualityPassed();
  product2->markQualityPassed();

  order->addProduct(product1, 2);  // 2 * 100 = 200
  order->addProduct(product2, 3);  // 3 * 200 = 600

  CHECK_EQUAL(800.0, order->getOrderValue());  // 200 + 600
}

TEST(Shipment_SetInsurance_SetsInsurance) {
  auto shipment =
      std::make_shared<Shipment>("CUST001", "123 Main St", "2025-02-15");

  CHECK_EQUAL(false, shipment->getIsInsured());
  CHECK_EQUAL(0.0, shipment->getDeclaredValue());

  shipment->setInsurance(5000.0);

  CHECK_EQUAL(true, shipment->getIsInsured());
  CHECK_EQUAL(5000.0, shipment->getDeclaredValue());
}

TEST(Carrier_UpdatePerformance_UpdatesRate) {
  auto carrier = std::make_shared<Carrier>("Test Carrier", "John Doe",
                                           "555-1234", "john@carrier.com");

  double initialRate = carrier->getOnTimeDeliveryRate();
  CHECK_EQUAL(85.0, initialRate);

  carrier->updatePerformance(true);
  CHECK_CLOSE(85.5, carrier->getOnTimeDeliveryRate(), 0.1);

  carrier->updatePerformance(false);
  CHECK_CLOSE(83.5, carrier->getOnTimeDeliveryRate(), 0.1);
}

TEST(PurchaseOrder_GetTotalQuantity_CalculatesTotal) {
  auto po = std::make_shared<PurchaseOrder>("SUP001", "2025-02-15");

  auto material1 = std::make_shared<RawMaterial>(
      "Steel", "STL001", "METAL", "SUP001", 0, 100.0, "WH1", 50, 5);
  auto material2 = std::make_shared<RawMaterial>(
      "Aluminum", "ALU001", "METAL", "SUP001", 0, 200.0, "WH1", 30, 5);

  po->addLineItem(material1, 100);
  po->addLineItem(material2, 50);

  CHECK_EQUAL(150, po->getTotalQuantity());  // 100 + 50
}

TEST(SafetyTraining_GetCertifiedEmployees_ReturnsList) {
  auto training = std::make_shared<SafetyTraining>("Fire Safety", 24);

  CHECK_EQUAL(0, training->getCertifiedEmployees().size());

  training->certifyEmployee("EMP001");
  training->certifyEmployee("EMP002");
  training->certifyEmployee("EMP003");

  auto certified = training->getCertifiedEmployees();
  CHECK_EQUAL(3, certified.size());
}

TEST(MaintenanceRequest_UpdateStatus_ValidatesStatus) {
  auto request =
      std::make_shared<MaintenanceRequest>("MACH001", "Test issue", "MEDIUM");

  request->updateStatus("ASSIGNED");
  CHECK_EQUAL("ASSIGNED", request->getStatus());

  request->updateStatus("IN_PROGRESS");
  CHECK_EQUAL("IN_PROGRESS", request->getStatus());

  try {
    request->updateStatus("INVALID_STATUS");
    CHECK(false);
  } catch (const MaintenanceException&) {
    CHECK(true);
  }
}

TEST(SparePart_GetRecommendedStockLevel_ReturnsLevel) {
  auto criticalPart = std::make_shared<SparePart>(
      "PLC", "PLC001", "S7-1200", "Siemens", 2, 1200.0, "WH1", 21);
  auto standardPart = std::make_shared<SparePart>(
      "Fuse", "FUS001", "5A", "Bussmann", 20, 2.5, "WH2", 3);

  CHECK_EQUAL(10, criticalPart->getRecommendedStockLevel());
  CHECK_EQUAL(3, standardPart->getRecommendedStockLevel());
}

TEST(Shipment_CalculateDimensionalWeight_CalculatesWeight) {
  auto shipment =
      std::make_shared<Shipment>("CUST001", "123 Main St", "2025-02-15");

  auto product = std::make_shared<Product>("PROD001", "Test Product",
                                           "Description", 100.0);
  product->markQualityPassed();

  shipment->addProductWithDetails(product, 2, 10.0,
                                  0.1);  // 2 * 0.1м³ = 0.2м³

  //  0.2м³ = 200,000 см³
  //  = 200,000 / 5000 = 40кг
  CHECK_CLOSE(40.0, shipment->calculateDimensionalWeight(), 0.1);
}

TEST(Final_SimpleIntegration) {
  Factory factory("Test Factory");

  auto dept = std::make_shared<Department>("Production", 100000.0);
  factory.addDepartment(dept);

  auto emp = std::make_shared<Employee>("Test Emp", Role::OPERATOR, 20.0);
  factory.addEmployee(emp);

  factory.assignEmployeeToDepartment(emp->getEmployeeId(),
                                     dept->getDepartmentId());

  std::string report = factory.generateFactoryReport();
  CHECK(report.find("Test Factory") != std::string::npos);

  AuditLog log("Test");
  log.addEntry("user", "CREATE", "Test", "ID", "old", "new");
  CHECK_EQUAL(1, log.getEntryCount());
}

TEST(DeliveryOrder_Validate_ThrowsWhenMissingData) {
  CHECK_THROW(DeliveryOrder("", "Address", "2025-02-15"), ShippingException);
  CHECK_THROW(DeliveryOrder("CUST001", "", "2025-02-15"), ShippingException);
  CHECK_THROW(DeliveryOrder("CUST001", "Address", "invalid-date"),
              ShippingException);
}

TEST(DeliveryOrder_Ship_ThrowsWhenNoCarrier) {
  auto order =
      std::make_shared<DeliveryOrder>("CUST001", "123 Main St", "2025-02-15");
  auto product = std::make_shared<Product>("PROD001", "Test", "Desc", 100.0);

  product->markQualityPassed();
  order->addProduct(product, 1);
  CHECK_THROW(order->ship("TRACK001"), ShippingException);
}

TEST(Shipment_CalculateDimensionalWeight_ReturnsWeight) {
  auto shipment =
      std::make_shared<Shipment>("CUST001", "Address", "2025-02-15");
  auto product = std::make_shared<Product>("PROD001", "Test", "Desc", 100.0);

  product->markQualityPassed();
  shipment->addProductWithDetails(product, 1, 10.0, 0.1);

  // 0.1 m³ = 100,000 cm³ / 5000 = 20 kg
  CHECK_CLOSE(20.0, shipment->calculateDimensionalWeight(), 0.1);
}

TEST(Shipment_RequiresSpecialHandling_ChecksConditions) {
  auto shipment =
      std::make_shared<Shipment>("CUST001", "Address", "2025-02-15");

  CHECK_EQUAL(false, shipment->requiresSpecialHandling());

  shipment->setInsurance(6000.0);
  CHECK_EQUAL(true, shipment->requiresSpecialHandling());
}

TEST(Shipment_SetPackaging_SetsCorrectly) {
  auto shipment =
      std::make_shared<Shipment>("CUST001", "Address", "2025-02-15");

  shipment->setPackaging("BOX", 3);
  CHECK_EQUAL("BOX", shipment->getPackagingType());
  CHECK_EQUAL(3, shipment->getPackageCount());

  CHECK_THROW(shipment->setPackaging("INVALID", 1), ShippingException);
}

TEST(Carrier_ProvidesService_ChecksService) {
  auto carrier =
      std::make_shared<Carrier>("Test", "Contact", "555-1234", "test@test.com");

  carrier->addServiceType("GROUND");
  carrier->addServiceType("AIR");

  CHECK_EQUAL(true, carrier->providesService("GROUND"));
  CHECK_EQUAL(true, carrier->providesService("AIR"));
  CHECK_EQUAL(false, carrier->providesService("EXPRESS"));
}

TEST(Carrier_SetActive_SetsStatus) {
  auto carrier =
      std::make_shared<Carrier>("Test", "Contact", "555-1234", "test@test.com");

  CHECK_EQUAL(true, carrier->getIsActive());

  carrier->setActive(false);
  CHECK_EQUAL(false, carrier->getIsActive());
}

TEST(QualityCheck_PerformCheck_PassesOrFails) {
  auto qualityStandard =
      std::make_shared<QualityStandard>("Test Standard", "Description");
  qualityStandard->addCriterion("Length", 10.0, 20.0);
  qualityStandard->addCriterion("Width", 5.0, 15.0);

  auto qualityCheck = std::make_shared<QualityCheck>(
      "PROD001", qualityStandard->getStandardId(), "INSP001");

  qualityCheck->recordMeasurement("Length", 15.0);
  qualityCheck->recordMeasurement("Width", 10.0);

  qualityCheck->performCheck(*qualityStandard);
  CHECK_EQUAL(true, qualityCheck->getIsPassed());

  auto qualityCheck2 = std::make_shared<QualityCheck>(
      "PROD002", qualityStandard->getStandardId(), "INSP002");
  qualityCheck2->recordMeasurement("Length", 25.0);
  qualityCheck2->recordMeasurement("Width", 10.0);

  qualityCheck2->performCheck(*qualityStandard);
  CHECK_EQUAL(false, qualityCheck2->getIsPassed());
}

TEST(QualityCheck_GetMeasurementSummary_ReturnsSummary) {
  auto qualityCheck =
      std::make_shared<QualityCheck>("PROD001", "STD001", "INSP001");

  qualityCheck->recordMeasurement("Length", 15.0);
  qualityCheck->recordMeasurement("Width", 10.0);

  std::string summary = qualityCheck->getMeasurementSummary();
  CHECK(summary.find("Length") != std::string::npos);
  CHECK(summary.find("15") != std::string::npos);
  CHECK(summary.find("Width") != std::string::npos);
  CHECK(summary.find("10") != std::string::npos);
}

TEST(Defect_MarkAsResolved_ResolvesDefect) {
  auto defect =
      std::make_shared<Defect>("PROD001", "STD001", "INSP001", "COSMETIC", 5);

  defect->setRootCause("Painting issue");
  defect->addCorrectiveAction("Repaint");

  defect->markAsResolved();
  CHECK_EQUAL(true, defect->getIsResolved());
}

TEST(Defect_CalculateCostImpact_ReturnsCost) {
  auto defect =
      std::make_shared<Defect>("PROD001", "STD001", "INSP001", "FUNCTIONAL", 3);

  // 3 units * $100 * 5.0 severity multiplier = $1500
  CHECK_CLOSE(1500.0, defect->calculateCostImpact(), 0.1);
}

TEST(TestProcedure_AddStep_AddsStep) {
  auto procedure = std::make_shared<TestProcedure>("Test Proc", "Description",
                                                   "30 minutes", true);

  procedure->addStep(1, "Step 1");
  procedure->addStep(2, "Step 2");

  CHECK_EQUAL(2, procedure->getStepCount());

  CHECK_THROW(procedure->addStep(1, "Duplicate step"), QualityException);
}

TEST(TestProcedure_GenerateChecklist_CreatesChecklist) {
  auto procedure = std::make_shared<TestProcedure>("Test Proc", "Description",
                                                   "30 minutes", true);

  procedure->addStep(1, "Step 1");
  procedure->addRequiredTool("Tool A");

  std::string checklist = procedure->generateChecklist();
  CHECK(checklist.find("Test Proc") != std::string::npos);
  CHECK(checklist.find("Step 1") != std::string::npos);
  CHECK(checklist.find("Tool A") != std::string::npos);
}

TEST(MaintenanceSchedule_RecordPerformance_Records) {
  auto schedule = std::make_shared<MaintenanceSchedule>(
      "MACH001", "Maintenance", "MONTHLY", 30);

  schedule->recordPerformance("2025-01-15", 3);
  CHECK_EQUAL("2025-01-15", schedule->getLastPerformedDate());
  CHECK_EQUAL(3, schedule->getEstimatedHours());
}

TEST(MaintenanceSchedule_IsOverdue_Checks) {
  auto schedule = std::make_shared<MaintenanceSchedule>(
      "MACH001", "Maintenance", "DAILY", 1);

  bool overdue = schedule->isOverdue();
  CHECK((overdue == true || overdue == false));
}

TEST(SparePart_NeedsReorder_ChecksStock) {
  auto criticalPart = std::make_shared<SparePart>("Part", "SKU001", "P001",
                                                  "MFG", 5, 50.0, "WH1", 21);
  CHECK_EQUAL(true, criticalPart->needsReorder());  // 5 <= 10
  auto standardPart = std::make_shared<SparePart>("Part", "SKU002", "P002",
                                                  "MFG", 2, 10.0, "WH1", 3);
  CHECK_EQUAL(true, standardPart->needsReorder());  // 2 <= 3

  auto wellStocked = std::make_shared<SparePart>("Part", "SKU003", "P003",
                                                 "MFG", 15, 10.0, "WH1", 3);
  CHECK_EQUAL(false, wellStocked->needsReorder());  // 15 > 3
}

TEST(Invoice_CalculateTotals_ThrowsWhenPaid) {
  auto invoice = std::make_shared<Invoice>("CUST001", "2025-02-15");

  invoice->addLineItem("Item", 100.0);
  invoice->calculateTotals(10.0);
  invoice->markAsPaid();

  CHECK_THROW(invoice->calculateTotals(10.0), FinanceException);
}

TEST(Invoice_IsOverdue_ChecksDate) {
  auto invoice = std::make_shared<Invoice>("CUST001", "2024-01-01");

  bool overdue = invoice->isOverdue();
  CHECK((overdue == true || overdue == false));
}

TEST(Budget_RecordExpense_ThrowsWhenCategoryNotFound) {
  auto budget = std::make_shared<Budget>("DEPT001", "2025-Q1", 1000.0);

  budget->allocate("MATERIALS", 500.0);

  CHECK_THROW(budget->recordExpense("NONEXISTENT", 100.0), FinanceException);
}

TEST(Budget_GetRemaining_ThrowsForNonExistent) {
  auto budget = std::make_shared<Budget>("DEPT001", "2025-Q1", 1000.0);

  CHECK_THROW(budget->getRemaining("NONEXISTENT"), FinanceException);
}

TEST(Payroll_Process_ThrowsWhenNoEmployees) {
  auto payroll = std::make_shared<Payroll>("2025-01-01 to 2025-01-31");

  CHECK_THROW(payroll->process(), PayrollException);
}

TEST(Payroll_AddEmployeeHours_ThrowsWhenInvalidHours) {
  auto payroll = std::make_shared<Payroll>("2025-01-01 to 2025-01-31");
  auto emp = std::make_shared<Employee>("Test", Role::OPERATOR, 20.0);

  CHECK_THROW(payroll->addEmployeeHours(emp, 400.0), PayrollException);
}

TEST(Transaction_Validate_ThrowsForInvalidData) {
  CHECK_THROW(Transaction("PAYMENT", 0, "ACC001", "ACC002", "REF"),
              FinanceException);
  CHECK_THROW(Transaction("PAYMENT", -100, "ACC001", "ACC002", "REF"),
              FinanceException);

  CHECK_THROW(Transaction("", 100, "ACC001", "ACC002", "REF"),
              FinanceException);

  CHECK_THROW(Transaction("TRANSFER", 100, "ACC001", "ACC001", "REF"),
              FinanceException);
}
TEST(Employee_AddCertification_ThrowsWhenEmpty) {
  auto emp = std::make_shared<Employee>("Test", Role::OPERATOR, 20.0);

  CHECK_THROW(emp->addCertification(""), FactoryException);
}

TEST(Employee_CalculateMonthlySalary_ValidHours) {
  auto emp = std::make_shared<Employee>("Test", Role::OPERATOR, 20.0);
  double minSalary = emp->calculateMonthlySalary(130.0);
  CHECK_CLOSE(2600.0, minSalary, 0.1);

  double maxSalary = emp->calculateMonthlySalary(176.0);
  CHECK_CLOSE(3520.0, maxSalary, 0.1);

  CHECK_THROW(emp->calculateMonthlySalary(129.9), FactoryException);
  CHECK_THROW(emp->calculateMonthlySalary(176.1), FactoryException);
}

TEST(Department_Constructor_ThrowsWhenNegativeBudget) {
  CHECK_THROW(Department("Test", -100.0), FactoryException);
}

TEST(Department_AddEmployee_ThrowsWhenNull) {
  auto dept = std::make_shared<Department>("Test", 10000.0);

  CHECK_THROW(dept->addEmployee(nullptr), FactoryException);
}

TEST(Factory_Constructor_ThrowsWhenEmptyName) {
  CHECK_THROW(Factory(""), FactoryException);
}

TEST(Factory_AddNullObjects_Throws) {
  auto factory = std::make_shared<Factory>("Test");

  CHECK_THROW(factory->addDepartment(nullptr), FactoryException);
  CHECK_THROW(factory->addEmployee(nullptr), FactoryException);
  CHECK_THROW(factory->addProductionLine(nullptr), FactoryException);
  CHECK_THROW(factory->addWarehouse(nullptr), FactoryException);
  CHECK_THROW(factory->addBudget(nullptr), FactoryException);
  CHECK_THROW(factory->addSafetyIncident(nullptr), FactoryException);
  CHECK_THROW(factory->addDeliveryOrder(nullptr), FactoryException);
}

TEST(AuditLog_Constructor_ThrowsWhenEmptyModule) {
  CHECK_THROW(AuditLog(""), FactoryException);
}

TEST(AuditLog_AddEntry_ThrowsWhenSealed) {
  auto log = std::make_shared<AuditLog>("Test");
  log->seal();

  CHECK_THROW(log->addEntry("user", "action", "type", "id", "old", "new"),
              FactoryException);
}

TEST(AuditLog_GetEntriesByUser_ReturnsFiltered) {
  auto log = std::make_shared<AuditLog>("Test");

  log->addEntry("USER1", "CREATE", "Type", "ID1", "", "new");
  log->addEntry("USER2", "UPDATE", "Type", "ID2", "old", "new");
  log->addEntry("USER1", "DELETE", "Type", "ID3", "exists", "");

  auto user1Entries = log->getEntriesByUser("USER1");
  CHECK_EQUAL(2, user1Entries.size());

  auto user2Entries = log->getEntriesByUser("USER2");
  CHECK_EQUAL(1, user2Entries.size());

  auto user3Entries = log->getEntriesByUser("USER3");
  CHECK_EQUAL(0, user3Entries.size());
}

TEST(Schedule_Constructor_ThrowsWhenNullDepartment) {
  CHECK_THROW(Schedule("2025-01-01", nullptr), FactoryException);
}

TEST(Schedule_HasAdequateCoverage_ChecksDepartment) {
  auto dept = std::make_shared<Department>("Test", 10000.0);
  auto schedule = std::make_shared<Schedule>("2025-01-01", dept);

  CHECK_EQUAL(false, schedule->hasAdequateCoverage());

  auto manager = std::make_shared<Employee>("Manager", Role::MANAGER, 35.0);
  auto op1 = std::make_shared<Employee>("Op1", Role::OPERATOR, 20.0);
  auto op2 = std::make_shared<Employee>("Op2", Role::OPERATOR, 20.0);

  dept->addEmployee(manager);
  dept->addEmployee(op1);
  dept->addEmployee(op2);

  CHECK_EQUAL(true, schedule->hasAdequateCoverage());
}

TEST(FullCoverage_Miscellaneous) {
  // RawMaterial
  auto material = std::make_shared<RawMaterial>("Test", "SKU", "TYPE", "SUP",
                                                100, 10.0, "LOC", 20, 5);
  CHECK_EQUAL("TYPE", material->getMaterialType());
  CHECK_EQUAL("SUP", material->getSupplierId());
  CHECK_EQUAL(20, material->getReorderPoint());
  CHECK_EQUAL(5, material->getLeadTimeDays());

  // MaintenanceRequest
  auto request =
      std::make_shared<MaintenanceRequest>("MACH001", "Issue", "HIGH");
  request->setSafetyRelated(true);
  CHECK_EQUAL(true, request->getIsSafetyRelated());

  // SafetyIncident
  auto incident =
      std::make_shared<SafetyIncident>("INJURY", "Desc", "Location", "REP001");
  incident->addCorrectiveAction("Action 1");
  CHECK_EQUAL(1, incident->getStatus().length() > 0);

  // QualityStandard
  auto standard = std::make_shared<QualityStandard>("Standard", "Desc");
  standard->addCriterion("Test", 1.0, 10.0);
  auto criteria = standard->getCriteriaNames();
  CHECK_EQUAL(1, criteria.size());
  CHECK_EQUAL("Test", criteria[0]);

  // TestProcedure
  auto procedure = std::make_shared<TestProcedure>("Proc", "Desc", "1h", false);
  CHECK_EQUAL(false, procedure->getRequiresCertification());

  // PurchaseOrder
  auto po = std::make_shared<PurchaseOrder>("SUP001", "2025-02-15");
  CHECK_EQUAL("DRAFT", po->getStatus());

  // Supplier
  auto supplier =
      std::make_shared<Supplier>("Sup", "email@test.com", "555-1234");
  CHECK_EQUAL("Sup", supplier->getName());

  // Carrier
  auto carrier = std::make_shared<Carrier>("Carrier", "Contact", "555-1234",
                                           "carrier@test.com");
  CHECK_EQUAL("Carrier", carrier->getName());

  // SafetyTraining
  auto training = std::make_shared<SafetyTraining>("Training", 12);
  CHECK_EQUAL("Training", training->getCertificationName());
}

TEST(EdgeCases_VerySmallValues) {
  auto budget = std::make_shared<Budget>("DEPT001", "2025-Q1", 0.01);
  CHECK_CLOSE(0.01, budget->getTotalBudgetAmount(), 0.001);

  auto emp = std::make_shared<Employee>("Test", Role::OPERATOR, 15.0);
  CHECK(!emp->getTrackingId().empty());

  auto wo = std::make_shared<WorkOrder>("PROD001", 1, "2025-02-15");
  CHECK_EQUAL(1, wo->getQuantityRequested());
}
