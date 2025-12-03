#include <UnitTest++/UnitTest++.h>

#include "booking.h"
#include "employee.h"
#include "frontdesk.h"
#include "guest.h"
#include "room.h"
#include "test_helpers.h"

TEST(FrontDesk_CheckInCheckOut_Works) {
  Employee employee("E001", "Receptionist", Role::RECEPTIONIST);
  FrontDesk frontDesk("FD001", &employee);

  Guest guest("G001", "John");
  Room room("101", RoomType::SINGLE, 100.0, 1);
  Booking booking("B001", &guest, &room, "2024-01-01", "2024-01-03");

  booking.confirm();
  frontDesk.processCheckIn(&booking);
  CHECK(booking.getStatus() == BookingStatus::CHECKED_IN);

  frontDesk.processCheckOut(&booking);
  CHECK(booking.getStatus() == BookingStatus::CHECKED_OUT);

  CHECK_EQUAL("FD001", frontDesk.getDeskId());
}
