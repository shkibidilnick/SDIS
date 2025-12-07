#include "AuditLog.h"

#include <algorithm>
#include <sstream>
#include <unordered_map>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

AuditLog::AuditLog(const std::string& moduleName) : isSealed(false) {
  auditId = IDGenerator::generate("AUDIT");

  if (moduleName.empty()) {
    throw FactoryException("Module name is required for audit log");
  }

  Logger::getInstance().log("AuditLog", "Created audit log " + auditId +
                                            " for module: " + moduleName);
}

void AuditLog::addEntry(const std::string& userId, const std::string& action,
                        const std::string& entityType,
                        const std::string& entityId,
                        const std::string& oldValue,
                        const std::string& newValue,
                        const std::string& ipAddress) {
  if (isSealed) {
    throw FactoryException("Cannot add entries to sealed audit log: " +
                           auditId);
  }

  if (userId.empty() || action.empty() || entityType.empty() ||
      entityId.empty()) {
    throw FactoryException("All audit entry fields are required");
  }

  LogEntry entry;
  entry.timestamp = std::chrono::system_clock::now();
  entry.userId = userId;
  entry.action = action;
  entry.entityType = entityType;
  entry.entityId = entityId;
  entry.oldValue = oldValue;
  entry.newValue = newValue;
  entry.ipAddress = ipAddress;

  entries.push_back(entry);

  Logger::getInstance().log("AuditLog", "Added entry to " + auditId + ": " +
                                            userId + " " + action + " " +
                                            entityType + ":" + entityId);
}

void AuditLog::seal() {
  isSealed = true;
  Logger::getInstance().log("AuditLog", "Sealed audit log " + auditId);
}

std::vector<AuditLog::LogEntry> AuditLog::getEntriesInRange(
    const std::string& startDate, const std::string& endDate) const {
  if (!DateTimeUtils::isValidDate(startDate) ||
      !DateTimeUtils::isValidDate(endDate)) {
    throw FactoryException("Invalid date range for audit query");
  }

  std::vector<LogEntry> filtered;
  for (const auto& entry : entries) {
    std::string entryDate =
        DateTimeUtils::getCurrentTimestamp();  // Simplified - need proper date
                                               // extraction
    if (entryDate >= startDate && entryDate <= endDate) {
      filtered.push_back(entry);
    }
  }
  return filtered;
}

std::vector<AuditLog::LogEntry> AuditLog::getEntriesByUser(
    const std::string& userId) const {
  if (userId.empty()) {
    throw FactoryException("User ID is required");
  }

  std::vector<LogEntry> filtered;
  for (const auto& entry : entries) {
    if (entry.userId == userId) {
      filtered.push_back(entry);
    }
  }
  return filtered;
}

std::vector<AuditLog::LogEntry> AuditLog::getEntriesByEntity(
    const std::string& entityType, const std::string& entityId) const {
  if (entityType.empty() || entityId.empty()) {
    throw FactoryException("Entity type and ID are required");
  }

  std::vector<LogEntry> filtered;
  for (const auto& entry : entries) {
    if (entry.entityType == entityType && entry.entityId == entityId) {
      filtered.push_back(entry);
    }
  }
  return filtered;
}

bool AuditLog::detectAnomalies() const {
  // Simple anomaly detection: multiple changes to same entity in short time
  std::unordered_map<std::string, int> entityChangeCount;

  for (const auto& entry : entries) {
    std::string key = entry.entityType + ":" + entry.entityId;
    entityChangeCount[key]++;

    if (entityChangeCount[key] > 5) {  // More than 5 changes is suspicious
      return true;
    }
  }
  return false;
}

std::string AuditLog::exportToCSV() const {
  std::stringstream csv;
  csv << "Timestamp,UserId,Action,EntityType,EntityId,OldValue,NewValue,"
         "IPAddress\n";

  for (const auto& entry : entries) {
    auto time_t = std::chrono::system_clock::to_time_t(entry.timestamp);
    csv << time_t << ",";
    csv << entry.userId << ",";
    csv << entry.action << ",";
    csv << entry.entityType << ",";
    csv << entry.entityId << ",";
    csv << "\"" << entry.oldValue << "\",";
    csv << "\"" << entry.newValue << "\",";
    csv << entry.ipAddress << "\n";
  }

  return csv.str();
}
