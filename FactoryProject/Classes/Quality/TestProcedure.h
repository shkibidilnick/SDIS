#pragma once
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/QualityExceptionh.h"
#include "QualityStandard.h"

// Represents a detailed test procedure with step-by-step instructions
// Used for complex quality inspections that require sequential steps
class TestProcedure : public QualityStandard {
 private:
  std::string procedureId;
  std::vector<std::pair<int, std::string>> steps;  // StepNumber, Instruction
  std::vector<std::string> requiredTools;
  std::string estimatedDuration;  // e.g., "30 minutes"
  bool requiresCertification;
  std::string revisionNumber;

 public:
  TestProcedure(const std::string& name, const std::string& description,
                const std::string& duration, bool requiresCert);

  // Adds a step to the procedure
  void addStep(int stepNumber, const std::string& instruction);

  // Adds a required tool/equipment
  void addRequiredTool(const std::string& toolName);

  // Updates the procedure revision
  void updateRevision(const std::string& newRevision);

  // Gets the procedure as a formatted checklist
  std::string generateChecklist() const;

  // Checks if a technician has required certification to perform this procedure
  bool canBePerformedBy(const std::string& technicianCertification) const;

  std::string getProcedureId() const { return procedureId; }
  std::string getEstimatedDuration() const { return estimatedDuration; }
  std::string getRevisionNumber() const { return revisionNumber; }
  bool getRequiresCertification() const { return requiresCertification; }
  size_t getStepCount() const { return steps.size(); }
};
