#include "Logger.h"

#include <mutex>  // mutual exclusion - взаимное исключение (только один поток может писать в файл в любой момент времени)

Logger* Logger::instance = nullptr;
std::mutex loggerMutex;

Logger& Logger::getInstance() {
  std::lock_guard<std::mutex> lock(loggerMutex);
  if (!instance) {
    instance = new Logger();
  }
  return *instance;
}

std::string Logger::getTimestamp() const {
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);
  return std::to_string(time_t);
}

void Logger::log(const std::string& module, const std::string& message) {
  std::lock_guard<std::mutex> lock(loggerMutex);
  if (logFile.is_open()) {
    logFile << "[" << getTimestamp() << "] [" << module << "] " << message
            << std::endl;
  }
}

void Logger::logError(const std::string& module, const std::string& error) {
  log(module, "ERROR: " + error);
}
