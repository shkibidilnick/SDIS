#include "hotel.h"

#include "iostream"

Hotel::Hotel(std::string hotelName) : name(std::move(hotelName)) {}

Hotel::~Hotel() {
  for (auto* r : rooms) delete r;
  for (auto* g : guests) delete g;
  for (auto* e : staff) delete e;
  for (auto* b : bookings) delete b;
  for (auto* a : amenities) delete a;
}

void Hotel::addRoom(Room* room) {
  if (!room) throw HotelException("Cannot add null room");
  rooms.push_back(room);
  roomIndex[room->getNumber()] = room;
}

void Hotel::addGuest(Guest* guest) {
  if (!guest) throw HotelException("Cannot add null guest");
  guests.push_back(guest);
  guestIndex[guest->getId()] = guest;
}

void Hotel::addEmployee(Employee* employee) {
  if (!employee) throw HotelException("Cannot add null employee");
  staff.push_back(employee);
  employeeIndex[employee->getId()] = employee;
}

void Hotel::addBooking(Booking* booking) {
  if (!booking) throw HotelException("Cannot add null booking");
  bookings.push_back(booking);
}

void Hotel::addAmenity(Amenity* amenity) {
  if (!amenity) throw HotelException("Cannot add null amenity");
  amenities.push_back(amenity);
}

Room* Hotel::findRoom(const std::string& number) const {
  auto it = roomIndex.find(number);
  return (it != roomIndex.end()) ? it->second : nullptr;
}

Guest* Hotel::findGuest(const std::string& id) const {
  auto it = guestIndex.find(id);
  return (it != guestIndex.end()) ? it->second : nullptr;
}

Employee* Hotel::findEmployee(const std::string& id) const {
  auto it = employeeIndex.find(id);
  return (it != employeeIndex.end()) ? it->second : nullptr;
}

Amenity* Hotel::findAmenity(const std::string& id) const {
  for (auto* a : amenities) {
    if (a && a->getAmenityId() == id) return a;
  }
  return nullptr;
}

bool Hotel::isRoomAvailable(const std::string& roomNumber) const {
  Room* room = findRoom(roomNumber);
  return room && room->isAvailable();
}

void Hotel::checkInGuest(const std::string& guestId,
                         const std::string& roomNumber) {
  Guest* guest = findGuest(guestId);
  if (!guest) throw HotelException("Guest not found: " + guestId);

  Room* room = findRoom(roomNumber);
  if (!room) throw HotelException("Room not found: " + roomNumber);

  if (!room->isAvailable()) throw HotelException("Room not available");

  if (!room->book()) throw HotelException("Room not available");

  Booking* booking = new Booking("BKG_" + guestId + "_" + roomNumber, guest,
                                 room, "2024-12-01", "2024-12-03");
  try {
    booking->confirm();
    bookings.push_back(booking);
    guest->checkIn(roomNumber);
  } catch (...) {
    room->cancel();
    delete booking;
    throw;
  }

  std::ofstream log("hotel.log", std::ios::app);
  if (log.is_open()) {
    log << "[Check-in] Guest: " << guestId << " -> Room: " << roomNumber
        << "\n";
    log.close();
  }
}

void Hotel::checkOutGuest(const std::string& roomNumber) {
  Room* room = findRoom(roomNumber);
  if (!room) throw HotelException("Room not found");

  Guest* guest = nullptr;
  for (auto* g : guests) {
    if (g && g->getCurrentRoom() == roomNumber) {
      guest = g;
      break;
    }
  }
  if (!guest) throw HotelException("No guest in room " + roomNumber);

  room->cancel();
  guest->checkOut();

  std::ofstream log("hotel.log", std::ios::app);
  if (log.is_open()) {
    log << "[Check-out] Room: " << roomNumber << " -> Guest removed\n";
    log.close();
  }
}

Report* Hotel::generateOccupancyReport(const std::string& reportId) const {
  Report* report = new Report(reportId, ReportType::OCCUPANCY);
  for (const auto* b : bookings) {
    report->addBooking(const_cast<Booking*>(b));
  }
  return report;
}

double Hotel::getTotalRevenue() const {
  double revenue = 0.0;
  for (const auto* b : bookings) {
    if (b && b->getStatus() == BookingStatus::CHECKED_OUT) {
      revenue += 100.0;
    }
  }
  return revenue;
}

int Hotel::getAvailableRoomCount() const {
  int count = 0;
  for (const auto* room : rooms) {
    if (room && room->isAvailable()) ++count;
  }
  return count;
}
