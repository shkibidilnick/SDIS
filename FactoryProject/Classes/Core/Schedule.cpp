#include "Schedule.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

Schedule::Schedule(const std::string& weekStartDate, std::shared_ptr<Department> dept)
    : weekStartDate(weekStartDate), department(dept) {
    scheduleId = IDGenerator::generate("SCHED");

    if (!dept) {
        throw FactoryException("Schedule must be assigned to a valid department");
    }

    Logger::getInstance().log("Schedule", "Created schedule " + scheduleId + " for " + dept->getName());
}

void Schedule::addShift(const Shift& shift) {
    // Check for conflicts
    for (const auto& existingShift : shifts) {
        if (existingShift.getEmployeeId() == shift.getEmployeeId() &&
            existingShift.getDate() == shift.getDate()) {
            throw FactoryException("Employee " + shift.getEmployeeId() + " already has shift on " + shift.getDate());
        }
    }
    shifts.push_back(shift);
}

std::vector<Shift> Schedule::getShiftsForEmployee(const std::string& employeeId) const {
    std::vector<Shift> employeeShifts;
    for (const auto& shift : shifts) {
        if (shift.getEmployeeId() == employeeId) {
            employeeShifts.push_back(shift);
        }
    }
    return employeeShifts;
}

bool Schedule::hasAdequateCoverage() const {
    if (!department) return false;
    return department->isFullyStaffed();
}

double Schedule::calculateLaborCost() const {
    double totalCost = 0.0;
    for (const auto& shift : shifts) {
        totalCost += shift.calculateCost();
    }
    return totalCost;
}

void Schedule::validate() const {
    if (shifts.empty()) {
        throw FactoryException("Schedule has no shifts");
    }

    if (!hasAdequateCoverage()) {
        throw FactoryException("Schedule does not have adequate coverage");
    }

    if (calculateLaborCost() > department->getBudget()) {
        throw FactoryException("Schedule exceeds department budget");
    }
}