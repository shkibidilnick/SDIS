#pragma once
#include <string>
#include <vector>
#include <memory>
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ShippingException.h"

// Represents a shipping carrier or logistics provider
class Carrier : public ITrackable {
private:
    std::string carrierId;
    std::string name;
    std::string contactPerson;
    std::string phone;
    std::string email;
    std::vector<std::string> serviceTypes; // "GROUND", "AIR", "EXPRESS"
    double onTimeDeliveryRate; // 0-100%
    bool isActive;

public:
    Carrier(const std::string& name, const std::string& contact, const std::string& phone,
        const std::string& email);

    // Adds a service type
    void addServiceType(const std::string& serviceType);

    // Updates delivery performance metrics
    void updatePerformance(bool deliveredOnTime);

    // Checks if carrier provides a specific service
    bool providesService(const std::string& serviceType) const;

    // ITrackable implementation
    std::string getTrackingId() const override { return carrierId; }
    std::string getStatus() const override { return isActive ? "ACTIVE" : "INACTIVE"; }

    std::string getCarrierId() const { return carrierId; }
    std::string getName() const { return name; }
    double getOnTimeDeliveryRate() const { return onTimeDeliveryRate; }
    void setActive(bool active) { isActive = active; }
};
