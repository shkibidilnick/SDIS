#include "penalty.h"
#include "invoice.h"
#include "payment.h"
#include "exceptions.h"

static std::string penaltyTypeToString(PenaltyType t) {
    switch (t) {
    case PenaltyType::CANCELLATION: return "Cancellation Fee";
    case PenaltyType::DAMAGE: return "Property Damage";
    case PenaltyType::SMOKING: return "Smoking Violation";
    }
    return "Unknown";
}

Penalty::Penalty(std::string id, Booking* b, Invoice* inv, Payment* p,
    PenaltyType t, double r, double fixed)
    : penaltyId(std::move(id)), booking(b), invoice(inv), payment(p), type(t),
    rate(r), fixedAmount(fixed), calculatedAmount(0.0) {

    if (r < 0.0 || r > 1.0) {
        throw PenaltyException("Invalid penalty rate: " + std::to_string(r));
    }
}
void Penalty::apply() {
    if (!invoice) {
        throw PenaltyException("Cannot apply penalty without invoice");
    }
    double baseAmount = 0.0;
    if (payment) {
        baseAmount = payment->getAmount();
    }

    calculatedAmount = calculate(baseAmount);

    invoice->addPenalty(this);

    std::ofstream logFile("penalties.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << "[Penalty Applied] ID: " << penaltyId
            << " Type: " << penaltyTypeToString(type)
            << " Amount: $" << calculatedAmount
            << " Invoice: " << invoice->getInvoiceId() << "\n";
        logFile.close();
    }
}

double Penalty::calculate(double baseAmount) const {
    if (type == PenaltyType::SMOKING) {
        return fixedAmount; 
    }

    double result = baseAmount * rate + fixedAmount;

    return std::round(result * 100.0) / 100.0;
}

std::string Penalty::getDescription() const {
    std::string desc = penaltyTypeToString(type);
    if (rate > 0) {
        desc += " (" + std::to_string(static_cast<int>(rate * 100)) + "%)";
    }
    if (fixedAmount > 0) {
        desc += " + $" + std::to_string(static_cast<int>(fixedAmount));
    }
    return desc;
}