#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FinanceException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"

// Represents an invoice for products or services
// Tracks billing status and payment history
class Invoice : public ITrackable {
 private:
  std::string invoiceId;
  std::string customerId;
  std::string issueDate;
  std::string dueDate;
  std::vector<std::pair<std::string, double>> lineItems;  // Description, Amount
  double taxAmount;
  double totalAmount;
  bool isPaid;
  std::string purchaseOrderNumber;

 public:
  Invoice(const std::string& customerId, const std::string& dueDate,
          const std::string& poNumber = "");

  // Adds a line item to the invoice
  void addLineItem(const std::string& description, double amount);

  // Calculates totals including tax
  void calculateTotals(double taxRate);

  // Marks invoice as paid
  void markAsPaid();

  // Checks if invoice is overdue
  bool isOverdue() const;

  // ITrackable implementation
  std::string getTrackingId() const override { return invoiceId; }
  std::string getStatus() const override {
    return isPaid ? "PAID" : "OUTSTANDING";
  }

  std::string getInvoiceId() const { return invoiceId; }
  std::string getCustomerId() const { return customerId; }
  double getTotalAmount() const { return totalAmount; }
  bool getIsPaid() const { return isPaid; }
  std::string getPurchaseOrderNumber() const { return purchaseOrderNumber; }
};
