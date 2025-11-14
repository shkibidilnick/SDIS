#ifndef RULEPARSER_H
#define RULEPARSER_H

#include <string>
#include <stdexcept>
#include "Rule.h"

class RuleParser {
public:
    static Rule parse(const std::string& line);
    static bool isValidFormat(const std::string& line);

    static bool tryParse(const std::string& line, Rule& outRule, std::string& errorMessage);

private:
    static std::string trim(const std::string& str);
    static bool containsFinalFlag(const std::string& str, size_t& finalPos);
    static void processEmptyKeyword(std::string& str);
};

#endif
