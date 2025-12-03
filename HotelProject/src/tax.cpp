#include "tax.h"

#include "exceptions.h"
#include "invoice.h"
#include "payment.h"

static std::string taxTypeToString(TaxType t) {
  switch (t) {
    case TaxType::VAT:
      return "VAT";
    case TaxType::SERVICE:
      return "Service Charge";
    case TaxType::TOURIST:
      return "Tourist Tax";
  }
  return "Unknown";
}

Tax::Tax(std::string id, TaxType t, double r)
    : taxId(std::move(id)),
      type(t),
      rate(r),
      invoice(nullptr),
      payment(nullptr),
      calculatedAmount(0.0) {
  if (r < 0.0 || r > 0.5) {
    throw TaxException("Invalid tax rate: " + std::to_string(r));
  }
}

void Tax::apply(Invoice* inv, Payment* pay) {
  if (!inv || !pay) {
    throw TaxException("Cannot apply tax to null objects");
  }

  invoice = inv;
  payment = pay;

  calculatedAmount = calculate(pay->getAmount());

  inv->addTax(this);

  std::ofstream logFile("tax.log", std::ios::app);
  if (logFile.is_open()) {
    logFile << "[Tax Applied] ID: " << taxId
            << " Type: " << taxTypeToString(type) << " Rate: " << (rate * 100)
            << "%"
            << " Amount: $" << calculatedAmount
            << " Invoice: " << inv->getInvoiceId() << "\n";
    logFile.close();
  }
}

double Tax::calculate(double baseAmount) const {
  if (payment && !payment->isCompleted()) {
    return 0.0;
  }
  return baseAmount * rate;
}

std::string Tax::getDescription() const {
  return taxTypeToString(type) + " (" +
         std::to_string(static_cast<int>(rate * 100)) + "%)";
}
