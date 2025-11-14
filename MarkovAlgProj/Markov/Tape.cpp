#include "Tape.h"

MarkovTape::MarkovTape(const std::string& initial) : content_(initial) {}

void MarkovTape::setContent(const std::string& content) { content_ = content; }

std::string MarkovTape::getContent() const { return content_; }

bool MarkovTape::operator==(const MarkovTape& other) const {
  return content_ == other.content_;
}

bool MarkovTape::operator!=(const MarkovTape& other) const {
  return !(*this == other);
}
