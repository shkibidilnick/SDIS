#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"
#include "RowMaterial.h"

// Represents a supplier of raw materials
// Tracks supplier reliability and material quality history
class Supplier : public ITrackable {
 private:
  std::string supplierId;
  std::string name;
  std::string contactEmail;
  std::string contactPhone;
  std::vector<std::string> materialsProvided;  // Material types
  double reliabilityScore;  // 0-100, based on on-time delivery
  bool isApproved;

 public:
  Supplier(const std::string& name, const std::string& email,
           const std::string& phone);

  // Adds a material type to supplier's catalog
  void addMaterial(const std::string& materialType);

  // Updates reliability score based on delivery performance
  void updateReliabilityScore(bool deliveredOnTime);

  // Checks if supplier provides a specific material
  bool suppliesMaterial(const std::string& materialType) const;

  // Gets average delivery time for this supplier
  int getAverageLeadTime() const;

  // ITrackable implementation
  std::string getTrackingId() const override { return supplierId; }
  std::string getStatus() const override {
    return isApproved ? "APPROVED" : "PENDING";
  }

  std::string getSupplierId() const { return supplierId; }
  std::string getName() const { return name; }
  double getReliabilityScore() const { return reliabilityScore; }
  bool getIsApproved() const { return isApproved; }
  void setApproved(bool approved) { isApproved = approved; }
};
