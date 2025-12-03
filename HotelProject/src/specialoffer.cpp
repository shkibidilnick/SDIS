#include "specialoffer.h"

SpecialOffer::SpecialOffer(std::string id, std::string desc, double disc)
    : offerId(std::move(id)),
      description(std::move(desc)),
      discount(disc),
      isActive(false) {}
void SpecialOffer::addTargetGuest(Guest* guest) {
  targetGuests.push_back(guest);
}
void SpecialOffer::addTargetRoom(Room* room) { targetRooms.push_back(room); }
void SpecialOffer::activate() { isActive = true; }
void SpecialOffer::deactivate() { isActive = false; }
