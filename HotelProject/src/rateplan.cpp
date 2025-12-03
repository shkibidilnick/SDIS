#include "rateplan.h"

RatePlan::RatePlan(std::string id, std::string desc, RateType t, double price)
    : planId(std::move(id)),
      description(std::move(desc)),
      type(t),
      basePrice(price) {}
void RatePlan::addApplicableRoom(Room* room) {
  applicableRooms.push_back(room);
}
double RatePlan::calculatePrice() const { return basePrice; }
