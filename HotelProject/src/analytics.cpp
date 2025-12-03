#include "analytics.h"

Analytics::Analytics(std::string id) : analyticsId(std::move(id)) {}

void Analytics::addReport(Report* r) {
  if (r) reports.push_back(r);
}

void Analytics::addBooking(Booking* b) {
  if (b) bookings.push_back(b);
}

void Analytics::generateAllReports() {
  for (auto* report : reports) {
    if (report) report->generate();
  }
}
