#include "parking.h"

Parking::Parking(std::string id, int spot)
    : parkingId(std::move(id)),
      spotNumber(spot),
      occupied(false),
      currentGuest(nullptr) {}

void Parking::parkVehicle(Vehicle* v, Guest* g) {
  if (v && g) {
    vehicles.push_back(v);
    currentGuest = g;
    occupied = true;
  }
}

void Parking::removeVehicle() {
  vehicles.clear();
  currentGuest = nullptr;
  occupied = false;
}
