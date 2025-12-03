#pragma once
#include <string>

#include "booking.h"
#include "payment.h"
class Deposit {
 private:
  std::string depositId;
  Booking* booking;
  Payment* payment;
  double amount;
  bool isRefundable;

 public:
  Deposit(std::string id, Booking* b, Payment* p, double amt);
  void hold();
  void release();
  double getAmount() const { return amount; }
  std::string getDepositId() const { return depositId; }
};
