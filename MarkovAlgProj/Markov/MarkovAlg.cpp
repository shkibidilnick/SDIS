#include "MarkovAlg.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "RuleParser.h"

MarkovAlgorithm::MarkovAlgorithm() : halted_(true), iterationCount_(0) {}

MarkovAlgorithm& MarkovAlgorithm::operator=(const MarkovAlgorithm& other) {
  if (this != &other) {
    rules_ = other.rules_;
    tape_ = other.tape_;
    halted_ = other.halted_;
    iterationCount_ = other.iterationCount_;
  }
  return *this;
}

void MarkovAlgorithm::addRule(const Rule& rule) { rules_.push_back(rule); }

void MarkovAlgorithm::setTape(const MarkovTape& tape) {
  tape_ = tape;
  halted_ = false;
  iterationCount_ = 0;
}

std::string MarkovAlgorithm::getCurrentTape() const {
  return tape_.getContent();
}

bool MarkovAlgorithm::isHalted() const { return halted_; }

void MarkovAlgorithm::loadRulesFromStream(std::istream& stream) {
  rules_.clear();
  std::string line;

  try {
    while (std::getline(stream, line)) {
      if (line.empty()) continue;

      Rule rule = RuleParser::parse(line);
      rules_.push_back(rule);
    }

    halted_ = false;
    iterationCount_ = 0;
  } catch (const std::exception&) {
    rules_.clear();
    halted_ = true;
    throw;
  }
}

void MarkovAlgorithm::step() {
  if (halted_) return;

  if (iterationCount_ >= MAX_ITERATIONS) {
    halted_ = true;
    return;
  }

  std::string currentContent = tape_.getContent();

  for (size_t i = 0; i < rules_.size(); ++i) {
    const Rule& rule = rules_[i];
    std::string pattern = rule.getPattern();
    size_t pos = std::string::npos;

    if (pattern.empty()) {
      pos = 0;
    } else {
      pos = currentContent.find(pattern);
    }

    if (pos != std::string::npos) {
      std::string newContent = currentContent;
      if (pattern.empty()) {
        newContent = rule.getReplacement() + currentContent;
      } else {
        newContent.replace(pos, pattern.length(), rule.getReplacement());
      }
      tape_.setContent(newContent);

      iterationCount_++;

      if (rule.isFinal()) {
        halted_ = true;
      }
      return;
    }
  }
  halted_ = true;
}

void MarkovAlgorithm::run() {
  iterationCount_ = 0;
  while (!halted_ && iterationCount_ < MAX_ITERATIONS) {
    step();
  }
  if (iterationCount_ >= MAX_ITERATIONS) {
    halted_ = true;
  }
}

void MarkovAlgorithm::reset() {
  halted_ = false;
  iterationCount_ = 0;
}
