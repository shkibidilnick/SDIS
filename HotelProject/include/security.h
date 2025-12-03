#pragma once
#include <string>

#include "employee.h"

class Security {
 private:
  std::string securityId;
  Employee* guard;
  bool onDuty;

 public:
  Security(std::string id, Employee* emp);
  void startShift();
  void endShift();
  bool isOnDuty() const { return onDuty; }
  std::string getSecurityId() const { return securityId; }
};
