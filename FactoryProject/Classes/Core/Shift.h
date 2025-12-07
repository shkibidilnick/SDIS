#pragma once
#include <memory>
#include <string>

#include "Employee.h"

// Represents a single work shift for an employee
// Calculates labor cost and tracks hours worked
class Shift {
 private:
  std::string shiftId;
  std::string employeeId;
  std::string date;
  double startHour;  // 24-hour format, e.g., 8.0 = 8:00 AM
  double endHour;
  bool isApproved;

 public:
  Shift(const std::string& employeeId, const std::string& date,
        double startHour, double endHour);

  // Calculates total hours of this shift
  double getHours() const { return endHour - startHour; }

  // Calculates labor cost for this shift
  // Requires: Employee reference to get hourly rate
  double calculateCost() const;

  // Approves the shift for payroll processing
  void approve() { isApproved = true; }

  void setApproved(bool approved) { isApproved = approved; }

  std::string getShiftId() const { return shiftId; }
  std::string getEmployeeId() const { return employeeId; }
  std::string getDate() const { return date; }
  bool getIsApproved() const { return isApproved; }
};
