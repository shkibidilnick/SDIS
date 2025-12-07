#pragma once
#include "FactoryException.h"

// Thrown when shipping/delivery operations fail
// Examples: carrier delay, damaged goods, lost shipment
class ShippingException : public FactoryException {
 public:
  explicit ShippingException(const std::string& message)
      : FactoryException("Shipping: " + message) {}
};
