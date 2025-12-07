#include "Payroll.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

Payroll::Payroll(const std::string& period)
    : payPeriod(period), isProcessed(false) {
  payrollId = IDGenerator::generate("PAY");

  if (period.empty()) {
    throw PayrollException("Pay period cannot be empty");
  }

  Logger::getInstance().log(
      "Payroll", "Created payroll " + payrollId + " for period: " + period);
}

void Payroll::addEmployeeHours(std::shared_ptr<Employee> employee,
                               double hours) {
  if (isProcessed) {
    throw PayrollException("Cannot modify processed payroll: " + payrollId);
  }

  if (!employee || !employee->getIsActive()) {
    throw PayrollException("Cannot add null or inactive employee to payroll");
  }

  Validator::validatePositive(hours, "Hours worked");

  if (hours > 336) {  // Max hours in a month (24 * 14)
    throw PayrollException("Invalid hours: " + std::to_string(hours));
  }

  employeeHours.push_back({employee, hours});

  Logger::getInstance().log("Payroll", "Added " + employee->getEmployeeId() +
                                           " with " + std::to_string(hours) +
                                           " hours");
}

double Payroll::calculateTotalPayroll() const {
  double total = 0.0;
  for (const auto& pair : employeeHours) {
    total += pair.first->calculateMonthlySalary(pair.second);
  }
  return total;
}

void Payroll::process() {
  if (isProcessed) {
    throw PayrollException("Payroll already processed: " + payrollId);
  }

  if (employeeHours.empty()) {
    throw PayrollException("No employees in payroll: " + payrollId);
  }

  isProcessed = true;

  Logger::getInstance().log("Payroll",
                            "Processed payroll " + payrollId + ". Total: $" +
                                std::to_string(calculateTotalPayroll()));
}

double Payroll::getEmployeePay(const std::string& employeeId) const {
  for (const auto& pair : employeeHours) {
    if (pair.first->getEmployeeId() == employeeId) {
      return pair.first->calculateMonthlySalary(pair.second);
    }
  }
  throw PayrollException("Employee " + employeeId +
                         " not found in payroll: " + payrollId);
}
