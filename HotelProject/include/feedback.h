#pragma once
#include <string>

class Guest;
class Employee;

class Feedback {
 private:
  std::string feedbackId;
  Guest* guest;
  Employee* assignedEmployee;  // assigned - назначенный
  int rating;
  std::string comment;

 public:
  Feedback(std::string id, Guest* g, int stars);
  void assignToEmployee(Employee* emp);
  std::string getFeedbackId() const { return feedbackId; }
  int getRating() const { return rating; }
};
