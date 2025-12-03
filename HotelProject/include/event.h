#pragma once
#include <string>
#include <vector>

class Guest;
class Event {
 private:
  std::string eventId;
  std::string name;
  std::vector<Guest*> attendees;
  int capacity;  // capacity - גלוסעטלמסעü
  double ticketPrice;

 public:
  Event(std::string id, std::string eventName, int cap, double price);
  void registerGuest(Guest* guest);
  void cancelGuest(Guest* guest);
  bool isFull() const;
  std::string getEventId() const { return eventId; }
};
