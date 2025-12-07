#pragma once
#include <chrono>
#include <fstream>
#include <string>

// Thread-safe logging utility for all system events
// Logs to file with timestamps for audit purposes
class Logger {
 private:
  static Logger* instance;  // instance - пример
  std::ofstream logFile;
  std::string logFileName;

  Logger() {
    logFileName = "factory_log_" + getTimestamp() + ".txt";
    logFile.open(logFileName, std::ios::app);
  }

  std::string getTimestamp() const;

 public:
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  static Logger& getInstance();
  // static делает метод методом класса, а не методом объекта
  // ≈го можно вызывать без создани€ экземпл€ра класса
  // Logs a message with timestamp to the log file
  // Example: log("Production", "Machine M1 started");
  void log(const std::string& module, const std::string& message);

  // Logs an error with timestamp
  void logError(const std::string& module, const std::string& error);

  ~Logger() {
    if (logFile.is_open()) {
      logFile.close();
    }
  }
};
