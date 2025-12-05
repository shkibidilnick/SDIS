#pragma once
#include <string>

#include "../Exceptions/FactoryException.h"

// Central validation utility for common data formats
// Ensures consistency across all modules
class Validator {
 public:
  // Validates email format (returns true if valid)
  static bool validateEmail(const std::string& email);

  // Validates phone format (XXX-XXX-XXXX)
  static bool validatePhone(const std::string& phone);

  // Validates that value is positive
  static void validatePositive(double value, const std::string& fieldName);

  // Validates that ID follows expected format (PREFIX_123456)
  static bool validateIdFormat(const std::string& id,
                               const std::string& expectedPrefix);

  // Validates percentage (0-100)
  static void validatePercentage(double value, const std::string& fieldName);
};
