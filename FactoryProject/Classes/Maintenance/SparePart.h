#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Inventory/InventoryItem.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/MaintenanceException.h"

// Represents a spare part(запасная часть) for machine maintenance
// Tracks part compatibility and reorder information
class SparePart : public InventoryItem {
 private:
  std::string partNumber;
  std::string manufacturer;
  std::vector<std::string> compatibleMachines;  // Machine IDs
  int leadTimeDays;                             // Time to procure

 public:
  SparePart(const std::string& name, const std::string& sku,
            const std::string& partNum, const std::string& manufacturer,
            int quantity, double cost, const std::string& location,
            int leadTime);

  // Adds a machine ID that this part is compatible with
  void addCompatibleMachine(const std::string& machineId);

  // Checks if part is compatible with a specific machine
  bool isCompatible(const std::string& machineId) const;

  // Gets recommended stock level based on criticality
  int getRecommendedStockLevel() const;
  bool needsReorder() const override {
    return quantity <= getRecommendedStockLevel();
  }
  std::string getPartNumber() const { return partNumber; }
  std::string getManufacturer() const { return manufacturer; }
  int getLeadTimeDays() const { return leadTimeDays; }
};
