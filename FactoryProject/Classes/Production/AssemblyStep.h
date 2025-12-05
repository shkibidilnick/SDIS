#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Employee.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ProductionException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"
#include "Machine.h"

// Represents a single step in the assembly process(процесс сборки)
// Tracks time, operator, and machine usage for each step
class AssemblyStep : public ITrackable {
 private:
  std::string stepId;
  std::string description;
  int sequenceNumber;  // Order in assembly (1, 2, 3...)
  double estimatedMinutes;
  double actualMinutes;
  std::string machineId;
  std::string operatorId;
  bool isCompleted;
  std::vector<std::string> qualityCheckpoints;  // Required inspections

 public:
  AssemblyStep(const std::string& desc, int sequence, double estMinutes);

  // Records actual completion time
  void recordCompletion(double actualMinutes);

  // Assigns machine to this step
  void assignMachine(const std::string& machId);

  // Assigns operator to this step
  void assignOperator(const std::string& opId);

  // Adds a quality checkpoint requirement
  void addQualityCheckpoint(const std::string& checkpoint);

  // Marks step as completed
  void complete();

  // ITrackable implementation
  std::string getTrackingId() const override { return stepId; }
  std::string getStatus() const override {
    return isCompleted ? "COMPLETED" : "PENDING";
  }

  std::string getDescription() const { return description; }
  int getSequenceNumber() const { return sequenceNumber; }
  double getEstimatedMinutes() const { return estimatedMinutes; }
  double getActualMinutes() const { return actualMinutes; }
  std::string getMachineId() const { return machineId; }
  std::string getOperatorId() const { return operatorId; }
  bool getIsCompleted() const { return isCompleted; }
};
