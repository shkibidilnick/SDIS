#pragma once
#include "FactoryException.h"

// Thrown when payroll processing fails
// Examples: incorrect hours, rate mismatch, payment calculation error
class PayrollException : public FactoryException {
 public:
  explicit PayrollException(const std::string& message)
      : FactoryException("Payroll: " + message) {}
};
