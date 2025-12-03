#include <UnitTest++/UnitTest++.h>

#include "employee.h"
#include "maintenance.h"
#include "room.h"
#include "test_helpers.h"

TEST(Maintenance_AssignResolve_Works) {
  Room room("101", RoomType::SINGLE, 100.0, 1);
  Employee tech1("E001", "Tech1", Role::MANAGER);
  Employee tech2("E002", "Tech2", Role::MANAGER);

  Maintenance request("M001", &room, &tech1, "Leak");

  request.assignTechnician(&tech2);
  request.resolveIssue();

  CHECK_EQUAL("M001", request.getRequestId());
}
