#pragma once
#include <memory>
#include <string>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FinanceException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"

// Represents a financial transaction (payment, receipt, transfer)
// Immutable once created for audit compliance
class Transaction : public ITrackable {
 private:
  std::string transactionId;
  std::string type;  // "PAYMENT", "RECEIPT", "TRANSFER"
  double amount;
  std::string sourceAccount;
  std::string destinationAccount;
  std::string date;
  bool isPosted;          // True when transaction is finalized
  std::string reference;  // Invoice ID, PO number, etc.

 public:
  Transaction(const std::string& type, double amount, const std::string& source,
              const std::string& dest, const std::string& ref);

  // Posts the transaction (makes it permanent)
  // Throws FinanceException if validation fails
  void post();

  // Reverses the transaction (creates a reversal entry)
  std::shared_ptr<Transaction> reverse() const;

  // Validates transaction data
  void validate() const;

  // ITrackable implementation
  std::string getTrackingId() const override { return transactionId; }
  std::string getStatus() const override {
    return isPosted ? "POSTED" : "PENDING";
  }

  double getAmount() const { return amount; }
  std::string getType() const { return type; }
  bool getIsPosted() const { return isPosted; }
};
