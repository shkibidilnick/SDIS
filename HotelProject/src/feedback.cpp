#include "feedback.h"

Feedback::Feedback(std::string id, Guest* g, int stars)
    : feedbackId(std::move(id)),
      guest(g),
      assignedEmployee(nullptr),
      rating(stars) {}
void Feedback::assignToEmployee(Employee* emp) { assignedEmployee = emp; }
