#include <UnitTest++/UnitTest++.h>

#include "event.h"
#include "exceptions.h"
#include "guest.h"
#include "test_helpers.h"

TEST(Event_RegisterCancelGuest_Works) {
  Event event("E001", "Conference", 2, 50.0);
  Guest guest1("G001", "John");
  Guest guest2("G002", "Jane");

  event.registerGuest(&guest1);
  event.registerGuest(&guest2);

  CHECK(event.isFull());

  event.cancelGuest(&guest1);
  CHECK(!event.isFull());

  CHECK_EQUAL("E001", event.getEventId());
}

TEST(Event_RegisterGuest_ThrowsWhenFull) {
  Event event("E002", "Small Event", 1, 30.0);
  Guest guest1("G001", "John");
  Guest guest2("G002", "Jane");

  event.registerGuest(&guest1);
  CHECK_THROW(event.registerGuest(&guest2), GuestException);
}
