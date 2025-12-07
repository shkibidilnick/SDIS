#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/QualityExceptionh.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IValidatable.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/Product.h"
#include "QualityStandard.h"

// Represents a quality inspection/check
// Records measurements and determines pass/fail status
class QualityCheck : public ITrackable, public IValidatable {
 private:
  std::string checkId;
  std::string productId;
  std::string standardId;
  std::vector<std::pair<std::string, double>> measurements;  // Criterion, Value
  bool isPassed;
  std::string inspectorId;
  std::string checkDate;
  std::string notes;

 public:
  QualityCheck(const std::string& productId, const std::string& standardId,
               const std::string& inspectorId);

  // Records a measurement for a specific criterion
  void recordMeasurement(const std::string& criterionName, double value);

  // Performs the quality check against the standard
  void performCheck(const QualityStandard& standard);

  // Gets a summary of all measurements
  std::string getMeasurementSummary() const;

  // ITrackable implementation
  std::string getTrackingId() const override { return checkId; }
  std::string getStatus() const override {
    return isPassed ? "PASSED" : "FAILED";
  }

  // IValidatable implementation
  void validate() const override;

  std::string getCheckId() const { return checkId; }
  std::string getProductId() const { return productId; }
  bool getIsPassed() const { return isPassed; }
  std::string getInspectorId() const { return inspectorId; }
  void setNotes(const std::string& n) { notes = n; }
};
