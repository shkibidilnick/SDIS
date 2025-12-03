#include "vendor.h"

Vendor::Vendor(std::string id, std::string vendorName)
    : vendorId(std::move(id)), name(std::move(vendorName)) {}

void Vendor::addSupply(Inventory* item) {
    if (item) supplies.push_back(item);
}

void Vendor::addMaintenanceContract(Maintenance* contract) {
    if (contract) maintenanceContracts.push_back(contract);
}