#pragma once
#include <string>
#include <vector>

#include "report.h"

class Booking;

class Analytics {
 private:
  std::string analyticsId;
  std::vector<Report*> reports;
  std::vector<Booking*> bookings;

 public:
  explicit Analytics(std::string id);

  void addReport(Report* r);
  void addBooking(Booking* b);
  void generateAllReports();

  std::string getAnalyticsId() const { return analyticsId; }
};
