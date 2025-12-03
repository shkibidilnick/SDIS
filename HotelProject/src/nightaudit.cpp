#include "nightaudit.h"

NightAudit::NightAudit(std::string id)
    : auditId(std::move(id)), totalRevenue(0.0) {
  date = getCurrentTimestamp();
}

void NightAudit::run(Report* revenueReport,
                     const std::vector<Payment*>& payments) {
  totalRevenue = 0.0;
  for (Payment* p : payments) {
    if (p && p->isCompleted()) {
      totalRevenue += p->getAmount();
    }
  }
  if (revenueReport) {
    revenueReport->generate();
  }

  std::ofstream file("audit_" + auditId + "_" + date + ".txt");
  if (file.is_open()) {
    file << "Night Audit #" << auditId << "\n";
    file << "Date: " << date << "\n";
    file << "Revenue: $" << totalRevenue << "\n";
    file << "Reports processed: " << (revenueReport ? "1" : "0") << "\n";
    file.close();
  }
}
