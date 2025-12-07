#include "Defect.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

Defect::Defect(const std::string& productId, const std::string& standardId,
               const std::string& inspectorId, const std::string& type,
               int qtyAffected)
    : QualityCheck(productId, standardId, inspectorId),
      defectType(type),
      quantityAffected(qtyAffected),
      isResolved(false) {
  defectId = IDGenerator::generate("DEF");

  if (type != "DIMENSIONAL" && type != "COSMETIC" && type != "FUNCTIONAL") {
    throw QualityException("Invalid defect type: " + type);
  }

  if (qtyAffected <= 0) {
    throw QualityException("Quantity affected must be positive");
  }

  // Default severity based on type
  if (type == "FUNCTIONAL")
    severity = "CRITICAL";
  else if (type == "DIMENSIONAL")
    severity = "MAJOR";
  else
    severity = "MINOR";

  Logger::getInstance().log("Defect",
                            "Created defect " + defectId + " of type: " + type);
}

void Defect::setRootCause(const std::string& cause) {
  if (cause.empty()) {
    throw QualityException("Root cause cannot be empty");
  }
  rootCause = cause;

  Logger::getInstance().log("Defect", "Root cause set for defect " + defectId);
}

void Defect::addCorrectiveAction(const std::string& action) {
  if (action.empty()) {
    throw QualityException("Corrective action cannot be empty");
  }
  correctiveActions.push_back(action);

  Logger::getInstance().log("Defect",
                            "Added corrective action to defect " + defectId);
}

void Defect::markAsResolved() {
  if (correctiveActions.empty()) {
    throw QualityException("Cannot resolve defect without corrective actions");
  }
  isResolved = true;

  Logger::getInstance().log("Defect",
                            "Marked defect " + defectId + " as RESOLVED");
}

double Defect::calculateCostImpact() const {
  // Simplified calculation: quantity * unit cost * severity multiplier
  double baseCostPerUnit = 100.0;  // Placeholder
  double severityMultiplier = (severity == "CRITICAL") ? 5.0
                              : (severity == "MAJOR")  ? 2.0
                                                       : 1.0;
  return quantityAffected * baseCostPerUnit * severityMultiplier;
}
