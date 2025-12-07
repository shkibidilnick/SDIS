#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ProductionException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IReportable.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"

// Represents a finished product or sub-assembly
// Tracks product genealogy and quality history
class Product : public ITrackable, public IReportable {
 private:
  std::string productId;
  std::string productCode;
  std::string name;
  std::string description;
  double unitPrice;
  std::vector<std::string> rawMaterialsUsed;  // SKUs of materials
  std::string productionDate;
  bool passedQualityCheck;
  bool isShipped;

 public:
  Product(const std::string& productCode, const std::string& name,
          const std::string& description, double unitPrice);

  // Records a raw material used in this product (for traceability)
  void addRawMaterial(const std::string& materialSku);

  // Marks product as passed quality check
  void markQualityPassed();

  // Marks product as shipped to customer
  void markShipped();

  // Checks if product is ready for shipment (passed QC and not already shipped)
  bool isReadyForShipment() const;

  // ITrackable implementation
  std::string getTrackingId() const override { return productId; }
  std::string getStatus() const override;

  // IReportable implementation
  std::string generateReport() const override;
  std::string getReportType() const override { return "PRODUCT"; }

  std::string getProductId() const { return productId; }
  std::string getName() const { return name; }
  std::string getProductCode() const { return productCode; }
  double getUnitPrice() const { return unitPrice; }
  bool hasPassedQualityCheck() const { return passedQualityCheck; }
  bool getIsShipped() const { return isShipped; }
};
