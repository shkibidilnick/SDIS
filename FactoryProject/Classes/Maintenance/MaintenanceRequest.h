#pragma once
#include <string>
#include <vector>
#include <memory>
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/MaintenanceException.h"

// Represents a request for machine maintenance or repair
class MaintenanceRequest : public ITrackable {
private:
    std::string requestId;
    std::string machineId;
    std::string issueDescription;
    std::string priority; // "LOW", "MEDIUM", "HIGH", "URGENT"
    std::string status; // "OPEN", "ASSIGNED", "IN_PROGRESS", "COMPLETED"
    std::string requestedDate;
    std::string assignedTechnicianId;
    std::vector<std::string> requiredParts; // SKU of SparePart
    bool isSafetyRelated;

public:
    MaintenanceRequest(const std::string& machineId, const std::string& description,
        const std::string& priority = "MEDIUM");

    // Assigns a technician to the request
    void assignTechnician(const std::string& techId);

    // Adds a required spare part
    void addRequiredPart(const std::string& partSku);

    // Updates request status
    void updateStatus(const std::string& newStatus);

    // Marks as completed
    void complete();
    std::string getIssueDescription() const { return issueDescription; }

    // ITrackable implementation
    std::string getTrackingId() const override { return requestId; }
    std::string getStatus() const override { return status; }

    std::string getRequestId() const { return requestId; }
    std::string getMachineId() const { return machineId; }
    std::string getPriority() const { return priority; }
    std::string getAssignedTechnicianId() const { return assignedTechnicianId; }
    bool getIsSafetyRelated() const { return isSafetyRelated; }
    void setSafetyRelated(bool safety) { isSafetyRelated = safety; }
};
