#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IReportable.h"
#include "Employee.h"

// Represents a factory department (e.g., Production, Quality, Maintenance)
// Manages employees and generates department-level reports
class Department : public IReportable {
 private:
  std::string departmentId;
  std::string name;
  std::vector<std::shared_ptr<Employee>>
      employees;  // Using shared_ptr for memory safety
  double budget;
  bool isOperational;

 public:
  Department(const std::string& name, double initialBudget);

  // Adds an employee to the department
  // Validates that employee role matches department needs
  void addEmployee(std::shared_ptr<Employee> employee);

  // Removes employee from department (e.g., transfer or termination)
  void removeEmployee(const std::string& employeeId);

  // Gets total monthly payroll for all active employees
  double calculatePayroll() const;
  double getTotalBudgetAmount() const;

  // Checks if department has required staffing to operate
  bool isFullyStaffed() const;

  // IReportable implementation
  std::string generateReport() const override;
  std::string getReportType() const override { return "DEPARTMENT"; }

  std::string getDepartmentId() const { return departmentId; }
  std::string getName() const { return name; }
  double getBudget() const { return budget; }
};
