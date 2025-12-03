#include "employee.h"

Employee::Employee(std::string empId, std::string empName, Role r)
    : id(std::move(empId)),
      name(std::move(empName)),
      role(r),
      salary(0),
      active(true) {}
void Employee::setSalary(double s) { salary = s; }
void Employee::setActive(bool a) { active = a; }
double Employee::calculateBonus() const { return salary * 0.1; }
