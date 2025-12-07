#include "Validator.h"

#include <regex>

bool Validator::validateEmail(const std::string& email) {
  std::regex pattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
  return std::regex_match(email, pattern);
}

bool Validator::validatePhone(const std::string& phone) {
  std::regex pattern(R"(\d{3}-\d{3}-\d{4}|\d{3}-\d{4})");
  return std::regex_match(phone, pattern);
}

void Validator::validatePositive(double value, const std::string& fieldName) {
  if (value <= 0) {
    throw FactoryException(fieldName +
                           " must be positive, got: " + std::to_string(value));
  }
}

bool Validator::validateIdFormat(const std::string& id,
                                 const std::string& expectedPrefix) {
  std::regex pattern(expectedPrefix + R"(_\d{6})");
  return std::regex_match(id, pattern);
}

void Validator::validatePercentage(double value, const std::string& fieldName) {
  if (value < 0 || value > 100) {
    throw FactoryException(fieldName +
                           " must be 0-100, got: " + std::to_string(value));
  }
}
