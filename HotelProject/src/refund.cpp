#include "refund.h"
Refund::Refund(std::string id, Payment* p, Booking* b, double amt,
               std::string r)
    : refundId(std::move(id)),
      originalPayment(p),
      booking(b),
      amount(amt),
      reason(std::move(r)) {}
void Refund::process() {
  if (!originalPayment) {
    throw PaymentException("Original payment is null");
  }
  if (!originalPayment->isCompleted()) {
    throw PaymentException("Cannot refund incomplete payment");
  }
  amount = originalPayment->getAmount() * 0.8;
  originalPayment->refund();
}
