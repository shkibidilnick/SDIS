#include <UnitTest++/UnitTest++.h>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Department.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Employee.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Factory.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Role.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Schedule.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Shift.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FactoryException.h"
#include "test_helpers.h"

// ==================== Тесты Employee ====================
TEST(Employee_Constructor_CreatesEmployeeWithCorrectProperties) {
  Employee employee("John Doe", Role::MANAGER, 25.0);

  CHECK(!employee.getEmployeeId().empty());
  // У Employee нет getName(), используем getEmployeeId() для проверки
  CHECK_EQUAL(Role::MANAGER, employee.getRole());
  CHECK_EQUAL(true, employee.getIsActive());
}

TEST(Employee_Constructor_ThrowsWhenRateBelowMinimum) {
  CHECK_THROW(Employee("Jane Smith", Role::OPERATOR, 10.0), FactoryException);
}

TEST(Employee_AddCertification_AddsCertificationToEmployee) {
  Employee employee("Bob Wilson", Role::TECHNICIAN, 20.0);

  employee.addCertification("CNC_Operator");

  CHECK_EQUAL(true, employee.hasCertification("CNC_Operator"));
  CHECK_EQUAL(false, employee.hasCertification("Welding"));
}

TEST(Employee_CalculateMonthlySalary_ReturnsCorrectAmount) {
  Employee employee("Alice Brown", Role::ENGINEER, 30.0);

  double salary = employee.calculateMonthlySalary(160.0);

  CHECK_CLOSE(4800.0, salary, 0.01);
}

TEST(Employee_CalculateMonthlySalary_ThrowsForInvalidHours) {
  Employee employee("Charlie Davis", Role::OPERATOR, 18.0);

  CHECK_THROW(employee.calculateMonthlySalary(120.0), FactoryException);
  CHECK_THROW(employee.calculateMonthlySalary(200.0), FactoryException);
}

TEST(Employee_Deactivate_SetsEmployeeAsInactive) {
  Employee employee("Diana Evans", Role::QUALITY_INSPECTOR, 22.0);

  employee.deactivate();

  CHECK_EQUAL(false, employee.getIsActive());
}

// ==================== Тесты Department ====================
TEST(Department_Constructor_CreatesDepartmentWithCorrectProperties) {
  Department dept("Production", 100000.0);

  CHECK(!dept.getDepartmentId().empty());
  CHECK_EQUAL("Production", dept.getName());
  CHECK_EQUAL(100000.0, dept.getBudget());
  // isOperational - private, проверяем через generateReport
  CHECK(dept.generateReport().find("Production") != std::string::npos);
}

TEST(Department_AddEmployee_AddsActiveEmployee) {
  Department dept("Quality Control", 75000.0);
  auto employee =
      std::make_shared<Employee>("Eve Foster", Role::QUALITY_INSPECTOR, 24.0);

  dept.addEmployee(employee);

  // У Department нет getEmployeeCount(), проверяем через payroll
  CHECK(dept.generateReport().find("Employees: 1") != std::string::npos);
}

TEST(Department_AddEmployee_ThrowsForNullOrInactive) {
  Department dept("Maintenance", 50000.0);
  auto inactiveEmp =
      std::make_shared<Employee>("Frank Green", Role::MAINTENANCE_STAFF, 19.0);
  inactiveEmp->deactivate();

  CHECK_THROW(dept.addEmployee(nullptr), FactoryException);
  CHECK_THROW(dept.addEmployee(inactiveEmp), FactoryException);
}

TEST(Department_CalculatePayroll_ReturnsTotalSalary) {
  Department dept("Assembly", 150000.0);
  auto emp1 = std::make_shared<Employee>("Grace Hall", Role::OPERATOR, 18.0);
  auto emp2 = std::make_shared<Employee>("Henry Ivy", Role::OPERATOR, 18.0);

  dept.addEmployee(emp1);
  dept.addEmployee(emp2);

  double payroll = dept.calculatePayroll();

  CHECK_CLOSE(5760.0, payroll, 0.01);  // 2 * 18 * 160
}

TEST(Department_IsFullyStaffed_ReturnsCorrectStatus) {
  Department dept("Production", 100000.0);

  // Минимум: 1 менеджер и 2 оператора
  CHECK_EQUAL(false, dept.isFullyStaffed());

  auto manager = std::make_shared<Employee>("Ivan Jones", Role::MANAGER, 35.0);
  auto op1 = std::make_shared<Employee>("Julia King", Role::OPERATOR, 18.0);

  dept.addEmployee(manager);
  dept.addEmployee(op1);

  CHECK_EQUAL(false, dept.isFullyStaffed());

  auto op2 = std::make_shared<Employee>("Kevin Lee", Role::OPERATOR, 18.0);
  dept.addEmployee(op2);

  CHECK_EQUAL(true, dept.isFullyStaffed());
}

TEST(Department_GenerateReport_CreatesReportString) {
  Department dept("Testing", 80000.0);
  auto employee =
      std::make_shared<Employee>("Laura Moore", Role::TECHNICIAN, 21.0);
  dept.addEmployee(employee);

  std::string report = dept.generateReport();

  CHECK(report.find("Testing") != std::string::npos);
  CHECK(report.find("80000") != std::string::npos);
}

// ==================== Тесты Factory ====================
TEST(Factory_Constructor_CreatesFactoryWithCorrectProperties) {
  Factory factory("Tech Manufacturing Plant");

  CHECK(!factory.getFactoryId().empty());
  CHECK_EQUAL("Tech Manufacturing Plant", factory.getFactoryName());
  CHECK_EQUAL(0, factory.getTotalEmployeeCount());
  CHECK_EQUAL(0, factory.getTotalDepartmentCount());
}

TEST(Factory_AddDepartment_AddsDepartmentToFactory) {
  Factory factory("Auto Parts Factory");
  auto dept = std::make_shared<Department>("Engine Assembly", 200000.0);

  factory.addDepartment(dept);

  CHECK_EQUAL(1, factory.getTotalDepartmentCount());
  CHECK_EQUAL(dept, factory.findDepartment(dept->getDepartmentId()));
}

TEST(Factory_AddEmployee_AddsEmployeeToFactory) {
  Factory factory("Electronics Factory");
  auto employee =
      std::make_shared<Employee>("Mike North", Role::ENGINEER, 28.0);

  factory.addEmployee(employee);

  CHECK_EQUAL(1, factory.getTotalEmployeeCount());
  CHECK_EQUAL(employee, factory.findEmployee(employee->getEmployeeId()));
}

TEST(Factory_AssignEmployeeToDepartment_AssignsCorrectly) {
  Factory factory("Metal Works");
  auto dept = std::make_shared<Department>("Machining", 120000.0);
  auto employee =
      std::make_shared<Employee>("Nancy Owens", Role::TECHNICIAN, 23.0);

  factory.addDepartment(dept);
  factory.addEmployee(employee);
  factory.assignEmployeeToDepartment(employee->getEmployeeId(),
                                     dept->getDepartmentId());

  // Проверяем через отчет
  CHECK(dept->generateReport().find(employee->getEmployeeId()) !=
        std::string::npos);
}

TEST(Factory_GetTotalBudget_ReturnsSumOfAllBudgets) {
  Factory factory("Multi-Dept Factory");
  auto budget1 = std::make_shared<Budget>("D001", "2025-Q1", 100000.0);
  auto budget2 = std::make_shared<Budget>("D002", "2025-Q1", 150000.0);

  factory.addBudget(budget1);
  factory.addBudget(budget2);

  CHECK_CLOSE(250000.0, factory.getTotalBudget(), 0.01);
}

TEST(Factory_GenerateFactoryReport_CreatesComprehensiveReport) {
  Factory factory("Test Factory");
  auto dept = std::make_shared<Department>("Production", 100000.0);
  auto employee = std::make_shared<Employee>("Oscar Pine", Role::MANAGER, 35.0);

  factory.addDepartment(dept);
  factory.addEmployee(employee);

  std::string report = factory.generateFactoryReport();

  CHECK(report.find("Test Factory") != std::string::npos);
  CHECK(report.find("Departments: 1") != std::string::npos);
  CHECK(report.find("Employees: 1") != std::string::npos);
}

// ==================== Тесты Shift ====================
TEST(Shift_Constructor_CreatesShiftWithCorrectProperties) {
  Shift shift("EMP001", "2025-01-15", 8.0, 16.5);

  CHECK(!shift.getShiftId().empty());
  CHECK_EQUAL("EMP001", shift.getEmployeeId());
  CHECK_EQUAL(8.5, shift.getHours());
  CHECK_EQUAL(false, shift.getIsApproved());
}

TEST(Shift_Constructor_ThrowsForInvalidHours) {
  CHECK_THROW(Shift("EMP001", "2025-01-15", -1, 16.0), FactoryException);
  CHECK_THROW(Shift("EMP001", "2025-01-15", 16.0, 8.0), FactoryException);
  CHECK_THROW(Shift("EMP001", "2025-01-15", 25.0, 26.0), FactoryException);
}

TEST(Shift_CalculateCost_ReturnsCorrectCost) {
  Shift shift("EMP001", "2025-01-15", 8.0, 16.0);

  double cost = shift.calculateCost();

  CHECK_CLOSE(120.0, cost, 0.01);  // 8 hours * $15/hour
}

TEST(Shift_Approve_SetsApprovedStatus) {
  Shift shift("EMP001", "2025-01-15", 8.0, 16.0);

  shift.approve();

  CHECK_EQUAL(true, shift.getIsApproved());
}

// ==================== Тесты Schedule ====================
TEST(Schedule_Constructor_CreatesScheduleWithCorrectProperties) {
  auto dept = std::make_shared<Department>("Production", 100000.0);
  Schedule schedule("2025-01-13", dept);

  CHECK(!schedule.getScheduleId().empty());
  CHECK_EQUAL(0, schedule.getShifts().size());
}

TEST(Schedule_AddShift_AddsShiftToSchedule) {
  auto dept = std::make_shared<Department>("Production", 100000.0);
  Schedule schedule("2025-01-13", dept);

  Shift shift1("EMP001", "2025-01-13", 8.0, 16.0);
  Shift shift2("EMP002", "2025-01-13", 8.0, 16.0);

  schedule.addShift(shift1);
  schedule.addShift(shift2);

  CHECK_EQUAL(2, schedule.getShifts().size());
}

TEST(Schedule_AddShift_ThrowsForDuplicateEmployeeOnSameDay) {
  auto dept = std::make_shared<Department>("Production", 100000.0);
  Schedule schedule("2025-01-13", dept);

  Shift shift1("EMP001", "2025-01-13", 8.0, 16.0);
  Shift shift2("EMP001", "2025-01-13", 16.0, 24.0);

  schedule.addShift(shift1);
  CHECK_THROW(schedule.addShift(shift2), FactoryException);
}

TEST(Schedule_CalculateLaborCost_ReturnsTotalCost) {
  auto dept = std::make_shared<Department>("Production", 100000.0);
  Schedule schedule("2025-01-13", dept);

  Shift shift1("EMP001", "2025-01-13", 8.0, 16.0);
  Shift shift2("EMP002", "2025-01-13", 8.0, 16.0);

  schedule.addShift(shift1);
  schedule.addShift(shift2);

  double cost = schedule.calculateLaborCost();

  CHECK_CLOSE(240.0, cost, 0.01);  // 2 shifts * 8 hours * $15/hour
}

TEST(Schedule_Validate_ThrowsWhenNoShifts) {
  auto dept = std::make_shared<Department>("Production", 100000.0);
  Schedule schedule("2025-01-13", dept);

  CHECK_THROW(schedule.validate(), FactoryException);
}

TEST(Schedule_HasAdequateCoverage_ReturnsCorrectStatus) {
  auto dept = std::make_shared<Department>("Production", 100000.0);
  Schedule schedule("2025-01-13", dept);

  CHECK_EQUAL(false, schedule.hasAdequateCoverage());

  auto manager = std::make_shared<Employee>("Paul Quinn", Role::MANAGER, 35.0);
  auto op1 = std::make_shared<Employee>("Rachel Reed", Role::OPERATOR, 18.0);
  auto op2 = std::make_shared<Employee>("Sam Turner", Role::OPERATOR, 18.0);

  dept->addEmployee(manager);
  dept->addEmployee(op1);
  dept->addEmployee(op2);

  CHECK_EQUAL(true, schedule.hasAdequateCoverage());
}
