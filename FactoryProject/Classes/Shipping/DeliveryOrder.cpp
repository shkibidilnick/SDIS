#include "DeliveryOrder.h"

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/DateTimeUtils.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/IDGenerator.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Logger.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Utils/Validator.h"

DeliveryOrder::DeliveryOrder(const std::string& custId,
                             const std::string& address,
                             const std::string& reqDeliveryDate)
    : customerId(custId),
      shipToAddress(address),
      requiredDeliveryDate(reqDeliveryDate),
      status("PENDING"),
      shippingCost(0.0) {
  deliveryOrderId = IDGenerator::generate("DO");

  if (custId.empty() || address.empty()) {
    throw ShippingException("Customer ID and shipping address are required");
  }

  if (!DateTimeUtils::isValidDate(reqDeliveryDate)) {
    throw ShippingException("Invalid delivery date: " + reqDeliveryDate);
  }

  Logger::getInstance().log(
      "DeliveryOrder",
      "Created order " + deliveryOrderId + " for customer: " + custId);
}

void DeliveryOrder::addProduct(std::shared_ptr<Product> product, int quantity) {
  if (!product) {
    throw ShippingException("Cannot add null product to delivery order");
  }

  Validator::validatePositive(quantity, "Product quantity");

  if (!product->isReadyForShipment()) {
    throw ShippingException("Product " + product->getProductId() +
                            " is not ready for shipment");
  }

  products.push_back({product, quantity});

  Logger::getInstance().log(
      "DeliveryOrder", "Added product " + product->getProductId() + " x" +
                           std::to_string(quantity) + " to " + deliveryOrderId);
}

void DeliveryOrder::assignCarrier(const std::string& carrId, double cost) {
  if (carrId.empty()) {
    throw ShippingException("Carrier ID cannot be empty");
  }

  Validator::validatePositive(cost, "Shipping cost");

  carrierId = carrId;
  shippingCost = cost;

  Logger::getInstance().log(
      "DeliveryOrder", "Assigned carrier " + carrId + " to " + deliveryOrderId);
}

void DeliveryOrder::ship(const std::string& trackingNum) {
  if (status != "PENDING") {
    throw ShippingException("Cannot ship order in status: " + status);
  }

  if (products.empty()) {
    throw ShippingException("Cannot ship order without products");
  }

  if (carrierId.empty()) {
    throw ShippingException("Cannot ship without assigned carrier");
  }

  trackingNumber = trackingNum;
  status = "SHIPPED";
  shipDate = DateTimeUtils::getCurrentTimestamp();

  // Mark products as shipped
  for (auto& pair : products) {
    pair.first->markShipped();
  }

  Logger::getInstance().log(
      "DeliveryOrder",
      "SHIPPED order " + deliveryOrderId + " with tracking: " + trackingNum);
}

void DeliveryOrder::markDelivered() {
  if (status != "SHIPPED") {
    throw ShippingException("Cannot mark delivered order in status: " + status);
  }

  status = "DELIVERED";

  Logger::getInstance().log("DeliveryOrder",
                            "DELIVERED order " + deliveryOrderId);
}

double DeliveryOrder::getOrderValue() const {
  double total = 0.0;
  for (const auto& pair : products) {
    total += pair.first->getUnitPrice() * pair.second;
  }
  return total;
}

void DeliveryOrder::validate() const {
  if (customerId.empty() || shipToAddress.empty()) {
    throw ShippingException("Customer and address are required");
  }

  if (products.empty()) {
    throw ShippingException("Order must have at least one product");
  }

  if (carrierId.empty()) {
    throw ShippingException("Carrier must be assigned");
  }

  for (const auto& pair : products) {
    if (!pair.first->isReadyForShipment()) {
      throw ShippingException("Product " + pair.first->getProductId() +
                              " is not ready");
    }
  }
}
