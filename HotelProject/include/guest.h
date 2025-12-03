#pragma once
#include <string>
#include <vector>

class Guest {
 private:
  std::string id;
  std::string name;
  std::string email;
  std::string phone;
  int loyaltyPoints;
  bool isVIP;
  std::string currentRoom;

 public:
  Guest(std::string guestId, std::string guestName);
  void addPoints(int points);
  void makeVIP();
  void updateContact(std::string newEmail, std::string newPhone);
  void checkIn(const std::string& roomNumber);
  void checkOut();
  std::string getId() const { return id; }
  std::string getName() const { return name; }
  int getPoints() const { return loyaltyPoints; }
  bool getVIPStatus() const { return isVIP; }
  std::string getCurrentRoom() const { return currentRoom; }
};
