#include "review.h"

Review::Review(std::string id, Guest* g, std::string room, int stars)
    : reviewId(std::move(room)),
      guest(g),
      roomNumber(std::move(room)),
      rating(stars) {}
void Review::updateRating(int stars) { rating = stars; }
void Review::updateComment(const std::string& text) { comment = text; }
