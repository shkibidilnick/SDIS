#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FinanceException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IValidatable.h"
#include "RowMaterial.h"

// Represents a purchase order for procuring raw materials from suppliers
// Tracks order lifecycle from draft to completion with financial details
class PurchaseOrder : public ITrackable, public IValidatable {
 private:
  std::string poNumber;
  std::string supplierId;
  std::string orderDate;
  std::string expectedDeliveryDate;
  std::string status;  // "DRAFT", "SENT", "CONFIRMED", "PARTIALLY_RECEIVED",
                       // "COMPLETED", "CANCELLED"
  std::vector<std::pair<std::shared_ptr<RawMaterial>, int>>
      lineItems;  // Material, Quantity
  double subtotal;
  double taxAmount;
  double totalAmount;
  bool isApproved;

 public:
  PurchaseOrder(const std::string& supplierId,
                const std::string& expectedDeliveryDate);

  // Adds a material and quantity to the order
  void addLineItem(std::shared_ptr<RawMaterial> material, int quantity);

  // Calculates order totals with tax
  void calculateTotals(double taxRate);

  // Approves and sends order to supplier
  void approve();

  // Marks as confirmed by supplier
  void markConfirmed();

  // Records partial goods receipt
  void recordPartialReceipt(const std::string& materialSku,
                            int quantityReceived);

  // Marks order as fully received
  void markCompleted();

  // Cancels the order
  void cancel();

  // Gets total quantity across all items
  int getTotalQuantity() const;

  // ITrackable implementation
  std::string getTrackingId() const override { return poNumber; }
  std::string getStatus() const override { return status; }

  // IValidatable implementation
  void validate() const override;

  std::string getPoNumber() const { return poNumber; }
  std::string getSupplierId() const { return supplierId; }
  double getTotalAmount() const { return totalAmount; }
  bool getIsApproved() const { return isApproved; }
};
