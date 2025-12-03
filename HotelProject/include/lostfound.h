#pragma once
#include <string>

class Guest;

class LostFound {
 private:
  std::string itemId;
  std::string description;
  Guest* owner;
  bool returned;

 public:
  LostFound(std::string id, std::string desc, Guest* g = nullptr);

  void markReturned();

  Guest* getOwner() const { return owner; }
  std::string getItemId() const { return itemId; }
  bool isReturned() const { return returned; }
};
