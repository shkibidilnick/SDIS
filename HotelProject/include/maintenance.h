#pragma once
#include <string>

#include "employee.h"
#include "room.h"

class Maintenance {
 private:
  std::string requestId;
  Room* room;
  Employee* technician;
  std::string issue;

 public:
  Maintenance(std::string id, Room* r, Employee* tech, std::string problem);
  void assignTechnician(Employee* tech);
  void resolveIssue();
  std::string getRequestId() const { return requestId; }
};
