#pragma once
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FactoryException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"
#include "Role.h"

// Represents a factory employee with tracking capabilities
// Contains personal info, role, and work history
class Employee : public ITrackable {
 private:
  std::string employeeId;
  std::string name;
  Role role;
  double hourlyRate;
  bool isActive;
  std::vector<std::string> certifications;  // Required for certain roles

 public:
  // Creates a new employee with auto-generated ID
  Employee(const std::string& name, Role role, double hourlyRate);

  // Adds a certification to the employee's record
  // Required for operating specific machines or quality inspection
  void addCertification(const std::string& cert);

  // Checks if employee has required certification
  bool hasCertification(const std::string& cert) const;

  // Gets total monthly salary based on hours worked
  double calculateMonthlySalary(double hoursWorked) const;

  // Deactivates employee (e.g., when they leave the company)
  void deactivate();

  // ITrackable implementation
  std::string getTrackingId() const override { return employeeId; }
  std::string getStatus() const override {
    return isActive ? "ACTIVE" : "INACTIVE";
  }

  std::string getEmployeeId() const { return employeeId; }
  std::string getName() const { return name; }
  Role getRole() const { return role; }
  bool getIsActive() const { return isActive; }
};
