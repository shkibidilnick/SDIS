#pragma once
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/QualityExceptionh.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IValidatable.h"

// Defines quality standards for products or processes
// Contains measurable criteria and acceptable limits
class QualityStandard : public IValidatable {
 private:
  std::string standardId;
  std::string name;
  std::string description;
  std::vector<std::pair<std::string, std::pair<double, double>>> criteria;
  // Criteria: (Name, (MinValue, MaxValue))
  bool isActive;

 public:
  QualityStandard(const std::string& name, const std::string& description);

  // Adds a quality criterion with min/max acceptable values
  void addCriterion(const std::string& criterionName, double minValue,
                    double maxValue);

  // Checks if a measurement meets the standard
  bool isMeasurementValid(const std::string& criterionName, double value) const;

  // Gets all criteria
  std::vector<std::string> getCriteriaNames() const;

  // IValidatable implementation
  void validate() const override;

  std::string getStandardId() const { return standardId; }
  std::string getName() const { return name; }
  bool getIsActive() const { return isActive; }
  void setActive(bool active) { isActive = active; }
};
