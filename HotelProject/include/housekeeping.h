#pragma once
#include <string>

#include "employee.h"
#include "room.h"
class Housekeeping {
 private:
  std::string taskId;
  Room* room;
  Employee* cleaner;
  std::string schedule;

 public:
  Housekeeping(std::string id, Room* r, Employee* e);
  void assignTask();
  void completeTask();
  std::string getTaskId() const { return taskId; }
};
