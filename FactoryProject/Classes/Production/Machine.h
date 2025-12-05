#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Employee.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/MaintenanceException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IValidatable.h"

// Represents a production machine or equipment
// Tracks operational status and maintenance history
class Machine : public ITrackable, public IValidatable {
 private:
  std::string machineId;
  std::string machineCode;
  std::string name;
  std::string type;  // e.g., "CNC", "Assembly", "Welding"
  bool isOperational;
  double hourlyRate;                                // Operating cost per hour
  std::vector<std::string> requiredCertifications;  // For operators
  int hoursSinceLastMaintenance;
  int maintenanceIntervalHours;

 public:
  Machine(const std::string& machineCode, const std::string& name,
          const std::string& type, double hourlyRate, int maintenanceInterval);

  // Checks if employee is certified to operate this machine
  bool isOperatorCertified(const std::shared_ptr<Employee>& employee) const;

  // Records hours of operation and checks if maintenance is due
  void recordOperationHours(int hours);

  // Marks machine as under maintenance
  void setUnderMaintenance();

  // Marks machine as operational after maintenance
  void setOperational();

  // Checks if maintenance is overdue
  bool isMaintenanceDue() const;

  // ITrackable implementation
  std::string getTrackingId() const override { return machineId; }
  std::string getStatus() const override {
    return isOperational ? "OPERATIONAL" : "MAINTENANCE";
  }

  // IValidatable implementation
  void validate() const override;

  std::string getMachineId() const { return machineId; }
  std::string getMachineCode() const { return machineCode; }
  double getHourlyRate() const { return hourlyRate; }
  bool getIsOperational() const { return isOperational; }
  int getHoursSinceLastMaintenance() const { return hoursSinceLastMaintenance; }
};
