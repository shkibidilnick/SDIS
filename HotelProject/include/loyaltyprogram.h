#pragma once
#include <vector>

#include "guest.h"

class LoyaltyProgram {
 private:
  std::string programId;
  std::vector<Guest*> members;
  double discountRate;

 public:
  explicit LoyaltyProgram(std::string id);
  void addMember(Guest* guest);
  void removeMember(Guest* guest);
  double applyDiscount(double amount) const;
  std::string getProgramId() const { return programId; }
};
