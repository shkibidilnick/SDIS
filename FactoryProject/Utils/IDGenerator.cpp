#include "IDGenerator.h"
#include <sstream>
#include <iomanip>

std::atomic<int> IDGenerator::nextId(1);

std::string IDGenerator::generate(const std::string& prefix) {
    int id = nextId.fetch_add(1);
    std::stringstream ss;
    ss << prefix << "_" << std::setw(6) << std::setfill('0') << id; // setw = set width
    return ss.str();
}