#pragma once
#include <fstream>
#include <vector>

#include "payment.h"
#include "report.h"
#include "utils.h"

class NightAudit {
 private:
  std::string auditId;
  std::string date;
  double totalRevenue;  // revenue - доход
 public:
  explicit NightAudit(std::string id);
  void run(Report* revenueReport, const std::vector<Payment*>& payments);
  double getRevenue() const { return totalRevenue; }
  std::string getAuditId() const { return auditId; }
  std::string getDate() const { return date; }
};
