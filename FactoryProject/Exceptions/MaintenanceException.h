#pragma once
#include "FactoryException.h"

// Thrown when maintenance operations fail
// Examples: technician unavailable, spare part not found
class MaintenanceException : public FactoryException {
 public:
  explicit MaintenanceException(const std::string& message)
      : FactoryException("Maintenance: " + message) {}
};
