#include "guest.h"

Guest::Guest(std::string guestId, std::string guestName)
    : id(std::move(guestId)),
      name(std::move(guestName)),
      loyaltyPoints(0),
      isVIP(false) {}

void Guest::addPoints(int points) { loyaltyPoints += points; }

void Guest::makeVIP() { isVIP = loyaltyPoints > 1000; }

void Guest::updateContact(std::string newEmail, std::string newPhone) {
  email = std::move(newEmail);
  phone = std::move(newPhone);
}
void Guest::checkIn(const std::string& roomNumber) {
    currentRoom = roomNumber;
}

void Guest::checkOut() {
    currentRoom.clear();
}