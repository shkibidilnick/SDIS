#pragma once
#include <string>
#include <vector>

class Inventory;
class Maintenance;

class Vendor {
 private:
  std::string vendorId;
  std::string name;
  std::vector<Inventory*> supplies;
  std::vector<Maintenance*> maintenanceContracts;

 public:
  Vendor(std::string id, std::string vendorName);

  void addSupply(Inventory* item);
  void addMaintenanceContract(Maintenance* contract);

  std::string getVendorId() const { return vendorId; }
  std::string getName() const { return name; }
};
