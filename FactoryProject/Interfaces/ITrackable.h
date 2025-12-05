#pragma once
#include <string>

// Interface for objects that need to be tracked through the factory system
// Provides unique identification and audit trail capabilities
class ITrackable {
 public:
  virtual std::string getTrackingId() const = 0;
  virtual std::string getStatus() const = 0;
  virtual ~ITrackable() = default;
};
