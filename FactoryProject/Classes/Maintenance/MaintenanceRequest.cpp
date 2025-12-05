#include "MaintenanceRequest.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

MaintenanceRequest::MaintenanceRequest(const std::string& machId,
                                       const std::string& desc,
                                       const std::string& prior)
    : machineId(machId),
      issueDescription(desc),
      priority(prior),
      status("OPEN"),
      isSafetyRelated(false) {
  requestId = IDGenerator::generate("MR");

  if (machId.empty() || desc.empty()) {
    throw MaintenanceException("Machine ID and description are required");
  }

  if (priority != "LOW" && priority != "MEDIUM" && priority != "HIGH" &&
      priority != "URGENT") {
    throw MaintenanceException("Invalid priority: " + priority);
  }

  requestedDate = DateTimeUtils::getCurrentTimestamp();

  Logger::getInstance().log("MaintenanceRequest",
                            "Created request " + requestId + " for machine: " +
                                machId + ", Priority: " + priority);
}

void MaintenanceRequest::assignTechnician(const std::string& techId) {
  if (techId.empty()) {
    throw MaintenanceException("Technician ID cannot be empty");
  }
  assignedTechnicianId = techId;
  status = "ASSIGNED";

  Logger::getInstance().log(
      "MaintenanceRequest",
      "Assigned technician " + techId + " to " + requestId);
}

void MaintenanceRequest::addRequiredPart(const std::string& partSku) {
  if (partSku.empty()) {
    throw MaintenanceException("Part SKU cannot be empty");
  }
  requiredParts.push_back(partSku);
}

void MaintenanceRequest::updateStatus(const std::string& newStatus) {
  const std::vector<std::string> validStatuses = {"OPEN", "ASSIGNED",
                                                  "IN_PROGRESS", "COMPLETED"};

  if (std::find(validStatuses.begin(), validStatuses.end(), newStatus) ==
      validStatuses.end()) {
    throw MaintenanceException("Invalid status: " + newStatus);
  }

  status = newStatus;

  Logger::getInstance().log(
      "MaintenanceRequest",
      "Request " + requestId + " status updated to: " + newStatus);
}

void MaintenanceRequest::complete() {
  if (status != "IN_PROGRESS") {
    throw MaintenanceException("Cannot complete request in status: " + status);
  }

  status = "COMPLETED";

  Logger::getInstance().log("MaintenanceRequest",
                            "Completed request " + requestId);
}
