#include <UnitTest++/UnitTest++.h>

#include "employee.h"
#include "housekeeping.h"
#include "room.h"
#include "test_helpers.h"

TEST(Housekeeping_AssignCompleteTask_Works) {
  Room room("101", RoomType::SINGLE, 100.0, 1);
  Employee cleaner("E001", "Cleaner", Role::CLEANER);
  Housekeeping task("HK001", &room, &cleaner);

  task.assignTask();
  task.completeTask();

  CHECK_EQUAL("HK001", task.getTaskId());
}
