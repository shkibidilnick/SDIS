#pragma once
#include <memory>
#include <string>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/InventoryException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"

// Base class for all inventory items
// Provides common properties for tracking and managing inventory
class InventoryItem : public ITrackable {
 protected:
  std::string itemId;
  std::string name;
  std::string sku;  // Stock Keeping Unit (Отдел хранения товаров)
  int quantity;     // количество
  double unitCost;
  std::string location;  // Warehouse(склад) location

 public:
  InventoryItem(const std::string& name, const std::string& sku, int quantity,
                double unitCost, const std::string& location);
  // Adjusts inventory quantity (positive for receipt(поступление товаров), negative for issue(выдача))
  // Throws InventoryException if result would be negative
  virtual void adjustQuantity(int change);

  // Gets total value of this inventory
  virtual double getTotalValue() const { return quantity * unitCost; }

  // Checks if reorder is needed (threshold(порог) = 10% of typical batch size(размер партии))
  virtual bool needsReorder() const = 0;

  // ITrackable implementation
  std::string getTrackingId() const override { return itemId; }
  std::string getStatus() const override {
    return quantity > 0 ? "IN_STOCK" : "OUT_OF_STOCK";
  }

  std::string getItemId() const { return itemId; }
  std::string getName() const { return name; }
  std::string getSku() const { return sku; }
  int getQuantity() const { return quantity; }
  double getUnitCost() const { return unitCost; }
  std::string getLocation() const { return location; }
};
