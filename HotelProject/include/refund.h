#pragma once
#include <string>

#include "exceptions.h"
#include "payment.h"

class Booking;

class Refund {
 private:
  std::string refundId;
  Payment* originalPayment;
  Booking* booking;
  double amount;
  std::string reason;

 public:
  Refund(std::string id, Payment* p, Booking* b, double amt, std::string r);
  void process();
  double getAmount() const { return amount; }
  std::string getRefundId() const { return refundId; }
};
