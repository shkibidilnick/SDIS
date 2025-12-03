#include "vehicle.h"

Vehicle::Vehicle(std::string plate, std::string m, std::string c)
    : plateNumber(std::move(plate)), model(std::move(m)), color(std::move(c)) {}
