#include "ProductionLine.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

ProductionLine::ProductionLine(const std::string& code, const std::string& name,
                               double target)
    : lineCode(code), name(name), targetEfficiency(target), isActive(false) {
  if (code.empty() || name.empty()) {
    throw ProductionException("Production line code and name are required");
  }

  if (targetEfficiency < 0 || targetEfficiency > 100) {
    throw ProductionException("Target efficiency must be between 0-100%");
  }

  lineId = IDGenerator::generate("LINE");
  Logger::getInstance().log("ProductionLine",
                            "Created line " + lineId + " - " + code);
}

void ProductionLine::addMachine(std::shared_ptr<Machine> machine) {
  if (!machine) {
    throw ProductionException("Cannot add null machine to production line");
  }

  if (!machine->getIsOperational()) {
    throw ProductionException("Cannot add non-operational machine " +
                              machine->getMachineId());
  }

  machines.push_back(machine);
  Logger::getInstance().log(
      "ProductionLine",
      "Added machine " + machine->getMachineId() + " to " + lineId);
}

void ProductionLine::removeMachine(const std::string& machineId) {
  auto it =
      std::remove_if(machines.begin(), machines.end(),
                     [&machineId](const std::shared_ptr<Machine>& machine) {
                       return machine->getMachineId() == machineId;
                     });

  if (it != machines.end()) {
    machines.erase(it, machines.end());
    Logger::getInstance().log(
        "ProductionLine", "Removed machine " + machineId + " from " + lineId);
  }
}

bool ProductionLine::isLineOperational() const {
  if (machines.empty()) return false;

  for (const auto& machine : machines) {
    if (!machine->getIsOperational()) {
      return false;
    }
  }
  return true;
}

double ProductionLine::getHourlyCost() const {
  double total = 0.0;
  for (const auto& machine : machines) {
    total += machine->getHourlyRate();
  }
  return total;
}

std::string ProductionLine::generateReport() const {
  std::string report = "Production Line Report: " + name + "\n";
  report += "ID: " + lineId + "\n";
  report += "Machines: " + std::to_string(machines.size()) + "\n";
  report +=
      "Operational: " + std::string(isLineOperational() ? "YES" : "NO") + "\n";
  report += "Hourly Cost: $" + std::to_string(getHourlyCost()) + "\n";
  return report;
}

void ProductionLine::validate() const {
  if (machines.size() < 2) {
    throw ProductionException("Production line must have at least 2 machines");
  }

  if (!isLineOperational()) {
    throw ProductionException("Production line has non-operational machines");
  }

  if (targetEfficiency < 60) {
    throw ProductionException("Target efficiency too low");
  }
}
