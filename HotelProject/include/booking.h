#pragma once
#include <string>
#include <vector>

#include "guest.h"
#include "payment.h"
#include "room.h"

enum class BookingStatus {
  PENDING,
  CONFIRMED,
  CHECKED_IN,
  CHECKED_OUT,
  CANCELLED
};
class Booking {
 private:
  std::string id;
  Guest* guest;
  Room* room;
  std::string checkInDate;
  std::string checkOutDate;
  BookingStatus status;
  std::vector<Payment*> payments;

 public:
  Booking(std::string bookingId, Guest* g, Room* r, std::string in,
          std::string out);
  void confirm();
  void checkedInGuest();
  void checkedOutGuest();
  void cancel();
  void addPayment(Payment* p);
  double calculateTotal() const;
  Guest* getGuest() const { return guest; }
  Room* getRoom() const { return room; }
  std::string getId() const { return id; }
  BookingStatus getStatus() const { return status; }
};
