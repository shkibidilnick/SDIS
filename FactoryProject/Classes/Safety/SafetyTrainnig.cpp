#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "SafetyTraining.h"

SafetyTraining::SafetyTraining(const std::string& certName, int validityMonths)
    : SafetyIncident("VIOLATION", "Safety training required: " + certName,
                     "Training Room",
                     "TRAINING_SYSTEM"),  // Base class initialization
      certificationName(certName),
      validityPeriodMonths(validityMonths),
      requiresRenewal(true) {
  trainingId = IDGenerator::generate("ST");

  if (certName.empty()) {
    throw SafetyException("Certification name cannot be empty");
  }

  if (validityMonths < 1) {
    throw SafetyException("Validity period must be at least 1 month");
  }

  Logger::getInstance().log(
      "SafetyTraining",
      "Created training program " + trainingId + " for: " + certName);
}

void SafetyTraining::certifyEmployee(const std::string& employeeId) {
  if (employeeId.empty()) {
    throw SafetyException("Employee ID cannot be empty");
  }

  if (std::find(employeeCertifications.begin(), employeeCertifications.end(),
                employeeId) != employeeCertifications.end()) {
    throw SafetyException("Employee " + employeeId + " already certified");
  }

  employeeCertifications.push_back(employeeId);

  // Set expiration date
  auto now = std::chrono::system_clock::now();
  expirationDate = now + std::chrono::hours(24 * 30 * validityPeriodMonths);

  Logger::getInstance().log(
      "SafetyTraining",
      "Certified employee " + employeeId + " for " + certificationName);
}

void SafetyTraining::revokeCertification(const std::string& employeeId) {
  auto it = std::remove(employeeCertifications.begin(),
                        employeeCertifications.end(), employeeId);
  if (it == employeeCertifications.end()) {
    throw SafetyException("Employee " + employeeId + " is not certified");
  }
  employeeCertifications.erase(it, employeeCertifications.end());

  Logger::getInstance().log("SafetyTraining",
                            "Revoked certification for employee " + employeeId);
}

bool SafetyTraining::isEmployeeCertified(const std::string& employeeId) const {
  return std::find(employeeCertifications.begin(), employeeCertifications.end(),
                   employeeId) != employeeCertifications.end();
}

bool SafetyTraining::isCertificationExpired(
    const std::string& employeeId) const {
  if (!isEmployeeCertified(employeeId)) {
    throw SafetyException("Employee " + employeeId + " is not certified");
  }

  auto now = std::chrono::system_clock::now();
  return now > expirationDate;
}

int SafetyTraining::getDaysUntilExpiration(
    const std::string& employeeId) const {
  if (!isEmployeeCertified(employeeId)) {
    throw SafetyException("Employee " + employeeId + " is not certified");
  }

  auto now = std::chrono::system_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::hours>(expirationDate - now)
          .count();

  return static_cast<int>(duration / 24);
}

void SafetyTraining::renewCertification(const std::string& employeeId) {
  if (!isEmployeeCertified(employeeId)) {
    throw SafetyException("Cannot renew: Employee " + employeeId +
                          " not certified");
  }

  // Extend expiration date
  auto now = std::chrono::system_clock::now();
  expirationDate = now + std::chrono::hours(24 * 30 * validityPeriodMonths);

  Logger::getInstance().log("SafetyTraining",
                            "Renewed certification for employee " + employeeId);
}

std::vector<std::string> SafetyTraining::getCertifiedEmployees() const {
  return employeeCertifications;
}
