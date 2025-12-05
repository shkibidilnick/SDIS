#include "Carrier.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

Carrier::Carrier(const std::string& nm, const std::string& contact,
    const std::string& ph, const std::string& em)
    : name(nm), contactPerson(contact), phone(ph), email(em),
    onTimeDeliveryRate(85.0), isActive(true) {
    carrierId = IDGenerator::generate("CAR");

    if (nm.empty() || contact.empty()) {
        throw ShippingException("Carrier name and contact are required");
    }

    if (!Validator::validatePhone(ph)) {
        throw ShippingException("Invalid phone format: " + ph);
    }

    if (!Validator::validateEmail(em)) {
        throw ShippingException("Invalid email format: " + em);
    }

    Logger::getInstance().log("Carrier", "Created carrier " + carrierId + " - " + nm);
}

void Carrier::addServiceType(const std::string& serviceType) {
    if (serviceType.empty()) {
        throw ShippingException("Service type cannot be empty");
    }
    serviceTypes.push_back(serviceType);
}

void Carrier::updatePerformance(bool deliveredOnTime) {
    if (deliveredOnTime) {
        onTimeDeliveryRate = std::min(100.0, onTimeDeliveryRate + 0.5);
    }
    else {
        onTimeDeliveryRate = std::max(0.0, onTimeDeliveryRate - 2.0);
    }
}

bool Carrier::providesService(const std::string& serviceType) const {
    return std::find(serviceTypes.begin(), serviceTypes.end(), serviceType)
        != serviceTypes.end();
}