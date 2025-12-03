#include <UnitTest++/UnitTest++.h>

#include "payment.h"
#include "test_helpers.h"

TEST(Payment_Constructor_CreatesPaymentWithCorrectProperties) {
  Payment payment("P001", "B001", 150.0, PaymentMethod::CARD);

  CHECK_EQUAL("P001", payment.getId());
  CHECK_EQUAL(150.0, payment.getAmount());
  CHECK_EQUAL(false, payment.isCompleted());
}

TEST(Payment_ProcessPayment_SuccessfullyProcesses) {
  Payment payment("P002", "B002", 200.0, PaymentMethod::CASH);

  payment.processPayment();
  CHECK_EQUAL(true, payment.isCompleted());
}

TEST(Payment_ProcessPayment_ThrowsWhenAlreadyCompleted) {
  Payment payment("P003", "B003", 300.0, PaymentMethod::TRANSFER);

  payment.processPayment();
  CHECK_THROW(payment.processPayment(), PaymentException);
}

TEST(Payment_Refund_SuccessfullyRefunds) {
  Payment payment("P004", "B004", 400.0, PaymentMethod::CARD);

  payment.processPayment();
  payment.refund();
  CHECK_EQUAL(false, payment.isCompleted());
}
