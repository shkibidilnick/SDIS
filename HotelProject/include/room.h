#pragma once
#include <string>

#include "exceptions.h"
#include "interfaces.h"

enum class RoomType { SINGLE, DOUBLE, SUITE };

class Room : public IBookable {
 private:
  std::string number;
  RoomType type;
  double price;
  bool available;
  int capacity; //גלוסעטלמסעü

 public:
  Room(std::string num, RoomType t, double p, int cap);

  bool book() override;
  void cancel() override;

  double getPrice() const { return price; }
  std::string getNumber() const { return number; }
  bool isAvailable() const { return available; }
  int getCapacity() const { return capacity; }
  void setPrice(double p) { price = p; }
};
