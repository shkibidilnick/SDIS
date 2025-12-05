#pragma once
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/QualityExceptionh.h"
#include "QualityCheck.h"

// Represents a specific product defect that failed quality inspection
// Tracks defect details, root cause, and corrective actions
class Defect : public QualityCheck {
 private:
  std::string defectId;
  std::string defectType;  // e.g., "DIMENSIONAL", "COSMETIC", "FUNCTIONAL"
  std::string severity;    // "MINOR", "MAJOR", "CRITICAL"
  std::string rootCause;
  std::vector<std::string> correctiveActions;
  bool isResolved;
  int quantityAffected;

 public:
  Defect(const std::string& productId, const std::string& standardId,
         const std::string& inspectorId, const std::string& defectType,
         int quantityAffected);

  // Sets the root cause analysis for this defect
  void setRootCause(const std::string& cause);

  // Adds a corrective action to address the defect
  void addCorrectiveAction(const std::string& action);

  // Marks the defect as resolved after corrective actions are implemented
  void markAsResolved();

  // Gets the cost impact of this defect (placeholder for business logic)
  double calculateCostImpact() const;

  std::string getDefectId() const { return defectId; }
  std::string getDefectType() const { return defectType; }
  std::string getSeverity() const { return severity; }
  std::string getRootCause() const { return rootCause; }
  bool getIsResolved() const { return isResolved; }
  int getQuantityAffected() const { return quantityAffected; }
};
