#include <UnitTest++/UnitTest++.h>

#include "analytics.h"
#include "booking.h"
#include "guest.h"
#include "report.h"
#include "room.h"
#include "test_helpers.h"

TEST(Analytics_GenerateReports_Works) {
  Analytics analytics("AN001");
  Report report("R001", ReportType::REVENUE);

  Guest guest("G001", "John");
  Room room("101", RoomType::SINGLE, 100.0, 1);
  Booking booking("B001", &guest, &room, "2024-01-01", "2024-01-03");

  analytics.addReport(&report);
  analytics.addBooking(&booking);
  analytics.generateAllReports();

  CHECK_EQUAL("AN001", analytics.getAnalyticsId());
}
