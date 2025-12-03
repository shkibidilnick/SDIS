#pragma once
#include <string>

#include "guest.h"
#include "vehicle.h"


enum class TransportType { AIRPORT_SHUTTLE, TAXI, RENTAL_CAR };

class Transportation {
 private:
  std::string transportId;
  Guest* guest;
  TransportType type;
  std::string schedule;
  double cost;
  bool isConfirmed;

 public:
  Transportation(std::string id, Guest* g, TransportType t, double c);
  void confirm();
  void cancel();
  double getCost() const { return cost; }
  std::string getTransportId() const { return transportId; }
  bool getIsConfirmed() const { return isConfirmed; }
};
