#include "booking.h"

#include "exceptions.h"

Booking::Booking(std::string bookingId, Guest* g, Room* r, std::string in,
                 std::string out)
    : id(std::move(bookingId)),
      guest(g),
      room(r),
      checkInDate(std::move(in)),
      checkOutDate(std::move(out)),
      status(BookingStatus::PENDING) {}
void Booking::confirm() {
  if (status != BookingStatus::PENDING) {
    throw BookingException("Booking already processed");
  }
  status = BookingStatus::CONFIRMED;
}

void Booking::checkedInGuest() {
  if (status != BookingStatus::CONFIRMED)
    throw BookingException("Cannot check in");
  status = BookingStatus::CHECKED_IN;
}

void Booking::checkedOutGuest() {
  status = BookingStatus::CHECKED_OUT;
  room->cancel();
}

void Booking::cancel() {
  if (status == BookingStatus::CHECKED_IN)
    throw BookingException("Cannot cancel active");
  room->cancel();
  status = BookingStatus::CANCELLED;
}

void Booking::addPayment(Payment* p) { payments.push_back(p); }

double Booking::calculateTotal() const {
  return room->getPrice() * 3;  // 3 nights default
}
