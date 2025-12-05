#pragma once
#include <memory>
#include <string>
#include <vector>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/Product.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ShippingException.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/ITrackable.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Interfaces/IValidatable.h"

// Represents a delivery order for shipping products to customers
class DeliveryOrder : public ITrackable, public IValidatable {
 private:
  std::string deliveryOrderId;
  std::string customerId;
  std::string shipToAddress;
  std::string shipDate;
  std::string requiredDeliveryDate;
  std::vector<std::pair<std::shared_ptr<Product>, int>>
      products;  // Product, Quantity
  std::string carrierId;
  std::string status;  // "PENDING", "SHIPPED", "DELIVERED", "CANCELLED"
  double shippingCost;
  std::string trackingNumber;

 public:
  DeliveryOrder(const std::string& customerId, const std::string& shipToAddress,
                const std::string& requiredDeliveryDate);

  // Adds a product to the order
  void addProduct(std::shared_ptr<Product> product, int quantity);

  // Assigns a carrier
  void assignCarrier(const std::string& carrierId, double cost);

  // Ships the order
  void ship(const std::string& trackingNum);

  // Marks as delivered
  void markDelivered();

  // Calculates total order value
  double getOrderValue() const;

  // ITrackable implementation
  std::string getTrackingId() const override { return deliveryOrderId; }
  std::string getStatus() const override { return status; }

  // IValidatable implementation
  void validate() const override;

  std::string getDeliveryOrderId() const { return deliveryOrderId; }
  std::string getCustomerId() const { return customerId; }
  std::string getCarrierId() const { return carrierId; }
  std::string getTrackingNumber() const { return trackingNumber; }
  double getShippingCost() const { return shippingCost; }
};
