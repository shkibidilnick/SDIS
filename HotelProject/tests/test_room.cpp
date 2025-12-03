#include <UnitTest++/UnitTest++.h>

#include "exceptions.h"
#include "room.h"
#include "test_helpers.h"

TEST(Room_DefaultConstructor_CreatesRoomWithCorrectProperties) {
  Room room("101", RoomType::DOUBLE, 150.0, 2);

  CHECK_EQUAL("101", room.getNumber());
  CHECK_EQUAL(150.0, room.getPrice());
  CHECK_EQUAL(true, room.isAvailable());
  CHECK_EQUAL(2, room.getCapacity());
}

TEST(Room_Book_SuccessfullyBooksAvailableRoom) {
  Room room("102", RoomType::SINGLE, 100.0, 1);

  CHECK_EQUAL(true, room.book());
  CHECK_EQUAL(false, room.isAvailable());
}

TEST(Room_Book_FailsWhenRoomNotAvailable) {
  Room room("103", RoomType::SUITE, 300.0, 4);

  room.book();
  CHECK_EQUAL(false, room.book());
}

TEST(Room_Cancel_MakesRoomAvailable) {
  Room room("104", RoomType::DOUBLE, 200.0, 2);

  room.book();
  CHECK_EQUAL(false, room.isAvailable());

  room.cancel();
  CHECK_EQUAL(true, room.isAvailable());
}

TEST(Room_SetPrice_UpdatesPrice) {
  Room room("105", RoomType::SINGLE, 100.0, 1);

  room.setPrice(120.0);
  CHECK_EQUAL(120.0, room.getPrice());
}
