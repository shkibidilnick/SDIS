#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Employee.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/PayrollException.h"

// Manages employee payroll processing
// Calculates wages, deductions, and generates pay records
class Payroll {
 private:
  std::string payrollId;
  std::string payPeriod;  // e.g., "2025-01-01 to 2025-01-31"
  std::vector<std::pair<std::shared_ptr<Employee>, double>>
      employeeHours;  // Employee + Hours worked
  bool isProcessed;

 public:
  Payroll(const std::string& period);

  // Adds employee and their hours for this pay period
  void addEmployeeHours(std::shared_ptr<Employee> employee, double hours);

  // Calculates total payroll amount for all employees
  double calculateTotalPayroll() const;

  // Processes payroll (marks as paid)
  void process();

  // Gets pay details for a specific employee
  double getEmployeePay(const std::string& employeeId) const;

  std::string getPayrollId() const { return payrollId; }
  std::string getPayPeriod() const { return payPeriod; }
  bool getIsProcessed() const { return isProcessed; }
};
