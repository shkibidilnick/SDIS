#include <UnitTest++/UnitTest++.h>

#include "amenity.h"
#include "room.h"
#include "test_helpers.h"

TEST(Amenity_AddRemoveRoom_Works) {
  Amenity amenity("A001", "WiFi");
  Room room("101", RoomType::SINGLE, 100.0, 1);

  amenity.addRoom(&room);
  amenity.removeRoom(&room);

  CHECK_EQUAL("A001", amenity.getAmenityId());
  CHECK_EQUAL("WiFi", amenity.getName());
}
