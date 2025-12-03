#include <UnitTest++/UnitTest++.h>

#include "guest.h"
#include "keycard.h"
#include "room.h"
#include "test_helpers.h"

TEST(Keycard_ActivateDeactivate_Works) {
  Room room("101", RoomType::SINGLE, 100.0, 1);
  Guest guest("G001", "John");
  Keycard card("KC001", &room, &guest);

  CHECK(card.getIsActive());

  card.deactivate();
  CHECK(!card.getIsActive());

  card.activate();
  CHECK(card.getIsActive());

  CHECK_EQUAL("KC001", card.getCardId());
}
