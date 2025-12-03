#pragma once
#include <string>
#include "exceptions.h"

class Inventory {
 private:
  std::string itemId;
  std::string name;
  int quantity;  // quantity - количество
  double unitPrice;
  std::string supplier;  // supplier - поставщик

 public:
  Inventory(std::string id, std::string itemName, int qty, double price);

  void updateQuantity(int change);
  void setUnitPrice(double price);
  bool isLowStock() const;

  std::string getItemId() const { return itemId; }
  int getQuantity() const { return quantity; }
  double getUnitPrice() const { return unitPrice; }
};
