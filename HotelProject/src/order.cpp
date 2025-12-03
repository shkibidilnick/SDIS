#include "order.h"
Order::Order(std::string id, Guest* g)
    : orderId(std::move(id)), guest(g), total(0.0), isDelivered(false) {}
void Order::addItem(MenuItem* item) { items.push_back(item); }

void Order::removeItem(MenuItem* item) {
  items.erase(std::remove(items.begin(), items.end(), item), items.end());
}

void Order::calculateTotal() {
  total = 0.0;
  for (auto* item : items) {
    total += item->getPrice();
  }
}

void Order::markDelivered() { isDelivered = true; }
