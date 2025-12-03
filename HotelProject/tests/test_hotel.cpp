#include <UnitTest++/UnitTest++.h>

#include "amenity.h"
#include "employee.h"
#include "exceptions.h"
#include "guest.h"
#include "hotel.h"
#include "room.h"
#include "test_helpers.h"

TEST(Hotel_Constructor_CreatesHotelWithCorrectName) {
  Hotel hotel("Grand Hotel");

  CHECK_EQUAL("Grand Hotel", hotel.getName());
  CHECK_EQUAL(0, hotel.getTotalRooms());
  CHECK_EQUAL(0, hotel.getTotalGuests());
  CHECK_EQUAL(0, hotel.getTotalBookings());
}

TEST(Hotel_AddRoom_AddsRoomToHotel) {
  Hotel hotel("Test Hotel");
  Room* room = new Room("101", RoomType::SINGLE, 100.0, 1);

  hotel.addRoom(room);
  CHECK_EQUAL(1, hotel.getTotalRooms());
  CHECK_EQUAL(room, hotel.findRoom("101"));
}

TEST(Hotel_AddRoom_ThrowsWhenNull) {
  Hotel hotel("Test Hotel");

  CHECK_THROW(hotel.addRoom(nullptr), HotelException);
}

TEST(Hotel_AddGuest_AddsGuestToHotel) {
  Hotel hotel("Test Hotel");
  Guest* guest = new Guest("G001", "John Doe");

  hotel.addGuest(guest);
  CHECK_EQUAL(1, hotel.getTotalGuests());
  CHECK_EQUAL(guest, hotel.findGuest("G001"));
}

TEST(Hotel_AddEmployee_AddsEmployeeToHotel) {
  Hotel hotel("Test Hotel");
  Employee* employee = new Employee("E001", "Alice Manager", Role::MANAGER);

  hotel.addEmployee(employee);
  CHECK_EQUAL(employee, hotel.findEmployee("E001"));
}

TEST(Hotel_IsRoomAvailable_ReturnsCorrectStatus) {
  Hotel hotel("Test Hotel");
  Room* room = new Room("201", RoomType::DOUBLE, 150.0, 2);
  hotel.addRoom(room);

  // Комната должна быть доступна после добавления
  CHECK_EQUAL(true, hotel.isRoomAvailable("201"));

  // Теперь забронируем комнату через Hotel
  Guest* guest = new Guest("G201", "Test Guest");
  hotel.addGuest(guest);

  // Проверяем, что комната доступна перед бронированием
  CHECK_EQUAL(true, hotel.isRoomAvailable("201"));

  // Бронируем комнату
  hotel.checkInGuest("G201", "201");

  // Теперь комната должна быть недоступна
  CHECK_EQUAL(false, hotel.isRoomAvailable("201"));
}

TEST(Hotel_CheckInGuest_SuccessfullyChecksInGuest) {
  Hotel hotel("Test Hotel");

  // Создаем и добавляем гостя и комнату в отель
  Guest* guest = new Guest("G002", "Jane Smith");
  Room* room = new Room("202", RoomType::SINGLE, 100.0, 1);

  hotel.addGuest(guest);
  hotel.addRoom(room);

  // Убедимся, что комната доступна перед заселением
  CHECK_EQUAL(true, hotel.isRoomAvailable("202"));

  hotel.checkInGuest("G002", "202");

  // Проверяем, что гость заселен и комната занята
  CHECK_EQUAL("202", guest->getCurrentRoom());
  CHECK_EQUAL(false, hotel.isRoomAvailable("202"));
}

TEST(Hotel_CheckInGuest_ThrowsWhenGuestNotFound) {
  Hotel hotel("Test Hotel");
  Room* room = new Room("203", RoomType::DOUBLE, 150.0, 2);
  hotel.addRoom(room);

  CHECK_THROW(hotel.checkInGuest("NONEXISTENT", "203"), HotelException);
}

TEST(Hotel_CheckInGuest_ThrowsWhenRoomNotFound) {
  Hotel hotel("Test Hotel");
  Guest* guest = new Guest("G003", "Bob Wilson");
  hotel.addGuest(guest);

  CHECK_THROW(hotel.checkInGuest("G003", "999"), HotelException);
}

TEST(Hotel_CheckOutGuest_SuccessfullyChecksOutGuest) {
  Hotel hotel("Test Hotel");

  // Создаем и добавляем гостя и комнату
  Guest* guest = new Guest("G004", "Alice Brown");
  Room* room = new Room("204", RoomType::SINGLE, 100.0, 1);

  hotel.addGuest(guest);
  hotel.addRoom(room);

  // Сначала заселяем гостя
  hotel.checkInGuest("G004", "204");
  CHECK_EQUAL("204", guest->getCurrentRoom());
  CHECK_EQUAL(false, hotel.isRoomAvailable("204"));

  // Затем выселяем
  hotel.checkOutGuest("204");

  // Проверяем, что гость выселен и комната доступна
  CHECK_EQUAL("", guest->getCurrentRoom());
  CHECK_EQUAL(true, hotel.isRoomAvailable("204"));
}

TEST(Hotel_GetTotalRevenue_ReturnsCorrectAmount) {
  Hotel hotel("Test Hotel");

  // Просто проверяем, что метод работает без ошибок
  double revenue = hotel.getTotalRevenue();
  CHECK_CLOSE(0.0, revenue, 0.01);
}

TEST(Hotel_GetAvailableRoomCount_ReturnsCorrectCount) {
  Hotel hotel("Test Hotel");

  // Добавляем несколько комнат
  Room* room1 = new Room("207", RoomType::SINGLE, 100.0, 1);
  Room* room2 = new Room("208", RoomType::DOUBLE, 150.0, 2);
  Room* room3 = new Room("209", RoomType::SUITE, 300.0, 4);

  hotel.addRoom(room1);
  hotel.addRoom(room2);
  hotel.addRoom(room3);

  // Все комнаты должны быть доступны
  CHECK_EQUAL(3, hotel.getAvailableRoomCount());

  // Бронируем одну комнату
  Guest* guest = new Guest("G207", "Test Guest");
  hotel.addGuest(guest);
  hotel.checkInGuest("G207", "208");

  // Теперь должно быть 2 доступные комнаты
  CHECK_EQUAL(2, hotel.getAvailableRoomCount());
}

TEST(Hotel_FindAmenity_ReturnsCorrectAmenity) {
  Hotel hotel("Test Hotel");
  Amenity* amenity = new Amenity("A001", "WiFi");

  hotel.addAmenity(amenity);
  CHECK_EQUAL(amenity, hotel.findAmenity("A001"));
  CHECK_EQUAL(nullptr, hotel.findAmenity("NONEXISTENT"));
}

TEST(Hotel_GenerateOccupancyReport_ReturnsReport) {
  Hotel hotel("Test Hotel");

  Report* report = hotel.generateOccupancyReport("R001");
  CHECK(report != nullptr);
  CHECK_EQUAL("R001", report->getReportId());

  delete report;
}

// Простой тест для проверки базовой функциональности
TEST(Hotel_SimpleCheckInCheckOut_Works) {
  Hotel hotel("Simple Hotel");

  // Создаем и добавляем гостя и комнату
  Guest* guest = new Guest("SIMPLE01", "Simple Guest");
  Room* room = new Room("SIMPLE01", RoomType::SINGLE, 100.0, 1);

  hotel.addGuest(guest);
  hotel.addRoom(room);

  // Проверяем начальное состояние
  CHECK_EQUAL(true, hotel.isRoomAvailable("SIMPLE01"));
  CHECK_EQUAL("", guest->getCurrentRoom());

  // Заселяем
  hotel.checkInGuest("SIMPLE01", "SIMPLE01");

  // Проверяем состояние после заселения
  CHECK_EQUAL("SIMPLE01", guest->getCurrentRoom());
  CHECK_EQUAL(false, hotel.isRoomAvailable("SIMPLE01"));

  // Выселяем
  hotel.checkOutGuest("SIMPLE01");

  // Проверяем состояние после выселения
  CHECK_EQUAL("", guest->getCurrentRoom());
  CHECK_EQUAL(true, hotel.isRoomAvailable("SIMPLE01"));
}
