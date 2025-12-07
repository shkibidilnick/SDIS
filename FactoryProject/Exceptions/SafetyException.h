#pragma once
#include "FactoryException.h"

// Thrown when safety protocols are violated or incidents occur
// Examples: missing PPE, safety training expired, incident reporting
class SafetyException : public FactoryException {
 public:
  explicit SafetyException(const std::string& message)
      : FactoryException("Safety: " + message) {}
};
