#pragma once
#include <string>

class Vehicle {
 private:
  std::string plateNumber;
  std::string model;
  std::string color;

 public:
  Vehicle(std::string plate, std::string m, std::string c);
  std::string getPlate() const { return plateNumber; }
  std::string getModel() const { return model; }
};
