#pragma once
#include <string>
#include <vector>

#include "booking.h"
#include "employee.h"

class FrontDesk {
 private:
  std::string deskId;
  Employee* operatorEmp;
  std::vector<Booking*> checkins;

 public:
  FrontDesk(std::string id, Employee* emp);
  void processCheckIn(Booking* booking);
  void processCheckOut(Booking* booking);
  std::string getDeskId() const { return deskId; }
};
