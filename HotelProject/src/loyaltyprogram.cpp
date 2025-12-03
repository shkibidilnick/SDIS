#include "loyaltyprogram.h"

LoyaltyProgram::LoyaltyProgram(std::string id)
    : programId(std::move(id)), discountRate(0.1) {}
void LoyaltyProgram::addMember(Guest* guest) {
  members.push_back(guest);
  guest->makeVIP();
}

void LoyaltyProgram::removeMember(Guest* guest) {
  members.erase(std::remove(members.begin(), members.end(), guest),
                members.end());
}
double LoyaltyProgram::applyDiscount(double amount) const {
  return amount * (1.0 - discountRate);
}
