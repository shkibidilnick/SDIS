#include "inventory.h"


Inventory::Inventory(std::string id, std::string itemName, int qty, double price)
    : itemId(std::move(id)), name(std::move(itemName)), quantity(qty),
    unitPrice(price), supplier("Unknown") {}

void Inventory::updateQuantity(int change) {
    quantity += change;
    if (quantity < 0) quantity = 0;
}

void Inventory::setUnitPrice(double price) {
    if (price < 0) throw InventoryException("Price cannot be negative");
    unitPrice = price;
}

bool Inventory::isLowStock() const {
    return quantity < 10;
}