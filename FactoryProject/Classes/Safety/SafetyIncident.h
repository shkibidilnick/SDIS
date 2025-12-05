#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/SafetyException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"

// Represents a safety incident or near-miss event
// Tracks incident details, investigation, and corrective actions
class SafetyIncident : public ITrackable {
 private:
  std::string incidentId;
  std::string
      incidentType;  // "INJURY", "NEAR_MISS", "EQUIPMENT_DAMAGE", "VIOLATION"
  std::string description;
  std::string location;
  std::string dateTime;
  std::string reportedBy;  // Employee ID
  std::string severity;    // "LOW", "MEDIUM", "HIGH", "CRITICAL"
  bool isInvestigated;
  std::string investigationReport;
  std::vector<std::string> correctiveActions;

 public:
  SafetyIncident(const std::string& incidentType,
                 const std::string& description, const std::string& location,
                 const std::string& reportedBy);

  // Investigates the incident
  void investigate(const std::string& report);

  // Adds a corrective action
  void addCorrectiveAction(const std::string& action);

  // Closes the incident after actions are completed
  void close();

  // ITrackable implementation
  std::string getTrackingId() const override { return incidentId; }
  std::string getStatus() const override {
    return isInvestigated ? "INVESTIGATED" : "OPEN";
  }

  std::string getIncidentId() const { return incidentId; }
  std::string getIncidentType() const { return incidentType; }
  std::string getSeverity() const { return severity; }
  bool getIsInvestigated() const { return isInvestigated; }
  std::string getInvestigationReport() const { return investigationReport; }
};
