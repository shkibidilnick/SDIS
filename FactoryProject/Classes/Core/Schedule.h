#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FactoryException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IValidatable.h"
#include "Employee.h"
#include "Shift.h"
#include "Department.h"

// Manages employee shifts and work schedules
// Ensures proper coverage and prevents scheduling conflicts
class Schedule : public IValidatable {
 private:
  std::string scheduleId;
  std::vector<Shift> shifts;
  std::string weekStartDate;
  std::shared_ptr<Department> department;

 public:
  Schedule(const std::string& weekStartDate,
           std::shared_ptr<Department> department);

  // Adds a shift to the schedule
  // Throws FactoryException if employee already has shift that day
  void addShift(const Shift& shift);

  // Gets all shifts for a specific employee
  std::vector<Shift> getShiftsForEmployee(const std::string& employeeId) const;

  // Checks if schedule has coverage for all required positions
  bool hasAdequateCoverage() const;

  // Gets total labor cost for this schedule
  double calculateLaborCost() const;

  // IValidatable implementation
  void validate() const override;

  std::string getScheduleId() const { return scheduleId; }
  const std::vector<Shift>& getShifts() const { return shifts; }
};
