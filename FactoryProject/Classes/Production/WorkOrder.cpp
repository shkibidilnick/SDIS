#include "WorkOrder.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

WorkOrder::WorkOrder(const std::string& productCode, int qty,
                     const std::string& targetDate)
    : productCode(productCode),
      quantityRequested(qty),
      quantityCompleted(0),
      targetCompletionDate(targetDate),
      status("PLANNED") {
  workOrderId = IDGenerator::generate("WO");

  if (productCode.empty()) {
    throw ProductionException("Product code is required for work order");
  }

  Validator::validatePositive(qty, "Work order quantity");

  if (!DateTimeUtils::isValidDate(targetDate)) {
    throw ProductionException("Invalid target date: " + targetDate);
  }

  Logger::getInstance().log("WorkOrder", "Created work order " + workOrderId +
                                             " for product: " + productCode);
}

void WorkOrder::start() {
  if (status != "PLANNED") {
    throw ProductionException("Work order " + workOrderId +
                              " cannot be started from status: " + status);
  }

  status = "IN_PROGRESS";
  startDate = DateTimeUtils::getCurrentTimestamp();

  Logger::getInstance().log("WorkOrder", "Started work order " + workOrderId);
}

void WorkOrder::completeQuantity(int qty) {
  if (status != "IN_PROGRESS") {
    throw ProductionException(
        "Cannot complete quantity for work order in status: " + status);
  }

  Validator::validatePositive(qty, "Completed quantity");

  if (quantityCompleted + qty > quantityRequested) {
    throw ProductionException(
        "Cannot complete more than requested. Requested: " +
        std::to_string(quantityRequested) +
        ", Already completed: " + std::to_string(quantityCompleted) +
        ", Attempting to add: " + std::to_string(qty));
  }

  quantityCompleted += qty;

  if (quantityCompleted == quantityRequested) {
    status = "COMPLETED";
    Logger::getInstance().log("WorkOrder",
                              "Work order " + workOrderId + " COMPLETED");
  } else {
    Logger::getInstance().log(
        "WorkOrder", "Work order " + workOrderId + " progress: " +
                         std::to_string(getCompletionPercentage()) + "%");
  }
}

void WorkOrder::cancel() {
  if (status == "COMPLETED") {
    throw ProductionException("Cannot cancel completed work order: " +
                              workOrderId);
  }

  status = "CANCELLED";

  Logger::getInstance().log("WorkOrder", "Cancelled work order " + workOrderId);
}

double WorkOrder::getCompletionPercentage() const {
  return (quantityRequested > 0) ? (static_cast<double>(quantityCompleted) /
                                    quantityRequested * 100.0)
                                 : 0.0;
}

void WorkOrder::addRequiredMaterial(const std::string& materialSku) {
  if (materialSku.empty()) {
    throw ProductionException("Material SKU cannot be empty");
  }
  requiredMaterials.push_back(materialSku);
}

void WorkOrder::assignToLine(const std::string& lineId) {
  if (lineId.empty()) {
    throw ProductionException("Production line ID cannot be empty");
  }
  assignedLineId = lineId;
}

void WorkOrder::validate() const {
  Validator::validatePositive(quantityRequested, "Requested quantity");

  if (requiredMaterials.empty()) {
    throw ProductionException(
        "Work order must have at least one required material");
  }

  if (assignedLineId.empty()) {
    throw ProductionException(
        "Work order must be assigned to a production line");
  }

  if (quantityCompleted > quantityRequested) {
    throw ProductionException("Completed quantity exceeds requested quantity");
  }
}
