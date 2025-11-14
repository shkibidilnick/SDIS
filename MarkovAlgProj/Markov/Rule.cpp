#include "Rule.h"

Rule::Rule(const std::string& pattern, const std::string& replacement,
           bool isFinal)
    : pattern_(pattern), replacement_(replacement), isFinal_(isFinal) {}

Rule::Rule(const Rule& other)
    : pattern_(other.pattern_),
      replacement_(other.replacement_),
      isFinal_(other.isFinal_) {}

Rule& Rule::operator=(const Rule& other) {
  if (this != &other) {
    pattern_ = other.pattern_;
    replacement_ = other.replacement_;
    isFinal_ = other.isFinal_;
  }
  return *this;
}

bool Rule::operator==(const Rule& other) const {
  return pattern_ == other.pattern_ && replacement_ == other.replacement_ &&
         isFinal_ == other.isFinal_;
}

bool Rule::operator!=(const Rule& other) const { return !(*this == other); }
std::string Rule::getPattern() const { return pattern_; }
std::string Rule::getReplacement() const { return replacement_; }
bool Rule::isFinal() const { return isFinal_; }

