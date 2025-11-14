#include "RuleParser.h"
#include <algorithm>
#include <cctype>

Rule RuleParser::parse(const std::string& line) {
    if (line.empty()) {
        throw std::invalid_argument("Empty line cannot be parsed as rule");
    }

    size_t arrowPos = line.find("->");
    if (arrowPos == std::string::npos) {
        throw std::runtime_error("Invalid rule format, missing '->': " + line);
    }

    std::string pattern = line.substr(0, arrowPos);
    pattern = trim(pattern);

    std::string rest = line.substr(arrowPos + 2);
    rest = trim(rest);

    bool isFinal = false;
    size_t finalPos;
    if (containsFinalFlag(rest, finalPos)) {
        isFinal = true;
        rest = rest.substr(0, finalPos);
        rest = trim(rest);
    }

    std::string replacement = rest;

    processEmptyKeyword(pattern);
    processEmptyKeyword(replacement);

    return Rule(pattern, replacement, isFinal);
}

bool RuleParser::isValidFormat(const std::string& line) {
    return line.find("->") != std::string::npos;
}

bool RuleParser::tryParse(const std::string& line, Rule& outRule, std::string& errorMessage) {
    try {
        outRule = parse(line);
        return true;
    }
    catch (const std::exception& e) {
        errorMessage = e.what();
        return false;
    }
}

std::string RuleParser::trim(const std::string& str) {
    if (str.empty()) return str;

    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos) return "";

    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

bool RuleParser::containsFinalFlag(const std::string& str, size_t& finalPos) {
    finalPos = str.find("final");
    if (finalPos == std::string::npos) return false;

    if (finalPos > 0 && !std::isspace(str[finalPos - 1])) {
        return false; 
    }

    size_t afterFinal = finalPos + 5; 
    if (afterFinal < str.length() && !std::isspace(str[afterFinal])) {
        return false; 
    }

    return true;
}

void RuleParser::processEmptyKeyword(std::string& str) {
    if (str == "EMPTY") {
        str = "";
    }
}