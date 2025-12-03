#pragma once
#include <chrono>
#include <string>

inline std::string getCurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  time_t t = std::chrono::system_clock::to_time_t(now);
  return std::to_string(t);
}
