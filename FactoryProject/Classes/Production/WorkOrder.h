#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ProductionException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IValidatable.h"
#include "Product.h"

// Represents a production work order
// Tracks manufacturing job from start to completion
class WorkOrder : public ITrackable, public IValidatable {
 private:
  std::string workOrderId;
  std::string productCode;
  int quantityRequested;
  int quantityCompleted;
  std::string startDate;
  std::string targetCompletionDate;
  std::string status;  // "PLANNED", "IN_PROGRESS", "COMPLETED", "CANCELLED"
  std::vector<std::string> requiredMaterials;  // SKUs
  std::string assignedLineId;

 public:
  WorkOrder(const std::string& productCode, int quantity,
            const std::string& targetDate);

  // Starts the work order
  void start();

  // Completes specified quantity
  void completeQuantity(int qty);

  // Cancels the work order
  void cancel();

  // Gets completion percentage
  double getCompletionPercentage() const;

  // Adds a required raw material SKU
  void addRequiredMaterial(const std::string& materialSku);

  // Assigns to a production line
  void assignToLine(const std::string& lineId);

  // ITrackable implementation
  std::string getTrackingId() const override { return workOrderId; }
  std::string getStatus() const override { return status; }

  // IValidatable implementation
  void validate() const override;

  std::string getWorkOrderId() const { return workOrderId; }
  std::string getProductCode() const { return productCode; }
  int getQuantityRequested() const { return quantityRequested; }
  int getQuantityCompleted() const { return quantityCompleted; }
  std::string getAssignedLineId() const { return assignedLineId; }
};
