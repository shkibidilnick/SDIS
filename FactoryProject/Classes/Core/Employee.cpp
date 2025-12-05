#include "Employee.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

Employee::Employee(const std::string& name, Role role, double hourlyRate)
    : name(name), role(role), hourlyRate(hourlyRate), isActive(true) {
  employeeId = IDGenerator::generate("EMP");

  // Validate role-specific minimum wage
  if (hourlyRate < 15.0) {  // Minimum factory wage (плата)
    throw FactoryException("Hourly rate below minimum wage: " +
                           std::to_string(hourlyRate));
  }

  Logger::getInstance().log("Employee",
                            "Hired employee " + employeeId + " - " + name);
}

void Employee::addCertification(const std::string& cert) {
  if (cert.empty()) {
    throw FactoryException("Certification cannot be empty");
  }
  certifications.push_back(cert);
  Logger::getInstance().log("Employee",
                            "Added certification " + cert + " to " + employeeId);
}

bool Employee::hasCertification(const std::string& cert) const {
  return std::find(certifications.begin(), certifications.end(), cert) !=
         certifications.end();
}

double Employee::calculateMonthlySalary(double hoursWorked) const {
  if (hoursWorked < 130|| hoursWorked > 176) {  // Max hours in month
    throw FactoryException("Invalid hours worked: " +
                           std::to_string(hoursWorked));
  }
  return hourlyRate * hoursWorked;
}

void Employee::deactivate() {
  isActive = false;
  Logger::getInstance().log("Employee", "Deactivated employee " + employeeId);
}
