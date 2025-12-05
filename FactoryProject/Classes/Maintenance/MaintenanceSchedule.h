#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/MaintenanceException.h"
#include "MaintenanceRequest.h"

// Represents a scheduled preventive maintenance task
// Extends MaintenanceRequest with scheduling and recurrence features
class MaintenanceSchedule : public MaintenanceRequest {
 private:
  std::string scheduleId;
  std::string recurrencePattern;  // "DAILY", "WEEKLY", "MONTHLY", "QUARTERLY"
  std::string nextDueDate;
  std::string lastPerformedDate;
  int estimatedHours;
  bool isRecurring;
  int recurrenceInterval;  // Number of days between occurrences

 public:
  MaintenanceSchedule(const std::string& machineId,
                      const std::string& description,
                      const std::string& recurrence, int intervalDays);

  // Calculates the next due date based on recurrence pattern
  void calculateNextDueDate();

  // Records that maintenance was performed and updates schedule
  void recordPerformance(const std::string& performedDate, int actualHours);

  // Generates work orders for the next maintenance cycle
  std::shared_ptr<MaintenanceRequest> generateWorkOrder() const;

  // Checks if maintenance is overdue
  bool isOverdue() const;

  // Skips one occurrence (e.g., if machine is not in use)
  void skipOccurrence();

  std::string getScheduleId() const { return scheduleId; }
  std::string getRecurrencePattern() const { return recurrencePattern; }
  std::string getNextDueDate() const { return nextDueDate; }
  std::string getLastPerformedDate() const { return lastPerformedDate; }
  int getEstimatedHours() const { return estimatedHours; }
  void setEstimatedHours(int hours) { estimatedHours = hours; }
};
