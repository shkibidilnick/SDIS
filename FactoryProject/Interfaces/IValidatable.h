#pragma once

// Interface for objects that require validation before processing
// Ensures data integrity and business rule compliance
class IValidatable {
 public:
  // Validates the object's current state
  // Throws appropriate exception if validation fails
  virtual void validate() const = 0;
  virtual ~IValidatable() = default;
};
