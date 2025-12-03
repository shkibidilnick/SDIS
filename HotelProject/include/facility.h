#pragma once
#include <string>

#include "employee.h"
#include "service.h"

class Facility {
 private:
  std::string facilityId;
  std::string name;
  Service* mainService;
  Employee* manager;
  bool isOpen;

 public:
  Facility(std::string id, std::string facilityName, Service* s, Employee* m);
  void open();
  void close();
  bool getIsOpen() const { return isOpen; }
  std::string getFacilityId() const { return facilityId; }
};
