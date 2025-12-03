#include "event.h"

#include "exceptions.h"
Event::Event(std::string id, std::string eventName, int cap, double price)
    : eventId(std::move(id)),
      name(std::move(eventName)),
      capacity(cap),
      ticketPrice(price) {}
void Event::registerGuest(Guest* guest) {
  if (isFull()) throw GuestException("Event is full");
  attendees.push_back(guest);
}
void Event::cancelGuest(Guest* guest) {
  attendees.erase(std::remove(attendees.begin(), attendees.end(), guest),
                  attendees.end());
}
bool Event::isFull() const { return attendees.size() >= capacity; }
