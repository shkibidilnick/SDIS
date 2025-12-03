#pragma once
#include <string>

#include "interfaces.h"

class MenuItem : public IPricable {
 private:
  std::string itemId;
  std::string name;
  double price;
  std::string category;

 public:
  MenuItem(std::string id, std::string itemName, double p, std::string cat);
  double getPrice() const override { return price; }
  std::string getId() const { return itemId; }
  void updatePrice(double newPrice) { price = newPrice; }
};
