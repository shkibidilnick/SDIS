#include "Machine.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

Machine::Machine(const std::string& code, const std::string& name,
                 const std::string& machineType, double rate, int maintInterval)
    : machineCode(code),
      name(name),
      type(machineType),
      hourlyRate(rate),
      isOperational(true),
      hoursSinceLastMaintenance(0),
      maintenanceIntervalHours(maintInterval) {
  if (code.empty() || name.empty()) {
    throw MaintenanceException("Machine code and name are required");
  }

  if (maintenanceIntervalHours <= 0) {
    throw MaintenanceException("Maintenance interval must be positive");
  }

  machineId = IDGenerator::generate("MACH");
  Logger::getInstance().log("Machine",
                            "Created machine " + machineId + " - " + code);
}

bool Machine::isOperatorCertified(
    const std::shared_ptr<Employee>& employee) const {
  if (!employee) return false;

  for (const auto& cert : requiredCertifications) {
    if (!employee->hasCertification(cert)) {
      return false;
    }
  }
  return true;
}

void Machine::recordOperationHours(int hours) {
  if (hours < 0) {
    throw MaintenanceException("Operation hours cannot be negative");
  }

  hoursSinceLastMaintenance += hours;

  if (isMaintenanceDue()) {
    Logger::getInstance().log("Machine", "Maintenance due for " + machineId);
    setUnderMaintenance();
  }
}

void Machine::setUnderMaintenance() {
  isOperational = false;
  Logger::getInstance().log("Machine",
                            "Machine " + machineId + " under maintenance");
}

void Machine::setOperational() {
  isOperational = true;
  hoursSinceLastMaintenance = 0;  // Reset counter after maintenance
  Logger::getInstance().log("Machine",
                            "Machine " + machineId + " back to operational");
}

bool Machine::isMaintenanceDue() const {
  return hoursSinceLastMaintenance >= maintenanceIntervalHours;
}

void Machine::validate() const {
  if (hourlyRate < 0) {
    throw MaintenanceException("Machine hourly rate cannot be negative");
  }

  if (maintenanceIntervalHours < 50) {
    throw MaintenanceException("Maintenance interval too short");
  }
}
