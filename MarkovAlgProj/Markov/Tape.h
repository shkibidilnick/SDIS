#ifndef TAPE_H
#define TAPE_H

#include <iostream>
#include <string>

class MarkovTape {
 public:
  MarkovTape(const std::string& initial = "");

  void setContent(const std::string& content);
  std::string getContent() const;
  bool operator==(const MarkovTape& other) const;
  bool operator!=(const MarkovTape& other) const;

 private:
  std::string content_;
};

#endif
