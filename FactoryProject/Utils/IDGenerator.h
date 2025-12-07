#pragma once
#include <atomic>
#include <string>

// Generates unique IDs for all entities in the factory system
// Ensures no ID collisions across different entity types
class IDGenerator {
 private:
  static std::atomic<int> nextId; // atomic - неделимый
  IDGenerator() = delete;  // Static utility class

 public:
  // Generates a unique ID with a given prefix
  // Example: generate("EMP") returns "EMP_000001"
  static std::string generate(const std::string& prefix);
};
