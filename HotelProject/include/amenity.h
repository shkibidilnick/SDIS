#pragma once
#include <string>
#include <vector>

class Room;

class Amenity {  // amenity - удобство в виде предмета(напр. наличие wifi)
 private:
  std::string amenityId;
  std::string name;
  std::vector<Room*> rooms;

 public:
  Amenity(std::string id, std::string itemName);

  void addRoom(Room* room);
  void removeRoom(Room* room);

  std::string getAmenityId() const { return amenityId; }
  std::string getName() const { return name; }
};
