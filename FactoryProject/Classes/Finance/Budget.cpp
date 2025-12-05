#include "Budget.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

Budget::Budget(const std::string& deptId, const std::string& period,
               double totalAmount)
    : departmentId(deptId), fiscalPeriod(period), isLocked(false) {
  budgetId = IDGenerator::generate("BUD");

  Validator::validatePositive(totalAmount, "Total budget amount");

  allocations["UNALLOCATED"] = totalAmount;  // Track unallocated funds

  Logger::getInstance().log(
      "Budget", "Created budget " + budgetId + " for dept: " + deptId);
}

void Budget::allocate(const std::string& category, double amount) {
  if (isLocked) {
    throw FinanceException("Cannot modify locked budget: " + budgetId);
  }

  Validator::validatePositive(amount, "Allocation amount");

  // Check if enough unallocated funds
  double unallocated = allocations["UNALLOCATED"];
  if (amount > unallocated) {
    throw FinanceException("Insufficient unallocated budget. Requested: " +
                           std::to_string(amount) +
                           ", Available: " + std::to_string(unallocated));
  }

  allocations[category] = amount;
  allocations["UNALLOCATED"] -= amount;
  actuals[category] = 0.0;  // Initialize actuals

  Logger::getInstance().log(
      "Budget", "Allocated " + std::to_string(amount) + " to " + category);
}

void Budget::recordExpense(const std::string& category, double amount) {
  if (actuals.find(category) == actuals.end()) {
    throw FinanceException("Category " + category +
                           " not found in budget allocations");
  }

  Validator::validatePositive(amount, "Expense amount");

  actuals[category] += amount;

  // Check if over budget
  if (actuals[category] > allocations[category]) {
    Logger::getInstance().log(
        "Budget", "WARNING: Category " + category + " is over budget");
  }

  Logger::getInstance().log(
      "Budget",
      "Recorded expense " + std::to_string(amount) + " for " + category);
}

double Budget::getRemaining(const std::string& category) const {
  auto allocIt = allocations.find(category);
  auto actualIt = actuals.find(category);

  if (allocIt == allocations.end() || actualIt == actuals.end()) {
    throw FinanceException("Category " + category + " not found");
  }

  return allocIt->second - actualIt->second;
}

double Budget::getUtilizationPercentage() const {
  double totalAllocated = 0.0;
  double totalActual = 0.0;

  for (const auto& pair : allocations) {
    if (pair.first != "UNALLOCATED") {
      totalAllocated += pair.second;
    }
  }

  for (const auto& pair : actuals) {
    totalActual += pair.second;
  }

  return (totalAllocated > 0) ? (totalActual / totalAllocated * 100.0) : 0.0;
}

double Budget::getTotalBudgetAmount() const {
  double total = 0.0;
  for (const auto& pair : allocations) {
    total += pair.second;
  }
  return total;
}
