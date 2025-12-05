#include "QualityStandard.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

QualityStandard::QualityStandard(const std::string& nm, const std::string& desc)
    : name(nm), description(desc), isActive(true) {
  standardId = IDGenerator::generate("QS");

  if (name.empty()) {
    throw QualityException("Quality standard name is required");
  }

  Logger::getInstance().log("QualityStandard",
                            "Created standard " + standardId + " - " + name);
}

void QualityStandard::addCriterion(const std::string& critName, double minVal,
                                   double maxVal) {
  if (critName.empty()) {
    throw QualityException("Criterion name cannot be empty");
  }

  if (minVal > maxVal) {
    throw QualityException("Min value cannot exceed max value for " + critName);
  }

  criteria.push_back({critName, {minVal, maxVal}});

  Logger::getInstance().log(
      "QualityStandard", "Added criterion to " + standardId + ": " + critName);
}

bool QualityStandard::isMeasurementValid(const std::string& critName,
                                         double value) const {
  for (const auto& crit : criteria) {
    if (crit.first == critName) {
      double minVal = crit.second.first;
      double maxVal = crit.second.second;
      return value >= minVal && value <= maxVal;
    }
  }
  throw QualityException("Criterion " + critName + " not found in standard " +
                         name);
}

std::vector<std::string> QualityStandard::getCriteriaNames() const {
  std::vector<std::string> names;
  for (const auto& crit : criteria) {
    names.push_back(crit.first);
  }
  return names;
}

void QualityStandard::validate() const {
  if (criteria.empty()) {
    throw QualityException("Quality standard must have at least one criterion");
  }

  for (const auto& crit : criteria) {
    if (crit.second.first > crit.second.second) {
      throw QualityException("Invalid range for criterion: " + crit.first);
    }
  }
}
