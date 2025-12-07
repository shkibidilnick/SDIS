#include "RowMaterial.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/InventoryException.h"

RawMaterial::RawMaterial(const std::string& name, const std::string& sku,
                         const std::string& matType, const std::string& suppId,
                         int qty, double cost, const std::string& loc,
                         int reorderPt, int leadTime)
    : InventoryItem(name, sku, qty, cost, loc),
      materialType(matType),
      supplierId(suppId),
      reorderPoint(reorderPt),
      leadTimeDays(leadTime) {
  if (reorderPoint < 0) {
    throw InventoryException("Reorder point cannot be negative");
  }

  if (leadTimeDays < 1) {
    throw InventoryException("Lead time must be at least 1 day");
  }
}

int RawMaterial::daysUntilStockout(double dailyUsage) const {
  if (dailyUsage <= 0) return -1;  // No usage means never runs out

  return static_cast<int>(quantity / dailyUsage);
}

bool RawMaterial::needsReorder() const { return quantity <= reorderPoint; }
