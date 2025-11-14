#ifndef RULE_H
#define RULE_H

#include <iostream>
#include <string>

class Rule {
 public:
  Rule(const std::string& pattern = "", const std::string& replacement = "",
       bool isFinal = false);
  Rule(const Rule& other);
  Rule& operator=(const Rule& other);
  bool operator==(const Rule& other) const;
  bool operator!=(const Rule& other) const;

  std::string getPattern() const;
  std::string getReplacement() const;
  bool isFinal() const;

 private:
  std::string pattern_;
  std::string replacement_;
  bool isFinal_;
};

#endif
