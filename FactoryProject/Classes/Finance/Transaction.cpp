#include "Transaction.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

Transaction::Transaction(const std::string& t, double amt,
                         const std::string& src, const std::string& dest,
                         const std::string& ref)
    : type(t),
      amount(amt),
      sourceAccount(src),
      destinationAccount(dest),
      reference(ref),
      isPosted(false) {
  transactionId = IDGenerator::generate("TXN");

  if (type.empty()) {
    throw FinanceException("Transaction type cannot be empty");
  }

  Validator::validatePositive(amount, "Transaction amount");
  validate();

  Logger::getInstance().log(
      "Transaction", "Created " + type + " transaction " + transactionId);
}

void Transaction::post() {
  if (isPosted) {
    throw FinanceException("Transaction already posted: " + transactionId);
  }

  validate();
  isPosted = true;

  Logger::getInstance().log("Transaction",
                            "Posted transaction " + transactionId);
}

std::shared_ptr<Transaction> Transaction::reverse() const {
  if (!isPosted) {
    throw FinanceException("Cannot reverse unposted transaction: " +
                           transactionId);
  }

  // Create reversal transaction
  auto reversal =
      std::make_shared<Transaction>("REVERSAL", amount, destinationAccount,
                                    sourceAccount, "REV_" + transactionId);

  reversal->post();

  Logger::getInstance().log("Transaction",
                            "Reversed transaction " + transactionId);

  return reversal;
}

void Transaction::validate() const {
  Validator::validatePositive(amount, "Transaction amount");

  if (sourceAccount.empty() || destinationAccount.empty()) {
    throw FinanceException("Source and destination accounts are required");
  }

  if (sourceAccount == destinationAccount) {
    throw FinanceException(
        "Source and destination accounts cannot be the same");
  }
}
