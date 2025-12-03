#pragma once
#include <string>
#include <vector>

class Vehicle;
class Guest;

class Parking {
 private:
  std::string parkingId;
  std::vector<Vehicle*> vehicles;
  Guest* currentGuest;
  int spotNumber;
  bool occupied;

 public:
  Parking(std::string id, int spot);

  void parkVehicle(Vehicle* v, Guest* g);
  void removeVehicle();

  bool isOccupied() const { return occupied; }
  std::string getParkingId() const { return parkingId; }
};
