#include "housekeeping.h"

Housekeeping::Housekeeping(std::string id, Room* r, Employee* e)
    : taskId(std::move(id)), room(r), cleaner(e) {}
void Housekeeping::assignTask() {
  schedule = "Assigned";  // assigned - назначено
}

void Housekeeping::completeTask() { schedule = "Completed"; }
