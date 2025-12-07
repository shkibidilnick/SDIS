#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/InventoryException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IReportable.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IValidatable.h"
#include "InventoryItem.h"

// Manages all inventory storage and movement
// Tracks items across multiple locations and generates stock reports
class Warehouse : public IReportable, public IValidatable {
 private:
  std::string warehouseId;
  std::string name;
  std::unordered_map<std::string, std::shared_ptr<InventoryItem>>
      items;  // SKU -> Item
  int capacity; //вместимость
  int currentOccupancy;

 public:
  Warehouse(const std::string& name, int capacity);

  // Adds an inventory item to the warehouse
  // Throws InventoryException if warehouse is full
  void addItem(std::shared_ptr<InventoryItem> item);

  // Removes an item from warehouse (e.g., for production use)
  void removeItem(const std::string& sku, int quantity);

  // Finds an item by SKU
  std::shared_ptr<InventoryItem> findItem(const std::string& sku) const;

  // Gets total inventory value in warehouse
  double getTotalInventoryValue() const;

  // Gets percentage of warehouse capacity used
  double getCapacityUtilization() const;

  // Checks if item is available in sufficient quantity(в достаточном количестве)
  bool checkAvailability(const std::string& sku, int requiredQuantity) const;

  // IReportable implementation
  std::string generateReport() const override;
  std::string getReportType() const override { return "INVENTORY"; }

  // IValidatable implementation
  void validate() const override;

  std::string getWarehouseId() const { return warehouseId; }
  std::string getName() const { return name; }
  int getCapacity() const { return capacity; }
  int getCurrentOccupancy() const { return currentOccupancy; }
};
