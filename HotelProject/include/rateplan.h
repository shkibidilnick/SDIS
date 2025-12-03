#pragma once
#include <string>
#include <vector>

#include "room.h"

// Rate plan types define cancellation and refund policies
enum class RateType {
  STANDARD,  // Standard rate: cancellation allowed with full refund (e.g., 24+
             // hours before check-in)
  FLEXIBLE,  // Flexible rate: allows late cancellation with partial refund
             // (e.g., 50% refund)
  NON_REFUNDABLE  // Non-refundable rate: no refund on cancellation, guest
                  // forfeits entire payment
};

class RatePlan {
private:
    std::string planId;
    std::string description;
    RateType type;
    double basePrice;
    std::vector<Room*> applicableRooms;
public:
    RatePlan(std::string id, std::string desc, RateType t, double price);
    void addApplicableRoom(Room* room);
    double calculatePrice() const;
    std::string getPlanId() const { return planId; }
    RateType getType() const { return type; }
 };
