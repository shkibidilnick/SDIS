#include "maintenance.h"

Maintenance::Maintenance(std::string id, Room* r, Employee* tech,
                         std::string problem)
    : requestId(std::move(id)),
      room(r),
      technician(tech),
      issue(std::move(problem)) {}
void Maintenance::assignTechnician(Employee* tech) { technician = tech; }
void Maintenance::resolveIssue() { issue.clear(); }
