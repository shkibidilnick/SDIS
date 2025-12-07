#include "MaintenanceSchedule.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

MaintenanceSchedule::MaintenanceSchedule(const std::string& machineId,
                                         const std::string& desc,
                                         const std::string& recurrence,
                                         int intervalDays)
    : MaintenanceRequest(machineId, desc,
                         "LOW"),  // Planned maintenance = low priority
      recurrencePattern(recurrence),
      recurrenceInterval(intervalDays),
      isRecurring(true),
      estimatedHours(2) {
  scheduleId = IDGenerator::generate("MSCH");

  if (recurrence != "DAILY" && recurrence != "WEEKLY" &&
      recurrence != "MONTHLY" && recurrence != "QUARTERLY") {
    throw MaintenanceException("Invalid recurrence pattern: " + recurrence);
  }

  Validator::validatePositive(intervalDays, "Recurrence interval");

  calculateNextDueDate();

  Logger::getInstance().log(
      "MaintenanceSchedule",
      "Created schedule " + scheduleId + " for machine: " + machineId);
}

void MaintenanceSchedule::calculateNextDueDate() {
  int daysToAdd = 0;
  if (recurrencePattern == "DAILY")
    daysToAdd = 1;
  else if (recurrencePattern == "WEEKLY")
    daysToAdd = 7;
  else if (recurrencePattern == "MONTHLY")
    daysToAdd = 30;
  else if (recurrencePattern == "QUARTERLY")
    daysToAdd = 90;

  nextDueDate = DateTimeUtils::addDays(
      DateTimeUtils::getCurrentTimestamp().substr(0, 10), daysToAdd);
}

void MaintenanceSchedule::recordPerformance(const std::string& performedDate,
                                            int actualHours) {
  if (!DateTimeUtils::isValidDate(performedDate)) {
    throw MaintenanceException("Invalid performed date: " + performedDate);
  }

  Validator::validatePositive(actualHours, "Actual hours");

  lastPerformedDate = performedDate;
  estimatedHours = actualHours;  // Update estimate based on actuals

  // Recalculate next due date
  calculateNextDueDate();

  Logger::getInstance().log("MaintenanceSchedule",
                            "Recorded performance for schedule " + scheduleId);
}

std::shared_ptr<MaintenanceRequest> MaintenanceSchedule::generateWorkOrder()
    const {
  // Create a new maintenance request from this schedule
  auto workOrder = std::make_shared<MaintenanceRequest>(
      getMachineId(),
      "Scheduled " + recurrencePattern +
          " maintenance: " + getIssueDescription(),
      "MEDIUM");

  // Copy required parts from the schedule
  // Note: Need to expose requiredParts in base class or add getter
  // For now, returning a basic work order

  Logger::getInstance().log("MaintenanceSchedule",
                            "Generated work order from schedule " + scheduleId);

  return workOrder;
}

bool MaintenanceSchedule::isOverdue() const {
  return DateTimeUtils::getCurrentTimestamp().substr(0, 10) > nextDueDate;
}

void MaintenanceSchedule::skipOccurrence() {
  // Move to next cycle without performing maintenance
  calculateNextDueDate();

  Logger::getInstance().log("MaintenanceSchedule",
                            "Skipped occurrence for schedule " + scheduleId);
}
