#ifndef MARKOVALGORITHM_H
#define MARKOVALGORITHM_H

#include <iostream>
#include <vector>

#include "Rule.h"
#include "Tape.h"

class MarkovAlgorithm {
 public:
  MarkovAlgorithm();
  MarkovAlgorithm& operator=(
      const MarkovAlgorithm& other); 

  void addRule(const Rule& rule);
  void loadRulesFromStream(std::istream& stream);
  void setTape(const MarkovTape& tape);
  void step();
  void run();

  std::string getCurrentTape() const;
  bool isHalted() const;
  void reset();

 private:
  std::vector<Rule> rules_;
  MarkovTape tape_;
  bool halted_ = false;
  const int MAX_ITERATIONS = 10000; 
  int iterationCount_ = 0;
};

#endif
