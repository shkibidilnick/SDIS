#pragma once
#include <string>
#include <vector>

class Booking;
class Payment;
class Tax;
class Penalty;

class Invoice {  // invoice - счет
 private:
  std::string invoiceId;
  Booking* booking;
  std::vector<Tax*> appliedTaxes;
  std::vector<Penalty*> appliedPenalties;
  std::vector<Payment*> payments;
  double taxAmount;
  double penaltyAmount;
  double totalAmount;
  bool isPaid;

 public:
  Invoice(std::string id, Booking* b);
  void addPayment(Payment* payment);
  void addTax(Tax* tax);
  void addPenalty(Penalty* penalty);
  void markAsPaid();
  double getRemaining() const;
  std::string getInvoiceId() const { return invoiceId; }
  bool getPaidStatus() const { return isPaid; }
  double getTaxAmount() const { return taxAmount; }
  double getPenaltyAmount() const { return penaltyAmount; }
};
