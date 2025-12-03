#pragma once
#include <string>
#include <vector>

#include "guest.h"
#include "service.h"

class Package {
  std::string packageId;
  std::string title;
  std::vector<Service*> services;
  Guest* buyer;
  double discount;

 public:
  Package(std::string id, std::string t, double disc);
  void addService(Service* s);
  void sellTo(Guest* g);
  double getPrice() const;
  std::string getPackageId() const { return packageId; }
};
