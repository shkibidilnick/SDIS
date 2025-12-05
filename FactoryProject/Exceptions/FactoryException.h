#pragma once
#include <stdexcept>
#include <string>

// Base exception class for all factory-related errors
// All specific exceptions inherit from this to provide common error handling
class FactoryException : public std::runtime_error {
 public:
  explicit FactoryException(const std::string& message)
      : std::runtime_error("Factory Error: " + message) {}
};
