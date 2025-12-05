#pragma once
#include "FactoryException.h"

// Thrown when production-related operations fail
// Examples: machine breakdown, work order delays, quality check failures
class ProductionException : public FactoryException {
 public:
  explicit ProductionException(const std::string& message)
      : FactoryException("Production: " + message) {}
};
