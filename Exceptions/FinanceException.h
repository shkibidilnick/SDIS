#pragma once
#include "FactoryException.h"

// Thrown when financial operations fail
// Examples: payment failure, budget overrun, payroll issues (проблемы расчета
// зарплаты)
class FinanceException : public FactoryException {
 public:
  explicit FinanceException(const std::string& message)
      : FactoryException("Finance: " + message) {}
};
