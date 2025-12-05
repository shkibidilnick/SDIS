#pragma once
#include "FactoryException.h"

// Thrown when quality control operations fail
// Examples: product fails inspection, defect rate too high
class QualityException : public FactoryException {
 public:
  explicit QualityException(const std::string& message)
      : FactoryException("Quality: " + message) {}
};
