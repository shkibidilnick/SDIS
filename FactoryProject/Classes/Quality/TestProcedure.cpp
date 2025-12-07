#include "TestProcedure.h"

#include <algorithm>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

TestProcedure::TestProcedure(const std::string& name,
                             const std::string& description,
                             const std::string& duration, bool requiresCert)
    : QualityStandard(name, description),
      estimatedDuration(duration),
      requiresCertification(requiresCert),
      revisionNumber("1.0") {
  procedureId = IDGenerator::generate("TP");

  if (duration.empty()) {
    throw QualityException("Estimated duration is required");
  }

  Logger::getInstance().log("TestProcedure",
                            "Created test procedure " + procedureId);
}

void TestProcedure::addStep(int stepNumber, const std::string& instruction) {
  if (stepNumber <= 0) {
    throw QualityException("Step number must be positive");
  }
  if (instruction.empty()) {
    throw QualityException("Step instruction cannot be empty");
  }

  // Check if step number already exists
  for (const auto& step : steps) {
    if (step.first == stepNumber) {
      throw QualityException("Step number " + std::to_string(stepNumber) +
                             " already exists");
    }
  }

  steps.push_back({stepNumber, instruction});
  // Sort by step number
  std::sort(steps.begin(), steps.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });

  Logger::getInstance().log("TestProcedure",
                            "Added step " + std::to_string(stepNumber) +
                                " to procedure " + procedureId);
}

void TestProcedure::addRequiredTool(const std::string& toolName) {
  if (toolName.empty()) {
    throw QualityException("Tool name cannot be empty");
  }
  requiredTools.push_back(toolName);
}

void TestProcedure::updateRevision(const std::string& newRevision) {
  if (newRevision.empty()) {
    throw QualityException("Revision number cannot be empty");
  }
  revisionNumber = newRevision;

  Logger::getInstance().log(
      "TestProcedure",
      "Updated procedure " + procedureId + " to revision " + newRevision);
}

std::string TestProcedure::generateChecklist() const {
  std::string checklist = "Test Procedure: " + getName() + "\n";
  checklist += "Revision: " + revisionNumber + "\n";
  checklist += "Estimated Duration: " + estimatedDuration + "\n\n";
  checklist += "Required Tools:\n";
  for (const auto& tool : requiredTools) {
    checklist += "  [ ] " + tool + "\n";
  }
  checklist += "\nProcedure Steps:\n";
  for (const auto& step : steps) {
    checklist += std::to_string(step.first) + ". [ ] " + step.second + "\n";
  }
  return checklist;
}

bool TestProcedure::canBePerformedBy(const std::string& techCert) const {
  if (!requiresCertification) return true;
  return !techCert.empty();  // Simplified check
}
