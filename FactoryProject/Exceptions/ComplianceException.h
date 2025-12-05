#pragma once
#include "FactoryException.h";

// Thrown when regulatory compliance is violated
// Examples: missing certifications, failed audit, expired licenses
class ComplianceException : public FactoryException {
 public:
  explicit ComplianceException(const std::string& message)
      : FactoryException("Compliance: " + message) {}
};
