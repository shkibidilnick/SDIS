#include <UnitTest++/UnitTest++.h>

#include "employee.h"
#include "test_helpers.h"

TEST(Employee_Constructor_CreatesEmployeeWithCorrectProperties) {
  Employee employee("E001", "John Manager", Role::MANAGER);

  CHECK_EQUAL("E001", employee.getId());
  CHECK_EQUAL(Role::MANAGER, employee.getRole());
  CHECK_EQUAL(true, employee.isActive());
}

TEST(Employee_SetSalary_UpdatesSalaryAndBonus) {
  Employee employee("E002", "Jane Receptionist", Role::RECEPTIONIST);

  employee.setSalary(50000.0);
  CHECK_CLOSE(5000.0, employee.calculateBonus(), 0.01);
}

TEST(Employee_SetActive_ChangesActivityStatus) {
  Employee employee("E003", "Bob Cleaner", Role::CLEANER);

  employee.setActive(false);
  CHECK_EQUAL(false, employee.isActive());

  employee.setActive(true);
  CHECK_EQUAL(true, employee.isActive());
}
