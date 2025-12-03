#pragma once  // tax - налоги
#include <string>
#include<fstream>
#include "exceptions.h"

class Invoice;
class Payment;

enum class TaxType { VAT, SERVICE, TOURIST };  // VAT - НДС
class Tax {
 private:
  std::string taxId;
  TaxType type;
  double rate;
  Invoice* invoice;
  Payment* payment;
  bool isApplied;
  double calculatedAmount;

 public:
  Tax(std::string id, TaxType t, double r);
  void apply(Invoice* inv, Payment* pay);
  double calculate(double amount) const;
  std::string getTaxId() const { return taxId; }
  std::string getDescription() const;
  double getCalculatedAmount() const { return calculatedAmount; }
};
