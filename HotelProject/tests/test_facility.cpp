#include <UnitTest++/UnitTest++.h>

#include "employee.h"
#include "facility.h"
#include "service.h"
#include "test_helpers.h"

TEST(Facility_OpenClose_Works) {
  Service service("S001", "Spa", ServiceType::SPA, 50.0);
  Employee manager("E001", "Manager", Role::MANAGER);
  Facility facility("F001", "Spa Center", &service, &manager);

  CHECK(!facility.getIsOpen());

  facility.open();
  CHECK(facility.getIsOpen());

  facility.close();
  CHECK(!facility.getIsOpen());

  CHECK_EQUAL("F001", facility.getFacilityId());
}
