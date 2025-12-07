#include "Department.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

Department::Department(const std::string& name, double initialBudget)
    : name(name), budget(initialBudget), isOperational(true) {
  departmentId = IDGenerator::generate("DEPT");

  if (initialBudget < 0) {
    throw FactoryException("Department budget cannot be negative");
  }

  Logger::getInstance().log("Department", "Created department " + departmentId);
}

void Department::addEmployee(std::shared_ptr<Employee> employee) {
  if (!employee || !employee->getIsActive()) {
    throw FactoryException("Cannot add non existent or inactive employee");
  }
  employees.push_back(employee);
  Logger::getInstance().log(
      "Department",
      "Added employee " + employee->getEmployeeId() + " to " + departmentId);
}

void Department::removeEmployee(const std::string& employeeId) {
  auto it = std::remove_if(employees.begin(), employees.end(),
                           [&employeeId](const std::shared_ptr<Employee>& emp) {
                             return emp->getEmployeeId() == employeeId;
                           });

  if (it != employees.end()) {
    employees.erase(it, employees.end());
    Logger::getInstance().log("Department", "Removed employee " + employeeId);
  }
}

double Department::calculatePayroll() const {
  double total = 0.0;
  for (const auto& emp : employees) {
    if (emp->getIsActive()) {
      total += emp->calculateMonthlySalary(160);  // Assume 160 hours/month
    }
  }
  return total;
}

bool Department::isFullyStaffed() const {
  // Minimum staffing logic: at least 1 manager and 2 operators per department
  int managers = 0;
  int operators = 0;

  for (const auto& emp : employees) {
    if (emp->getIsActive()) {
      if (emp->getRole() == Role::MANAGER) managers++;
      if (emp->getRole() == Role::OPERATOR) operators++;
    }
  }

  return managers >= 1 && operators >= 2;
}

std::string Department::generateReport() const {
  std::string report = "Department Report: " + name + "\n";
  report += "ID: " + departmentId + "\n";
  report += "Budget: $" + std::to_string(budget) + "\n";
  report += "Payroll: $" + std::to_string(calculatePayroll()) + "\n";
  report += "Employees: " + std::to_string(employees.size()) + "\n";
  report +=
      "Fully Staffed: " + std::string(isFullyStaffed() ? "YES" : "NO") + "\n";

  // Добавить список сотрудников
  report += "Employee IDs: ";
  for (const auto& emp : employees) {
    if (emp && emp->getIsActive()) {
      report += emp->getEmployeeId() + " ";
    }
  }
  report += "\n";

  return report;
}
