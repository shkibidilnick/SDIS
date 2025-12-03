#include <UnitTest++/UnitTest++.h>

#include "guest.h"
#include "test_helpers.h"

TEST(Guest_DefaultConstructor_CreatesGuestWithCorrectProperties) {
  Guest guest("G001", "John Doe");

  CHECK_EQUAL("G001", guest.getId());
  CHECK_EQUAL("John Doe", guest.getName());
  CHECK_EQUAL(0, guest.getPoints());
  CHECK_EQUAL(false, guest.getVIPStatus());
  CHECK_EQUAL("", guest.getCurrentRoom());
}

TEST(Guest_AddPoints_IncreasesLoyaltyPoints) {
  Guest guest("G002", "Jane Smith");

  guest.addPoints(500);
  CHECK_EQUAL(500, guest.getPoints());

  guest.addPoints(300);
  CHECK_EQUAL(800, guest.getPoints());
}

TEST(Guest_MakeVIP_UpdatesVIPStatusBasedOnPoints) {
  Guest guest("G003", "Bob Wilson");

  // Not enough points for VIP
  guest.addPoints(500);
  guest.makeVIP();
  CHECK_EQUAL(false, guest.getVIPStatus());

  // Enough points for VIP
  guest.addPoints(600);  // Total 1100 points
  guest.makeVIP();
  CHECK_EQUAL(true, guest.getVIPStatus());
}

TEST(Guest_UpdateContact_ChangesEmailAndPhone) {
  Guest guest("G004", "Alice Brown");

  guest.updateContact("alice@email.com", "123-456-7890");
  // Note: You'll need to add getter methods for email and phone in Guest class
}

TEST(Guest_CheckIn_AssignsCurrentRoom) {
  Guest guest("G005", "Charlie Davis");

  guest.checkIn("101");
  CHECK_EQUAL("101", guest.getCurrentRoom());
}

TEST(Guest_CheckOut_ClearsCurrentRoom) {
  Guest guest("G006", "Diana Evans");

  guest.checkIn("102");
  guest.checkOut();
  CHECK_EQUAL("", guest.getCurrentRoom());
}
