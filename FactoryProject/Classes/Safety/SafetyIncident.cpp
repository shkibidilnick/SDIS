#include "SafetyIncident.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

SafetyIncident::SafetyIncident(const std::string& type, const std::string& desc,
                               const std::string& loc,
                               const std::string& reporter)
    : incidentType(type),
      description(desc),
      location(loc),
      reportedBy(reporter),
      severity("MEDIUM"),
      isInvestigated(false),
      isClosed(false) {
  incidentId = IDGenerator::generate("SI");

  if (type.empty() || desc.empty() || loc.empty() || reporter.empty()) {
    throw SafetyException("All incident fields are required");
  }

  if (type != "INJURY" && type != "NEAR_MISS" && type != "EQUIPMENT_DAMAGE" &&
      type != "VIOLATION") {
    throw SafetyException("Invalid incident type: " + type);
  }

  dateTime = DateTimeUtils::getCurrentTimestamp();

  Logger::getInstance().log(
      "SafetyIncident",
      "CREATED " + type + " incident " + incidentId + " at " + location);
}

std::string SafetyIncident::getStatus() const {
  if (isClosed) return "CLOSED";
  return isInvestigated ? "INVESTIGATED" : "OPEN";
}

void SafetyIncident::investigate(const std::string& report) {
  if (report.empty()) {
    throw SafetyException("Investigation report cannot be empty");
  }

  investigationReport = report;
  isInvestigated = true;

  Logger::getInstance().log("SafetyIncident",
                            "INVESTIGATED incident " + incidentId);
}

void SafetyIncident::addCorrectiveAction(const std::string& action) {
  if (action.empty()) {
    throw SafetyException("Corrective action cannot be empty");
  }
  correctiveActions.push_back(action);

  Logger::getInstance().log("SafetyIncident",
                            "Added corrective action to " + incidentId);
}

void SafetyIncident::close() {
  if (!isInvestigated) {
    throw SafetyException("Cannot close un-investigated incident: " +
                          incidentId);
  }

  if (correctiveActions.empty()) {
    throw SafetyException("Cannot close incident without corrective actions");
  }

  isClosed = true;

  Logger::getInstance().log("SafetyIncident", "CLOSED incident " + incidentId);
}
