#pragma once
#include <fstream>
#include <vector>

#include "utils.h"

class Booking;
class Payment;

enum class ReportType { OCCUPANCY, REVENUE, GUEST, SERVICE };

class Report {
 private:
  std::string reportId;
  ReportType type;
  std::string generatedDate;
  std::vector<Booking*> bookings;
  std::vector<Payment*> payments;

 public:
  Report(std::string id, ReportType t);
  void addBooking(Booking* b);
  void addPayment(Payment* p);
  void generate();
  std::string getReportId() const { return reportId; }
  ReportType getType() const { return type; }
};
