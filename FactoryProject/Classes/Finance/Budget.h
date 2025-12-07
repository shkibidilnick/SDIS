#pragma once
#include <string>
#include <unordered_map>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FinanceException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"

// Manages departmental budgets and tracks expenditures
// Provides real-time budget vs actual analysis
class Budget : public ITrackable {
 private:
  std::string budgetId;
  std::string departmentId;
  std::string fiscalPeriod;                             // e.g., "2025-Q1"
  std::unordered_map<std::string, double> allocations;  // Category -> Amount
  std::unordered_map<std::string, double> actuals;      // Category -> Spent
  bool isLocked;  // Prevents changes after approval

 public:
  Budget(const std::string& deptId, const std::string& period,
         double totalAmount);

  // Allocates budget to a category (e.g., "Maintenance", "Materials")
  void allocate(const std::string& category, double amount);

  // Records actual spending against a category
  void recordExpense(const std::string& category, double amount);

  // Gets remaining budget for a category
  double getRemaining(const std::string& category) const;

  // Gets overall budget utilization percentage
  double getUtilizationPercentage() const;
  
  // Gets total budget amount (sum of all allocations)
  double getTotalBudgetAmount() const;

  // Locks budget after approval
  void lock() { isLocked = true; }

  // ITrackable implementation
  std::string getTrackingId() const override { return budgetId; }
  std::string getStatus() const override {
    return isLocked ? "LOCKED" : "DRAFT";
  }

  std::string getDepartmentId() const { return departmentId; }
  std::string getFiscalPeriod() const { return fiscalPeriod; }
};
