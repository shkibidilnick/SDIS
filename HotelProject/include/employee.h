#pragma once
#include <string>

#include "exceptions.h"

enum class Role { MANAGER, RECEPTIONIST, CLEANER };

class Employee {
 private:
  std::string id;
  std::string name;
  Role role;
  double salary;
  bool active;

 public:
  Employee(std::string empId, std::string empName, Role r);
  void setSalary(double s);
  void setActive(bool a);
  double calculateBonus() const;
  std::string getId() const { return id; }
  Role getRole() const { return role; }
  bool isActive() const { return active; }
};
