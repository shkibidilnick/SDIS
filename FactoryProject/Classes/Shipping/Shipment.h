#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/Product.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ShippingException.h"
#include "DeliveryOrder.h"

// Represents a physical shipment with packaging and tracking details
// Manages the logistics of packaging, labeling, and carrier handoff
class Shipment : public DeliveryOrder {
 private:
  std::string shipmentId;
  double totalWeightKg;
  double totalVolumeM3;
  std::string packagingType;  // "PALLET", "CRATE", "BOX", "ENVELOPE"
  int packageCount;
  std::vector<std::string> trackingNumbers;  // Multi-package shipments
  bool isInsured;
  double declaredValue;
  std::string customsDocumentation;  // For international shipments

 public:
  Shipment(const std::string& customerId, const std::string& shipToAddress,
           const std::string& requiredDeliveryDate);

  // Adds a product with weight and dimensions for logistics planning
  void addProductWithDetails(std::shared_ptr<Product> product, int quantity,
                             double weightKg, double volumeM3);

  // Sets packaging information
  void setPackaging(const std::string& packaging, int pkgCount);

  // Adds tracking number for individual packages
  void addTrackingNumber(const std::string& trackingNum);

  // Marks shipment as insured
  void setInsurance(double value);

  // Generates customs documentation for international shipping
  void generateCustomsDocumentation();

  // Calculates dimensional weight for freight charges
  double calculateDimensionalWeight() const;

  // Checks if shipment requires special handling (hazardous, fragile, etc.)
  bool requiresSpecialHandling() const;

  std::string getShipmentId() const { return shipmentId; }
  double getTotalWeight() const { return totalWeightKg; }
  double getTotalVolume() const { return totalVolumeM3; }
  std::string getPackagingType() const { return packagingType; }
  std::string getCustomsDocumentation() const { return customsDocumentation; }
  int getPackageCount() const { return packageCount; }
  bool getIsInsured() const { return isInsured; }
  double getDeclaredValue() const { return declaredValue; }
};
