#include "invoice.h"
#include "tax.h"
#include "penalty.h"
#include "exceptions.h"

Invoice::Invoice(std::string id, Booking* b)
    : invoiceId(std::move(id)),
      booking(b),
      totalAmount(0.0),
      taxAmount(0.0),
      penaltyAmount(0.0),
      isPaid(false) {}
void Invoice::addPayment(Payment* payment) { payments.push_back(payment); }
void Invoice::addTax(Tax* tax) {
  appliedTaxes.push_back(tax);
  taxAmount += tax->getCalculatedAmount();
}
void Invoice::addPenalty(Penalty* penalty) {
  if (!penalty) throw InvoiceException("Invalid penalty");
  appliedPenalties.push_back(penalty);
  penaltyAmount += penalty->getCalculatedAmount();
}
void Invoice::markAsPaid() { isPaid = true; }
double Invoice::getRemaining() const {
  return isPaid ? 0.0 : (totalAmount + taxAmount + penaltyAmount);
}
