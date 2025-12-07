#pragma once
#include <string>

// Interface for objects that can generate reports
// Allows unified reporting across different system modules
class IReportable {
 public:
  // Generates and returns a text-based report
  virtual std::string generateReport() const = 0;

  // Gets the type of report this object produces
  virtual std::string getReportType() const = 0;
  virtual ~IReportable() = default;
};
