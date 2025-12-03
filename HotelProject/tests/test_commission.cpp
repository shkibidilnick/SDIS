#include <UnitTest++/UnitTest++.h>

#include "booking.h"
#include "commission.h"
#include "guest.h"
#include "payment.h"
#include "room.h"
#include "test_helpers.h"

TEST(Commission_Calculate_Works) {
  Guest guest("G001", "John");
  Room room("101", RoomType::SINGLE, 100.0, 1);
  Booking booking("B001", &guest, &room, "2024-01-01", "2024-01-03");
  Payment payment("P001", "B001", 100.0, PaymentMethod::CARD);

  payment.processPayment();

  Commission commission("C001", &booking, &payment, 0.1);
  commission.calculate();

  CHECK_EQUAL(10.0, commission.getAmount());
  CHECK_EQUAL("C001", commission.getCommission());
}
