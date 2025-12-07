#include "Invoice.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

Invoice::Invoice(const std::string& custId, const std::string& dueDate,
                 const std::string& poNumber)
    : customerId(custId),
      dueDate(dueDate),
      taxAmount(0.0),
      totalAmount(0.0),
      isPaid(false),
      purchaseOrderNumber(poNumber) {
  invoiceId = IDGenerator::generate("INV");
  issueDate = DateTimeUtils::getCurrentTimestamp();

  if (custId.empty()) {
    throw FinanceException("Customer ID is required for invoice");
  }

  if (!DateTimeUtils::isValidDate(dueDate)) {
    throw FinanceException("Invalid due date format: " + dueDate);
  }

  Logger::getInstance().log(
      "Invoice", "Created invoice " + invoiceId + " for customer: " + custId);
}

void Invoice::addLineItem(const std::string& description, double amount) {
  if (isPaid) {
    throw FinanceException("Cannot modify paid invoice: " + invoiceId);
  }

  if (description.empty()) {
    throw FinanceException("Line item description cannot be empty");
  }

  Validator::validatePositive(amount, "Line item amount");

  lineItems.push_back({description, amount});

  Logger::getInstance().log(
      "Invoice", "Added line item to " + invoiceId + ": " + description);
}

void Invoice::calculateTotals(double taxRate) {
  if (isPaid) {
    throw FinanceException("Cannot calculate totals on paid invoice: " +
                           invoiceId);
  }

  Validator::validatePercentage(taxRate, "Tax rate");

  double subtotal = 0.0;
  for (const auto& item : lineItems) {
    subtotal += item.second;
  }

  taxAmount = subtotal * (taxRate / 100.0);
  totalAmount = subtotal + taxAmount;

  Logger::getInstance().log(
      "Invoice", "Calculated totals for " + invoiceId + ": Subtotal=$" +
                     std::to_string(subtotal) + ", Tax=$" +
                     std::to_string(taxAmount) + ", Total=$" +
                     std::to_string(totalAmount));
}

void Invoice::markAsPaid() {
  if (isPaid) {
    throw FinanceException("Invoice already paid: " + invoiceId);
  }

  isPaid = true;

  Logger::getInstance().log("Invoice",
                            "Marked invoice " + invoiceId + " as PAID");
}

bool Invoice::isOverdue() const {
  if (isPaid) return false;

  // Simplified: Compare dates (in production use proper date comparison)
  std::string currentDate = DateTimeUtils::getCurrentTimestamp().substr(0, 10);
  return currentDate > dueDate;
}
