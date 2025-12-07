#pragma once
#include <chrono>
#include <string>

// Utility class for date/time operations across the factory system
// Provides consistent timestamp formatting and date calculations
class DateTimeUtils {
 public:
  // Returns current timestamp in human-readable format: YYYY-MM-DD HH:MM:SS
  static std::string getCurrentTimestamp();

  // Calculates days between two dates (date format: YYYY-MM-DD)
  static int daysBetween(const std::string& startDate,
                         const std::string& endDate);

  // Adds days to a given date
  static std::string addDays(const std::string& date, int days);

  // Checks if a date string is valid format
  static bool isValidDate(const std::string& date);
};
