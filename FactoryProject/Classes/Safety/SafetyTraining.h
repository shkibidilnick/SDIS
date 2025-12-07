#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Employee.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/SafetyException.h"
#include "SafetyIncident.h"

// Represents a safety training requirement or certification
// Tracks employee training status and expiration dates
class SafetyTraining : public SafetyIncident {
 private:
  std::string trainingId;
  std::string certificationName;
  std::vector<std::string> employeeCertifications;  // Employee IDs with cert
  std::unordered_map<std::string, std::chrono::system_clock::time_point>
      employeeExpirations;  // Employee ID -> Expiration date
  int validityPeriodMonths;
  bool requiresRenewal;

 public:
  SafetyTraining(const std::string& certName, int validityMonths);

  // Grants certification to an employee
  void certifyEmployee(const std::string& employeeId);

  // Revokes certification from an employee
  void revokeCertification(const std::string& employeeId);

  // Checks if an employee's certification is current
  bool isEmployeeCertified(const std::string& employeeId) const;

  // Checks if certification is expired for an employee
  bool isCertificationExpired(const std::string& employeeId) const;

  // Gets days until expiration for an employee
  int getDaysUntilExpiration(const std::string& employeeId) const;

  // Renews certification for an employee
  void renewCertification(const std::string& employeeId);

  // Gets all certified employees
  std::vector<std::string> getCertifiedEmployees() const;

  // ITrackable implementation
  std::string getTrackingId() const override { return trainingId; }
  std::string getStatus() const override {
    // Проверяем состояние сертификации
    if (employeeCertifications.empty()) {
      return "NO_CERTIFICATIONS";
    }
    return "ACTIVE";
  }

  std::string getTrainingId() const { return trainingId; }
  std::string getCertificationName() const { return certificationName; }
  int getValidityPeriodMonths() const { return validityPeriodMonths; }
  bool getRequiresRenewal() const { return requiresRenewal; }

  // Новый метод для установки даты истечения (для тестирования)
  void setExpirationDateForTesting(
      const std::string& employeeId,
      const std::chrono::system_clock::time_point& date) {
    employeeExpirations[employeeId] = date;
  }
};
