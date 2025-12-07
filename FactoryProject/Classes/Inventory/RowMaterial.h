#pragma once
#include "InventoryItem.h"

// Represents raw materials(сырьё) used in production
// Tracks material properties and supplier(поставщик) information
class RawMaterial : public InventoryItem {
 private:
  std::string materialType;  // e.g., "Steel", "Plastic", "Electronic"
  std::string supplierId;
  int reorderPoint;  // Quantity at which to reorder
  int leadTimeDays;  // Days to receive new shipment(груз)

 public:
  RawMaterial(const std::string& name, const std::string& sku,
              const std::string& materialType, const std::string& supplierId,
              int quantity, double unitCost, const std::string& location,
              int reorderPoint, int leadTimeDays);

  // Calculates when material will run out based on usage rate
  // Returns estimated days until stockout
  int daysUntilStockout(double dailyUsage) const;

  // Checks if material needs to be reordered
  bool needsReorder() const override;

  std::string getMaterialType() const { return materialType; }
  std::string getSupplierId() const { return supplierId; }
  int getReorderPoint() const { return reorderPoint; }
  int getLeadTimeDays() const { return leadTimeDays; }
};
