#include "InventoryItem.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

InventoryItem::InventoryItem(const std::string& name, const std::string& sku,
                             int qty, double cost, const std::string& loc)
    : name(name), sku(sku), quantity(qty), unitCost(cost), location(loc) {
  itemId = IDGenerator::generate("INV");

  if (qty < 0) {
    throw InventoryException("Initial quantity cannot be negative");
  }

  if (cost < 0) {
    throw InventoryException("Unit cost cannot be negative");
  }

  Logger::getInstance().log("Inventory",
                            "Created item " + itemId + " - " + name);
}

void InventoryItem::adjustQuantity(int change) { 
  if (quantity + change < 0) {
    throw InventoryException("Insufficient stock for (недостаточный запас) " +
                             name + ". Available: " + std::to_string(quantity) +
                             ", Required: " + std::to_string(-change));
  }

  int oldQuantity = quantity;
  quantity += change;

  Logger::getInstance().log("Inventory", "Adjusted " + name + " from " +
                                             std::to_string(oldQuantity) +
                                             " to " + std::to_string(quantity));
}
