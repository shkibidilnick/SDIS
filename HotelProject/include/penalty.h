#pragma once  // penalty - רענאפ
#include <fstream>
#include <string>

#include "exceptions.h"

class Booking;
class Invoice;
class Payment;

enum class PenaltyType { CANCELLATION, DAMAGE, SMOKING };
class Penalty {
 private:
  std::string penaltyId;
  Booking* booking;
  Invoice* invoice;
  Payment* payment;
  PenaltyType type;
  double rate;
  double fixedAmount;
  double calculatedAmount;

 public:
  Penalty(std::string id, Booking* b, Invoice* inv, Payment* p, PenaltyType t,
          double r = 0.0, double fixed = 0.0);
  void apply();
  double calculate(double baseAmount) const;
  std::string getDescription() const;
  double getCalculatedAmount() const { return calculatedAmount; }
  std::string getPenaltyId() const { return penaltyId; }
  PenaltyType getType() const { return type; }
};
