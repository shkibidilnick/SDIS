#include "Warehouse.h"

#include <algorithm>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

Warehouse::Warehouse(const std::string& name, int cap)
    : name(name), capacity(cap), currentOccupancy(0) {
  warehouseId = IDGenerator::generate("WH");

  if (capacity <= 0) {
    throw InventoryException("Warehouse capacity must be positive");
  }

  Logger::getInstance().log("Warehouse",
                            "Created warehouse " + warehouseId + " - " + name);
}

void Warehouse::addItem(std::shared_ptr<InventoryItem> item) {
  if (!item) {
    throw InventoryException("Cannot add null item to warehouse");
  }

  if (currentOccupancy >= capacity) {
    throw InventoryException("Warehouse " + name + " is at full capacity");
  }

  // If item exists, merge quantities, otherwise add new
  std::string sku = item->getSku();
  if (items.count(sku) > 0) {
    // Merge with existing item
    int additionalQty = item->getQuantity();
    items[sku]->adjustQuantity(additionalQty);
  } else {
    items[sku] = item;
    currentOccupancy++;
  }

  Logger::getInstance().log("Warehouse",
                            "Added item " + sku + " to " + warehouseId);
}

void Warehouse::removeItem(const std::string& sku, int quantity) {
  auto item = findItem(sku);
  if (!item) {
    throw InventoryException("Item " + sku + " not found in warehouse " + name);
  }

  item->adjustQuantity(-quantity);

  // Remove from map if quantity reaches zero
  if (item->getQuantity() == 0) {
    items.erase(sku);
    currentOccupancy--;
  }
}

std::shared_ptr<InventoryItem> Warehouse::findItem(
    const std::string& sku) const {
  auto it = items.find(sku);
  if (it != items.end()) {
    return it->second;
  }
  return nullptr;
}

double Warehouse::getTotalInventoryValue() const {
  double total = 0.0;
  for (const auto& pair : items) {
    total += pair.second->getTotalValue();
  }
  return total;
}

double Warehouse::getCapacityUtilization() const {
  return (capacity > 0)
             ? (static_cast<double>(currentOccupancy) / capacity) * 100.0
             : 0.0;
}

bool Warehouse::checkAvailability(const std::string& sku,
                                  int requiredQuantity) const {
  auto item = findItem(sku);
  return item && item->getQuantity() >= requiredQuantity;
}

std::string Warehouse::generateReport() const {
  std::string report = "Warehouse Report: " + name + "\n";
  report += "ID: " + warehouseId + "\n";
  report +=
      "Capacity Utilization: " + std::to_string(getCapacityUtilization()) +
      "%\n";
  report += "Total Value: $" + std::to_string(getTotalInventoryValue()) + "\n";
  report += "Items Count: " + std::to_string(items.size()) + "\n";
  return report;
}

void Warehouse::validate() const {
  if (currentOccupancy > capacity) {
    throw InventoryException("Warehouse occupancy exceeds capacity");
  }

  if (getTotalInventoryValue() > 1000000) {  // Example: $1M limit
    throw InventoryException("Warehouse inventory value exceeds safety limit");
  }
}
