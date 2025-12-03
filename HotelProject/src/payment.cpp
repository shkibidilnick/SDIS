#include "payment.h"

Payment::Payment(std::string paymentId, std::string bid, double amt,
                 PaymentMethod m)
    : id(std::move(paymentId)),
      bookingId(std::move(bid)),
      amount(amt),
      method(m),
      completed(false) {}
void Payment::processPayment() {
  if (completed) throw PaymentException("Already completed");
  completed = true;
}
void Payment::refund() {
  if (!completed) throw PaymentException("Not completed");
  completed = false;
}
