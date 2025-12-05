#include "Shipment.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

Shipment::Shipment(const std::string& custId, const std::string& address,
                   const std::string& reqDeliveryDate)
    : DeliveryOrder(custId, address, reqDeliveryDate),
      totalWeightKg(0.0),
      totalVolumeM3(0.0),
      packageCount(0),
      isInsured(false),
      declaredValue(0.0) {
  shipmentId = IDGenerator::generate("SHP");

  Logger::getInstance().log("Shipment", "Created shipment " + shipmentId +
                                            " for customer: " + custId);
}

void Shipment::addProductWithDetails(std::shared_ptr<Product> product,
                                     int quantity, double weightKg,
                                     double volumeM3) {
  // Validate parameters
  Validator::validatePositive(weightKg, "Product weight");
  Validator::validatePositive(volumeM3, "Product volume");

  // Call base class method
  addProduct(product, quantity);

  // Accumulate weight and volume
  totalWeightKg += weightKg * quantity;
  totalVolumeM3 += volumeM3 * quantity;

  Logger::getInstance().log(
      "Shipment", "Added product " + product->getProductId() + " to shipment " +
                      shipmentId + " with weight: " + std::to_string(weightKg) +
                      "kg, volume: " + std::to_string(volumeM3) + "m3");
}

void Shipment::setPackaging(const std::string& packaging, int pkgCount) {
  if (packaging != "PALLET" && packaging != "CRATE" && packaging != "BOX" &&
      packaging != "ENVELOPE") {
    throw ShippingException("Invalid packaging type: " + packaging);
  }

  Validator::validatePositive(pkgCount, "Package count");

  packagingType = packaging;
  packageCount = pkgCount;

  Logger::getInstance().log("Shipment", "Set packaging for shipment " +
                                            shipmentId + ": " + packaging +
                                            ", " + std::to_string(pkgCount) +
                                            " packages");
}

void Shipment::addTrackingNumber(const std::string& trackingNum) {
  if (trackingNum.empty()) {
    throw ShippingException("Tracking number cannot be empty");
  }
  trackingNumbers.push_back(trackingNum);
}

void Shipment::setInsurance(double value) {
  Validator::validatePositive(value, "Insurance value");
  isInsured = true;
  declaredValue = value;

  Logger::getInstance().log("Shipment", "Insured shipment " + shipmentId +
                                            " for $" + std::to_string(value));
}

void Shipment::generateCustomsDocumentation() {
  if (!getIsInsured()) {
    throw ShippingException(
        "Cannot generate customs docs for uninsured international shipment");
  }

  // In real system, this would generate actual customs forms
  customsDocumentation = "Customs Declaration for " + shipmentId + "\n";
  customsDocumentation +=
      "Declared Value: $" + std::to_string(declaredValue) + "\n";
  customsDocumentation += "Contents: Factory Products\n";

  Logger::getInstance().log(
      "Shipment", "Generated customs documentation for " + shipmentId);
}

double Shipment::calculateDimensionalWeight() const {
  // Dimensional weight formula: (Length x Width x Height) / 5000 (for kg)
  // Simplified calculation using volume
  return totalVolumeM3 * 1000000 / 5000;  // Convert m3 to cm3 then divide
}

bool Shipment::requiresSpecialHandling() const {
  // Check if weight exceeds threshold or value is high
  return totalWeightKg > 100.0 || declaredValue > 5000.0 ||
         getOrderValue() > 10000.0;
}
