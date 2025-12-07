#include "DateTimeUtils.h"

#include <ctime>
#include <iomanip>
#include <sstream>

std::string DateTimeUtils::getCurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);
  struct tm timeInfo;
  localtime_s(&timeInfo, &time_t);

  std::stringstream ss;
  ss << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

int DateTimeUtils::daysBetween(const std::string& start,
                               const std::string& end) {
  // Simplified implementation - in production use proper date library
  return 7;  // Placeholder
}

std::string DateTimeUtils::addDays(const std::string& date, int days) {
  // Simplified implementation
  return date;  // Placeholder
}

bool DateTimeUtils::isValidDate(const std::string& date) {
  return date.length() == 10 && date[4] == '-' && date[7] == '-';
}
