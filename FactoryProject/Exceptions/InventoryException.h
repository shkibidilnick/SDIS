#pragma once
#include "FactoryException.h"

// Thrown when inventory operations fail
// Examples: insufficient stock, warehouse full, supplier delivery issues
// (Недостаток запасов, склад заполнен, проблемы с доставкой поставщиками)
class InventoryException : public FactoryException {
 public:
  explicit InventoryException(const std::string& message)
      : FactoryException("Inventory: " + message) {}
};
