#include "AssemblyStep.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

AssemblyStep::AssemblyStep(const std::string& desc, int sequence,
                           double estMinutes)
    : description(desc),
      sequenceNumber(sequence),
      estimatedMinutes(estMinutes),
      actualMinutes(0.0),
      isCompleted(false) {
  stepId = IDGenerator::generate("STEP");

  if (desc.empty()) {
    throw ProductionException("Assembly step description cannot be empty");
  }

  Validator::validatePositive(estMinutes, "Estimated time");

  Logger::getInstance().log(
      "AssemblyStep",
      "Created step " + stepId + " #" + std::to_string(sequence) + ": " + desc);
}

void AssemblyStep::recordCompletion(double actual) {
  Validator::validatePositive(actual, "Actual minutes");

  actualMinutes = actual;

  Logger::getInstance().log(
      "AssemblyStep",
      "Step " + stepId + " completed in " + std::to_string(actual) +
          " minutes (est: " + std::to_string(estimatedMinutes) + ")");
}

void AssemblyStep::assignMachine(const std::string& machId) {
  if (machId.empty()) {
    throw ProductionException("Machine ID cannot be empty");
  }
  machineId = machId;
}

void AssemblyStep::assignOperator(const std::string& opId) {
  if (opId.empty()) {
    throw ProductionException("Operator ID cannot be empty");
  }
  operatorId = opId;
}

void AssemblyStep::addQualityCheckpoint(const std::string& checkpoint) {
  if (checkpoint.empty()) {
    throw ProductionException("Quality checkpoint cannot be empty");
  }
  qualityCheckpoints.push_back(checkpoint);
}

void AssemblyStep::complete() {
  if (isCompleted) {
    throw ProductionException("Step " + stepId + " already completed");
  }

  if (actualMinutes == 0.0) {
    throw ProductionException("Cannot complete step " + stepId +
                              " without recording actual time");
  }

  isCompleted = true;

  Logger::getInstance().log("AssemblyStep",
                            "Marked step " + stepId + " as COMPLETED");
}
