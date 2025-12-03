#include <UnitTest++/UnitTest++.h>

#include "booking.h"
#include "deposit.h"
#include "guest.h"
#include "payment.h"
#include "room.h"
#include "test_helpers.h"

TEST(Deposit_HoldRelease_Works) {
  Guest guest("G001", "John");
  Room room("101", RoomType::SINGLE, 100.0, 1);
  Booking booking("B001", &guest, &room, "2024-01-01", "2024-01-03");
  Payment payment("P001", "B001", 50.0, PaymentMethod::CARD);

  Deposit deposit("D001", &booking, &payment, 50.0);

  deposit.hold();
  CHECK(payment.isCompleted());

  deposit.release();

  CHECK_EQUAL("D001", deposit.getDepositId());
  CHECK_EQUAL(50.0, deposit.getAmount());
}
