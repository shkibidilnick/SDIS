#include "QualityCheck.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

QualityCheck::QualityCheck(const std::string& prodId, const std::string& stdId,
                           const std::string& inspId)
    : productId(prodId),
      standardId(stdId),
      inspectorId(inspId),
      isPassed(false),
      notes("") {
  checkId = IDGenerator::generate("QC");

  if (prodId.empty() || stdId.empty() || inspId.empty()) {
    throw QualityException("Product, standard, and inspector IDs are required");
  }

  checkDate = DateTimeUtils::getCurrentTimestamp();

  Logger::getInstance().log("QualityCheck", "Created quality check " + checkId);
}

void QualityCheck::recordMeasurement(const std::string& critName,
                                     double value) {
  if (critName.empty()) {
    throw QualityException("Criterion name cannot be empty");
  }

  measurements.push_back({critName, value});

  Logger::getInstance().log(
      "QualityCheck",
      "Recorded measurement: " + critName + " = " + std::to_string(value));
}

void QualityCheck::performCheck(const QualityStandard& standard) {
  if (measurements.empty()) {
    throw QualityException("Cannot perform check without measurements");
  }

  bool allPassed = true;
  std::string failedCriteria;

  for (const auto& measurement : measurements) {
    const std::string& critName = measurement.first;
    double value = measurement.second;

    if (!standard.isMeasurementValid(critName, value)) {
      allPassed = false;
      failedCriteria += critName + " ";
    }
  }

  isPassed = allPassed;

  Logger::getInstance().log("QualityCheck",
                            "Check " + checkId + " result: " +
                                std::string(isPassed ? "PASSED" : "FAILED"));

  if (!isPassed) {
    Logger::getInstance().log("QualityCheck",
                              "Failed criteria: " + failedCriteria);
  }
}

std::string QualityCheck::getMeasurementSummary() const {
  std::string summary = "Quality Check " + checkId + " Measurements:\n";
  for (const auto& m : measurements) {
    summary += "  " + m.first + ": " + std::to_string(m.second) + "\n";
  }
  return summary;
}

void QualityCheck::validate() const {
  if (productId.empty() || standardId.empty() || inspectorId.empty()) {
    throw QualityException("All IDs are required");
  }

  if (measurements.empty()) {
    throw QualityException("At least one measurement is required");
  }
}
