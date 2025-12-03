#pragma once
#include <string>
#include <vector>

#include "guest.h"
#include "menuitem.h"

class Order {
  std::string orderId;
  Guest* guest;
  std::vector<MenuItem*> items;
  double total;
  bool isDelivered;

 public:
  Order(std::string id, Guest* guest);
  void addItem(MenuItem* item);
  void removeItem(MenuItem* item);
  void calculateTotal();
  void markDelivered();
  std::string getOrderId() const { return orderId; }
  double getTotal() const { return total; }
};
