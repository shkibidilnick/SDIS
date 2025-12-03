#include "room.h"

Room::Room(std::string num, RoomType t, double p, int cap)
    : number(std::move(num)),
      type(t),
      price(p),
      available(true),
      capacity(cap) {}
bool Room::book() {
  if (!available) return false;
  available = false;
  return true;
}

void Room::cancel() { available = true; }
