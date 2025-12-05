#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/FactoryException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"

// Represents an audit log entry for tracking system changes
// Provides immutable audit trail for compliance and troubleshooting
class AuditLog : public ITrackable {
 private:
  struct LogEntry {
    std::chrono::system_clock::time_point timestamp;
    std::string userId;
    std::string action;
    std::string entityType;
    std::string entityId;
    std::string oldValue;
    std::string newValue;
    std::string ipAddress;
  };

  std::string auditId;
  std::vector<LogEntry> entries;
  bool isSealed;  // Once sealed, no more entries can be added

 public:
  AuditLog(const std::string& moduleName);

  // Adds an audit entry (immutable after sealing)
  void addEntry(const std::string& userId, const std::string& action,
                const std::string& entityType, const std::string& entityId,
                const std::string& oldValue, const std::string& newValue,
                const std::string& ipAddress = "127.0.0.1");

  // Seals the audit log (e.g., at end of day for archival)
  void seal();

  // Gets entries within a date range
  std::vector<LogEntry> getEntriesInRange(const std::string& startDate,
                                          const std::string& endDate) const;

  // Gets entries for a specific user
  std::vector<LogEntry> getEntriesByUser(const std::string& userId) const;

  // Gets entries for a specific entity
  std::vector<LogEntry> getEntriesByEntity(const std::string& entityType,
                                           const std::string& entityId) const;

  // Checks if log contains suspicious activity
  bool detectAnomalies() const;

  // Exports audit log to CSV format for external analysis
  std::string exportToCSV() const;

  // ITrackable implementation
  std::string getTrackingId() const override { return auditId; }
  std::string getStatus() const override {
    return isSealed ? "SEALED" : "ACTIVE";
  }

  size_t getEntryCount() const { return entries.size(); }
  bool getIsSealed() const { return isSealed; }
};
