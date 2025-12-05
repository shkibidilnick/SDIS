#include "Product.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"

Product::Product(const std::string& code, const std::string& name,
    const std::string& desc, double price)
    : productCode(code), name(name), description(desc),
    unitPrice(price), passedQualityCheck(false), isShipped(false) {

    if (code.empty() || name.empty()) {
        throw ProductionException("Product code and name are required");
    }

    if (price < 0) {
        throw ProductionException("Product price cannot be negative");
    }

    productId = IDGenerator::generate("PROD");
    Logger::getInstance().log("Product", "Created product " + productId + " - " + code);
}

void Product::addRawMaterial(const std::string& materialSku) {
    if (materialSku.empty()) {
        throw ProductionException("Material SKU cannot be empty");
    }
    rawMaterialsUsed.push_back(materialSku);
    Logger::getInstance().log("Product", "Added material " + materialSku + " to " + productId);
}

void Product::markQualityPassed() {
    if (isShipped) {
        throw ProductionException("Cannot quality check already shipped product " + productId);
    }
    passedQualityCheck = true;
    Logger::getInstance().log("Product", "Product " + productId + " passed quality check");
}

void Product::markShipped() {
    if (!passedQualityCheck) {
        throw ProductionException("Cannot ship product " + productId + " without quality approval");
    }
    isShipped = true;
    Logger::getInstance().log("Product", "Product " + productId + " shipped");
}

bool Product::isReadyForShipment() const {
    return passedQualityCheck && !isShipped;
}

std::string Product::getStatus() const {
    if (isShipped) return "SHIPPED";
    if (passedQualityCheck) return "QC_PASSED";
    return "IN_PRODUCTION";
}

std::string Product::generateReport() const {
    std::string report = "Product Report: " + productCode + "\n";
    report += "ID: " + productId + "\n";
    report += "Status: " + getStatus() + "\n";
    report += "Materials Used: " + std::to_string(rawMaterialsUsed.size()) + "\n";
    report += "Price: $" + std::to_string(unitPrice) + "\n";
    return report;
}