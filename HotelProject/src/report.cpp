#include "report.h"

Report::Report(std::string id, ReportType t)
    : reportId(std::move(id)), type(t) {
  generatedDate = getCurrentTimestamp();
}

void Report::addBooking(Booking* b) {
  if (b) bookings.push_back(b);
}

void Report::addPayment(Payment* p) {
  if (p) payments.push_back(p);
}

void Report::generate() {
  std::ofstream file("report_" + reportId + ".txt");
  if (file.is_open()) {
    file << "Report ID: " << reportId << "\n";
    file << "Type: "
         << (type == ReportType::OCCUPANCY ? "Occupancy"
             : type == ReportType::REVENUE ? "Revenue"
             : type == ReportType::GUEST   ? "Guest"
                                           : "Service")
         << "\n";
    file << "Date: " << generatedDate << "\n";
    file << "Bookings: " << bookings.size() << "\n";
    file << "Payments: " << payments.size() << "\n";
    file.close();
  }
}
