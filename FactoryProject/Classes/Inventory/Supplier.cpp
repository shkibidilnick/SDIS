#include "Supplier.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

Supplier::Supplier(const std::string& name, const std::string& email,
                   const std::string& phone)
    : name(name),
      contactEmail(email),
      contactPhone(phone),
      reliabilityScore(85.0),
      isApproved(false) {  // Start with neutral score

  if (name.empty() || email.empty()) {
    throw FactoryException("Supplier name and email are required");
  }

  supplierId = IDGenerator::generate("SUP");
  Logger::getInstance().log("Supplier",
                            "Created supplier " + supplierId + " - " + name);
}

void Supplier::addMaterial(const std::string& materialType) {
  if (materialType.empty()) {
    throw FactoryException("Material type cannot be empty");
  }
  materialsProvided.push_back(materialType);
}

void Supplier::updateReliabilityScore(bool deliveredOnTime) {
  if (deliveredOnTime) {
    reliabilityScore = std::min(100.0, reliabilityScore + 1.0);
  } else {
    reliabilityScore =
        std::max(0.0, reliabilityScore - 5.0);  // Penalize more heavily
  }
}

bool Supplier::suppliesMaterial(const std::string& materialType) const {
  return std::find(materialsProvided.begin(), materialsProvided.end(),
                   materialType) != materialsProvided.end();
}

int Supplier::getAverageLeadTime() const {
  // Simplified: return based on reliability (higher score = faster delivery)
  return static_cast<int>(14 -
                          (reliabilityScore / 100.0 * 7));  // 7-14 days range
}
