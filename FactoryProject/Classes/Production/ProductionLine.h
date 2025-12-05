#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ProductionException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IReportable.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IValidatable.h"
#include "Machine.h"

// Represents a production line with multiple machines
// Manages workflow and production sequencing
class ProductionLine : public IReportable, public IValidatable {
 private:
  std::string lineId;
  std::string lineCode;
  std::string name;
  std::vector<std::shared_ptr<Machine>> machines;  // Ordered by process flow
  bool isActive;
  double targetEfficiency;  // 0-100%

 public:
  ProductionLine(const std::string& lineCode, const std::string& name,
                 double targetEfficiency);

  // Adds a machine to the production line in sequence
  void addMachine(std::shared_ptr<Machine> machine);

  // Removes a machine from the line
  void removeMachine(const std::string& machineId);

  // Checks if all machines in the line are operational
  bool isLineOperational() const;

  // Calculates total hourly operating cost of the line
  double getHourlyCost() const;

  // Gets number of machines in the line
  size_t getMachineCount() const { return machines.size(); }

  // IReportable implementation
  std::string generateReport() const override;
  std::string getReportType() const override { return "PRODUCTION_LINE"; }

  // IValidatable implementation
  void validate() const override;

  std::string getLineId() const { return lineId; }
  std::string getLineCode() const { return lineCode; }
  std::string getName() const { return name; }
  bool getIsActive() const { return isActive; }
  void setActive(bool active) { isActive = active; }
};
