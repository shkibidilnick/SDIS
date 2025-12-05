#include "Shift.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FactoryException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"

Shift::Shift(const std::string& empId, const std::string& shiftDate,
             double start, double end)
    : employeeId(empId),
      date(shiftDate),
      startHour(start),
      endHour(end),
      isApproved(false) {
  shiftId = IDGenerator::generate("SHIFT");

  if (startHour < 0 || startHour > 24 || endHour < 0 || endHour > 24) {
    throw FactoryException("Invalid shift hours: " + std::to_string(startHour) +
                           "-" + std::to_string(endHour));
  }

  if (startHour >= endHour) {
    throw FactoryException("Shift end must be after start");
  }
}

double Shift::calculateCost() const {
  // rate For now, assuming $15/hour average
  double hourlyRate = 15.0;
  return getHours() * hourlyRate;
}
