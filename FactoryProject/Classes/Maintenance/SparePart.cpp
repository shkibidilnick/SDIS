#include "SparePart.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

SparePart::SparePart(const std::string& name, const std::string& sku,
                     const std::string& partNum, const std::string& manuf,
                     int qty, double cost, const std::string& loc, int leadTime)
    : InventoryItem(name, sku, qty, cost, loc),
      partNumber(partNum),
      manufacturer(manuf),
      leadTimeDays(leadTime) {
  if (partNum.empty() || manuf.empty()) {
    throw MaintenanceException("Part number and manufacturer are required");
  }

  if (leadTime < 1) {
    throw MaintenanceException("Lead time must be at least 1 day");
  }

  Logger::getInstance().log(
      "SparePart", "Created spare part " + getItemId() + " - " + partNumber);
}

void SparePart::addCompatibleMachine(const std::string& machineId) {
  if (machineId.empty()) {
    throw MaintenanceException("Machine ID cannot be empty");
  }
  compatibleMachines.push_back(machineId);
}

bool SparePart::isCompatible(const std::string& machineId) const {
  return std::find(compatibleMachines.begin(), compatibleMachines.end(),
                   machineId) != compatibleMachines.end();
}

int SparePart::getRecommendedStockLevel() const {
  // Critical parts should have higher stock
  bool isCritical = (leadTimeDays > 7);  // Long lead time = critical
  return isCritical ? 10 : 3;            // Units to keep in stock
}
