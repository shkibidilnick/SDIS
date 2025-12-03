#include <UnitTest++/UnitTest++.h>

#include "booking.h"
#include "exceptions.h"
#include "guest.h"
#include "room.h"
#include "test_helpers.h"

TEST(Booking_Constructor_CreatesBookingWithCorrectProperties) {
  Guest guest("G001", "John Doe");
  Room room("101", RoomType::DOUBLE, 150.0, 2);
  Booking booking("B001", &guest, &room, "2024-01-01", "2024-01-03");

  CHECK_EQUAL("B001", booking.getId());
  CHECK_EQUAL(&guest, booking.getGuest());
  CHECK_EQUAL(&room, booking.getRoom());
  CHECK_EQUAL(BookingStatus::PENDING, booking.getStatus());
}

TEST(Booking_Confirm_SuccessfullyConfirmsPendingBooking) {
  Guest guest("G002", "Jane Smith");
  Room room("102", RoomType::SINGLE, 100.0, 1);
  Booking booking("B002", &guest, &room, "2024-01-01", "2024-01-03");

  // Комната должна быть доступна до подтверждения
  CHECK_EQUAL(true, room.isAvailable());

  booking.confirm();

  // После подтверждения статус должен измениться
  CHECK_EQUAL(BookingStatus::CONFIRMED, booking.getStatus());

  // Комната все еще должна быть доступна, т.к. бронирование теперь не меняет
  // состояние комнаты Бронирование комнаты происходит в Hotel::checkInGuest
  CHECK_EQUAL(true, room.isAvailable());
}

TEST(Booking_Confirm_ThrowsWhenAlreadyProcessed) {
  Guest guest("G003", "Bob Wilson");
  Room room("103", RoomType::SUITE, 300.0, 4);
  Booking booking("B003", &guest, &room, "2024-01-01", "2024-01-03");

  booking.confirm();
  CHECK_THROW(booking.confirm(), BookingException);
}

TEST(Booking_CheckedInGuest_UpdatesStatusFromConfirmed) {
  Guest guest("G004", "Alice Brown");
  Room room("104", RoomType::DOUBLE, 200.0, 2);
  Booking booking("B004", &guest, &room, "2024-01-01", "2024-01-03");

  booking.confirm();
  booking.checkedInGuest();
  CHECK_EQUAL(BookingStatus::CHECKED_IN, booking.getStatus());
}

TEST(Booking_CheckedInGuest_ThrowsWhenNotConfirmed) {
  Guest guest("G005", "Charlie Davis");
  Room room("105", RoomType::SINGLE, 100.0, 1);
  Booking booking("B005", &guest, &room, "2024-01-01", "2024-01-03");

  CHECK_THROW(booking.checkedInGuest(), BookingException);
}

TEST(Booking_CheckedOutGuest_UpdatesStatusAndReleasesRoom) {
  Guest guest("G006", "Diana Evans");
  Room room("106", RoomType::DOUBLE, 180.0, 2);
  Booking booking("B006", &guest, &room, "2024-01-01", "2024-01-03");

  booking.confirm();
  booking.checkedInGuest();
  booking.checkedOutGuest();

  CHECK_EQUAL(BookingStatus::CHECKED_OUT, booking.getStatus());
  CHECK_EQUAL(true, room.isAvailable());
}

TEST(Booking_Cancel_ReleasesRoomAndUpdatesStatus) {
  Guest guest("G007", "Edward Harris");
  Room room("107", RoomType::SINGLE, 120.0, 1);
  Booking booking("B007", &guest, &room, "2024-01-01", "2024-01-03");

  booking.confirm();
  booking.cancel();

  CHECK_EQUAL(BookingStatus::CANCELLED, booking.getStatus());
  CHECK_EQUAL(true, room.isAvailable());
}

TEST(Booking_Cancel_ThrowsWhenCheckedIn) {
  Guest guest("G008", "Fiona Clark");
  Room room("108", RoomType::DOUBLE, 220.0, 2);
  Booking booking("B008", &guest, &room, "2024-01-01", "2024-01-03");

  booking.confirm();
  booking.checkedInGuest();

  CHECK_THROW(booking.cancel(), BookingException);
}

TEST(Booking_CalculateTotal_ReturnsCorrectAmount) {
  Guest guest("G009", "George Lewis");
  Room room("109", RoomType::SUITE, 350.0, 4);
  Booking booking("B009", &guest, &room, "2024-01-01", "2024-01-03");

  double total = booking.calculateTotal();
  CHECK_EQUAL(1050.0, total);  // 350 * 3 nights
}
