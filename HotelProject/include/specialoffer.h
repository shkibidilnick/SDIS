#pragma once
#include <string>
#include <vector>

#include "guest.h"
#include "room.h"

class SpecialOffer {
 private:
  std::string offerId;
  std::string description;
  std::vector<Guest*> targetGuests;
  std::vector<Room*> targetRooms;
  double discount;
  bool isActive;

 public:
  SpecialOffer(std::string id, std::string desc, double disc);
  void addTargetGuest(Guest* guest);
  void addTargetRoom(Room* room);
  void activate();
  void deactivate();
  double getDiscount() const { return discount; }
  std::string getOfferId() const { return offerId; }
};
