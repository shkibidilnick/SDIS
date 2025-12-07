#include "PurchaseOrder.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

PurchaseOrder::PurchaseOrder(const std::string& suppId,
                             const std::string& expectedDate)
    : supplierId(suppId),
      expectedDeliveryDate(expectedDate),
      status("DRAFT"),
      subtotal(0.0),
      taxAmount(0.0),
      totalAmount(0.0),
      isApproved(false) {
  poNumber = IDGenerator::generate("PO");

  if (suppId.empty()) {
    throw FinanceException("Supplier ID is required for purchase order");
  }

  if (!DateTimeUtils::isValidDate(expectedDate)) {
    throw FinanceException("Invalid expected delivery date: " + expectedDate);
  }

  orderDate = DateTimeUtils::getCurrentTimestamp().substr(0, 10);

  Logger::getInstance().log(
      "PurchaseOrder", "Created PO " + poNumber + " for supplier: " + suppId);
}

void PurchaseOrder::addLineItem(std::shared_ptr<RawMaterial> material,
                                int quantity) {
  if (status != "DRAFT") {
    throw FinanceException("Cannot modify purchase order in status: " + status);
  }

  if (!material) {
    throw FinanceException("Cannot add null material to purchase order");
  }

  Validator::validatePositive(quantity, "Quantity");

  lineItems.push_back({material, quantity});
  subtotal += material->getUnitCost() * quantity;

  Logger::getInstance().log("PurchaseOrder", "Added line item to " + poNumber +
                                                 ": " + material->getSku() +
                                                 " x" +
                                                 std::to_string(quantity));
}

void PurchaseOrder::calculateTotals(double taxRate) {
  Validator::validatePercentage(taxRate, "Tax rate");

  if (lineItems.empty()) {
    throw FinanceException("Cannot calculate totals for empty purchase order");
  }

  taxAmount = subtotal * (taxRate / 100.0);
  totalAmount = subtotal + taxAmount;

  Logger::getInstance().log(
      "PurchaseOrder", "Calculated totals for " + poNumber + ": Subtotal=$" +
                           std::to_string(subtotal) + ", Tax=$" +
                           std::to_string(taxAmount) + ", Total=$" +
                           std::to_string(totalAmount));
}

void PurchaseOrder::approve() {
  if (status != "DRAFT") {
    throw FinanceException("Purchase order is not in DRAFT status");
  }

  if (lineItems.empty()) {
    throw FinanceException("Cannot approve empty purchase order");
  }

  isApproved = true;
  status = "SENT";

  Logger::getInstance().log("PurchaseOrder",
                            "Approved and sent PO " + poNumber);
}

void PurchaseOrder::markConfirmed() {
  if (status != "SENT") {
    throw FinanceException("Only SENT purchase orders can be confirmed");
  }
  status = "CONFIRMED";

  Logger::getInstance().log("PurchaseOrder",
                            "Supplier confirmed PO " + poNumber);
}

void PurchaseOrder::recordPartialReceipt(const std::string& materialSku,
                                         int qtyReceived) {
  if (status != "CONFIRMED" && status != "PARTIALLY_RECEIVED") {
    throw FinanceException("Cannot receive goods for PO in status: " + status);
  }

  status = "PARTIALLY_RECEIVED";

  Logger::getInstance().log("PurchaseOrder", "Partial receipt for " + poNumber +
                                                 ": " + materialSku + " x" +
                                                 std::to_string(qtyReceived));
}

void PurchaseOrder::markCompleted() {
  if (status != "CONFIRMED" && status != "PARTIALLY_RECEIVED") {
    throw FinanceException("Cannot complete PO in status: " + status);
  }
  status = "COMPLETED";

  Logger::getInstance().log("PurchaseOrder", "Completed PO " + poNumber);
}

void PurchaseOrder::cancel() {
  if (status == "COMPLETED") {
    throw FinanceException("Cannot cancel completed purchase order");
  }
  status = "CANCELLED";

  Logger::getInstance().log("PurchaseOrder", "Cancelled PO " + poNumber);
}

int PurchaseOrder::getTotalQuantity() const {
  int total = 0;
  for (const auto& item : lineItems) {
    total += item.second;
  }
  return total;
}

void PurchaseOrder::validate() const {
  if (supplierId.empty()) {
    throw FinanceException("Supplier ID is required");
  }

  if (lineItems.empty()) {
    throw FinanceException("Purchase order must have at least one line item");
  }

  if (expectedDeliveryDate < orderDate) {
    throw FinanceException(
        "Expected delivery date cannot be before order date");
  }
}
