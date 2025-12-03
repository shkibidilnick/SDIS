#pragma once
#include <string>

#include "exceptions.h"
#include "interfaces.h"

enum class PaymentMethod { CASH, CARD, TRANSFER };
class Payment : public IPayable {
 private:
  std::string id;
  std::string bookingId;
  double amount;
  PaymentMethod method;
  bool completed;

 public:
  Payment(std::string paymentId, std::string bid, double amt, PaymentMethod m);
  double getAmount() const { return amount; }
  void processPayment() override;
  void refund();
  bool isCompleted() const { return completed; }
  std::string getId() const { return id; }
  std::string getBookingId() const { return bookingId; }
};
