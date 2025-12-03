#pragma once
#include <string>

#include "guest.h"

class Review {
 private:
  std::string reviewId;
  Guest* guest;
  std::string roomNumber;
  int rating;
  std::string comment;

 public:
  Review(std::string id, Guest* g, std::string room, int stars);
  void updateRating(int stars);
  void updateComment(const std::string& text);
  std::string getId() const { return reviewId; }
  int getRating() const { return rating; }
};
