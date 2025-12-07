#include <UnitTest++/UnitTest++.h>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Core/Department.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Finance/Budget.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Finance/Invoice.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Finance/Payroll.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Finance/Transaction.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FactoryException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FinanceException.h"

// ==================== Тесты Budget ====================
TEST(Budget_Constructor_CreatesBudgetWithCorrectProperties) {
  Budget budget("D001", "2025-Q1", 100000.0);

  CHECK(!budget.getTrackingId().empty());
  CHECK_EQUAL("D001", budget.getDepartmentId());
  CHECK_EQUAL("2025-Q1", budget.getFiscalPeriod());
  CHECK_CLOSE(100000.0, budget.getTotalBudgetAmount(), 0.01);
}

TEST(Budget_Allocate_AssignsFundsToCategory) {
  Budget budget("D001", "2025-Q1", 100000.0);

  budget.allocate("MATERIALS", 40000.0);
  budget.allocate("LABOR", 35000.0);

  CHECK_CLOSE(40000.0, budget.getRemaining("MATERIALS"), 0.01);
  CHECK_CLOSE(35000.0, budget.getRemaining("LABOR"), 0.01);
  CHECK_CLOSE(25000.0, budget.getRemaining("UNALLOCATED"), 0.01);
}

TEST(Budget_Allocate_ThrowsWhenInsufficientFunds) {
  Budget budget("D001", "2025-Q1", 100000.0);

  budget.allocate("MATERIALS", 80000.0);
  CHECK_THROW(budget.allocate("LABOR", 30000.0), FinanceException);
}

TEST(Budget_RecordExpense_UpdatesActualSpending) {
  Budget budget("D001", "2025-Q1", 100000.0);

  budget.allocate("MATERIALS", 50000.0);
  budget.recordExpense("MATERIALS", 15000.0);

  CHECK_CLOSE(35000.0, budget.getRemaining("MATERIALS"), 0.01);
}

TEST(Budget_GetUtilizationPercentage_ReturnsCorrectValue) {
  Budget budget("D001", "2025-Q1", 100000.0);

  budget.allocate("MATERIALS", 50000.0);
  budget.recordExpense("MATERIALS", 25000.0);

  CHECK_CLOSE(25.0, budget.getUtilizationPercentage(), 1.0);
}

TEST(Budget_Lock_PreventsModification) {
  Budget budget("D001", "2025-Q1", 100000.0);

  budget.lock();

  CHECK_THROW(budget.allocate("LABOR", 50000.0), FinanceException);
  CHECK_EQUAL("LOCKED", budget.getStatus());
}

// ==================== Тесты Invoice ====================
TEST(Invoice_Constructor_CreatesInvoiceWithCorrectProperties) {
  Invoice invoice("CUST001", "2025-02-28", "PO12345");

  CHECK(!invoice.getTrackingId().empty());
  CHECK_EQUAL("CUST001", invoice.getCustomerId());
  CHECK_EQUAL("PO12345", invoice.getPurchaseOrderNumber());
  CHECK_CLOSE(0.0, invoice.getTotalAmount(), 0.01);
  CHECK_EQUAL(false, invoice.getIsPaid());
}

TEST(Invoice_AddLineItem_AddsItemToInvoice) {
  Invoice invoice("CUST001", "2025-02-28");

  invoice.addLineItem("Steel Sheets", 5000.0);
  invoice.addLineItem("Bolts", 250.0);

  // getLineItemCount не существует, проверяем через getTotalAmount после
  // calculateTotals
  invoice.calculateTotals(0.0);
  CHECK_CLOSE(5250.0, invoice.getTotalAmount(), 0.01);
}

TEST(Invoice_CalculateTotals_ComputesTaxAndTotal) {
  Invoice invoice("CUST001", "2025-02-28");

  invoice.addLineItem("Machine Parts", 10000.0);
  invoice.calculateTotals(8.5);  // 8.5% tax

  // getTaxAmount не существует, проверяем общую сумму
  CHECK_CLOSE(10850.0, invoice.getTotalAmount(), 0.01);  // 10000 + 850 tax
}

TEST(Invoice_MarkAsPaid_SetsPaidStatus) {
  Invoice invoice("CUST002", "2025-02-28");

  invoice.addLineItem("Services", 5000.0);
  invoice.calculateTotals(0.0);
  invoice.markAsPaid();

  CHECK_EQUAL(true, invoice.getIsPaid());
  CHECK_EQUAL("PAID", invoice.getStatus());
}

TEST(Invoice_IsOverdue_ReturnsCorrectStatus) {
  // Прошедшая дата
  Invoice invoice("CUST001", "2024-01-01");

  CHECK_EQUAL(true, invoice.isOverdue());
}

// ==================== Тесты Payroll ====================
TEST(Payroll_Constructor_CreatesPayrollWithCorrectProperties) {
  Payroll payroll("2025-01-01 to 2025-01-31");

  CHECK(!payroll.getPayrollId().empty());
  CHECK_EQUAL("2025-01-01 to 2025-01-31", payroll.getPayPeriod());
  CHECK_EQUAL(false, payroll.getIsProcessed());
}

TEST(Payroll_AddEmployeeHours_AddsEmployeeAndHours) {
  Payroll payroll("2025-01-01 to 2025-01-31");
  auto employee = std::make_shared<Employee>("Mike Hill", Role::OPERATOR, 18.0);

  payroll.addEmployeeHours(employee, 160.0);

  CHECK_CLOSE(2880.0, payroll.getEmployeePay(employee->getEmployeeId()), 0.01);
}

TEST(Payroll_CalculateTotalPayroll_ReturnsCorrectSum) {
  Payroll payroll("2025-01-01 to 2025-01-31");
  auto emp1 = std::make_shared<Employee>("Nancy Irish", Role::OPERATOR, 18.0);
  auto emp2 = std::make_shared<Employee>("Oliver James", Role::ENGINEER, 28.0);

  payroll.addEmployeeHours(emp1, 160.0);
  payroll.addEmployeeHours(emp2, 160.0);

  CHECK_CLOSE(7360.0, payroll.calculateTotalPayroll(), 0.01);  // (18+28)*160
}

TEST(Payroll_Process_MarksAsProcessed) {
  Payroll payroll("2025-01-01 to 2025-01-31");
  auto employee =
      std::make_shared<Employee>("Paula Jones", Role::MANAGER, 35.0);

  payroll.addEmployeeHours(employee, 160.0);
  payroll.process();

  CHECK_EQUAL(true, payroll.getIsProcessed());
}

TEST(Payroll_AddEmployeeHours_ThrowsWhenProcessed) {
  Payroll payroll("2025-01-01 to 2025-01-31");
  auto employee =
      std::make_shared<Employee>("Quinn King", Role::OPERATOR, 18.0);

  payroll.addEmployeeHours(employee, 160.0);
  payroll.process();

  CHECK_THROW(payroll.addEmployeeHours(employee, 40.0), PayrollException);
}

// ==================== Тесты Transaction ====================
TEST(Transaction_Constructor_CreatesTransactionWithCorrectProperties) {
  Transaction txn("PAYMENT", 5000.0, "ACC001", "ACC002", "INV001");

  CHECK(!txn.getTrackingId().empty());
  CHECK_EQUAL("PAYMENT", txn.getType());
  CHECK_CLOSE(5000.0, txn.getAmount(), 0.01);
  CHECK_EQUAL(false, txn.getIsPosted());
}

TEST(Transaction_Post_MarksAsPosted) {
  Transaction txn("RECEIPT", 10000.0, "CUST001", "ACC001", "INV001");

  txn.post();

  CHECK_EQUAL(true, txn.getIsPosted());
  CHECK_EQUAL("POSTED", txn.getStatus());
}

TEST(Transaction_Post_ThrowsWhenAlreadyPosted) {
  Transaction txn("TRANSFER", 2500.0, "ACC001", "ACC002", "REF001");

  txn.post();
  CHECK_THROW(txn.post(), FinanceException);
}

TEST(Transaction_Reverse_CreatesReversalTransaction) {
  Transaction txn("PAYMENT", 3000.0, "ACC001", "ACC002", "INV001");

  txn.post();
  auto reversal = txn.reverse();

  CHECK_EQUAL("REVERSAL", reversal->getType());
  CHECK_CLOSE(3000.0, reversal->getAmount(), 0.01);
  CHECK_EQUAL(true, reversal->getIsPosted());
}
