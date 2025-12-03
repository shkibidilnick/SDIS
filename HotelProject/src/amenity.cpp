#include "amenity.h"

Amenity::Amenity(std::string id, std::string itemName)
    : amenityId(std::move(id)), name(std::move(itemName)) {}

void Amenity::addRoom(Room* room) {
    if (room) rooms.push_back(room);
}

void Amenity::removeRoom(Room* room) {
    rooms.erase(std::remove(rooms.begin(), rooms.end(), room), rooms.end());
}