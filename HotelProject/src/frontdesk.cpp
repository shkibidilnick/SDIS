#include "frontdesk.h"

#include "exceptions.h"

FrontDesk::FrontDesk(std::string id, Employee* emp)
    : deskId(std::move(id)), operatorEmp(emp) {}
void FrontDesk::processCheckIn(Booking* booking) {
  if (!booking) throw BookingException("Inavalid booking");
  booking->checkedInGuest();
  checkins.push_back(booking);
}

void FrontDesk::processCheckOut(Booking* booking) {
  booking->checkedOutGuest();
  checkins.erase(std::remove(checkins.begin(), checkins.end(), booking),
                 checkins.end());
}
