#include <UnitTest++/UnitTest++.h>

#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Production/Product.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Shipping/Carrier.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Shipping/DeliveryOrder.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Classes/Shipping/Shipment.h"
#include "/Users/anteiku/Documents/SDIS/lab3_sdis/Exceptions/ShippingException.h"

TEST(Carrier_Constructor_CreatesCarrierWithCorrectProperties) {
  Carrier carrier("Fast Freight", "John Smith", "555-0100",
                  "dispatcher@fastfreight.com");

  CHECK(!carrier.getTrackingId().empty());
  CHECK_EQUAL("Fast Freight", carrier.getName());
  CHECK_EQUAL("John Smith", carrier.getContactPerson());
  CHECK_EQUAL(85.0, carrier.getOnTimeDeliveryRate());
  CHECK_EQUAL(true, carrier.getIsActive());
}

TEST(Carrier_Constructor_ThrowsForInvalidContact) {
  CHECK_THROW(Carrier("Bad Carrier", "", "invalid-phone", "invalid-email"),
              ShippingException);
}

TEST(Carrier_AddServiceType_AddsServiceToList) {
  Carrier carrier("Logistics Pro", "Jane Doe", "555-0200",
                  "info@logistics.com");

  carrier.addServiceType("GROUND");
  carrier.addServiceType("AIR");
  carrier.addServiceType("EXPRESS");

  CHECK_EQUAL(true, carrier.providesService("GROUND"));
  CHECK_EQUAL(true, carrier.providesService("AIR"));
  CHECK_EQUAL(false, carrier.providesService("RAIL"));
}

TEST(Carrier_UpdatePerformance_UpdatesOnTimeRate) {
  Carrier carrier("Reliable Transport", "Bob Brown", "555-0300",
                  "dispatch@reliable.com");

  carrier.updatePerformance(true);
  CHECK_CLOSE(85.5, carrier.getOnTimeDeliveryRate(), 0.01);

  carrier.updatePerformance(false);
  CHECK_CLOSE(83.5, carrier.getOnTimeDeliveryRate(), 0.01);
}

TEST(Carrier_UpdatePerformance_MaintainsBounds) {
  Carrier carrier("Perfect Delivery", "Alice", "555-0400", "info@perfect.com");

  for (int i = 0; i < 40; ++i) {
    carrier.updatePerformance(true);
  }
  CHECK_EQUAL(100.0, carrier.getOnTimeDeliveryRate());

  for (int i = 0; i < 60; ++i) {
    carrier.updatePerformance(false);
  }
  CHECK_EQUAL(0.0, carrier.getOnTimeDeliveryRate());
}

TEST(DeliveryOrder_Constructor_CreatesOrderWithCorrectProperties) {
  DeliveryOrder order("CUST001", "123 Main St, City, State 12345",
                      "2025-02-15");

  CHECK(!order.getTrackingId().empty());
  CHECK_EQUAL("CUST001", order.getCustomerId());
  CHECK_EQUAL("123 Main St, City, State 12345", order.getShipToAddress());
  CHECK_EQUAL("PENDING", order.getStatus());
  CHECK_EQUAL(0.0, order.getOrderValue());
}

TEST(DeliveryOrder_Constructor_ThrowsForInvalidData) {
  CHECK_THROW(DeliveryOrder("", "Address", "2025-02-15"), ShippingException);
  CHECK_THROW(DeliveryOrder("CUST001", "", "2025-02-15"), ShippingException);
  CHECK_THROW(DeliveryOrder("CUST001", "Address", "invalid-date"),
              ShippingException);
}

TEST(DeliveryOrder_AddProduct_AddsProductToOrder) {
  DeliveryOrder order("CUST001", "Address", "2025-02-15");
  auto product =
      std::make_shared<Product>("PROD-001", "Widget", "Description", 45.99);

  product->markQualityPassed();

  order.addProduct(product, 10);

  CHECK_CLOSE(459.9, order.getOrderValue(), 0.01);
}

TEST(DeliveryOrder_AddProduct_ThrowsWhenNotReady) {
  DeliveryOrder order("CUST001", "Address", "2025-02-15");
  auto product =
      std::make_shared<Product>("PROD-001", "Widget", "Description", 45.99);

  CHECK_THROW(order.addProduct(product, 10), ShippingException);
}

TEST(DeliveryOrder_AssignCarrier_AssignsCarrierAndCost) {
  DeliveryOrder order("CUST001", "Address", "2025-02-15");

  order.assignCarrier("CAR001", 125.5);

  CHECK_EQUAL("CAR001", order.getCarrierId());
  CHECK_CLOSE(125.5, order.getShippingCost(), 0.01);
}

TEST(DeliveryOrder_AssignCarrier_ThrowsForInvalidData) {
  DeliveryOrder order("CUST001", "Address", "2025-02-15");

  CHECK_THROW(order.assignCarrier("", 100.0), ShippingException);
  CHECK_THROW(order.assignCarrier("CAR001", -50.0), ShippingException);
}

TEST(DeliveryOrder_Ship_ChangesStatusToShipped) {
  DeliveryOrder order("CUST001", "Address", "2025-02-15");
  auto product =
      std::make_shared<Product>("PROD-001", "Widget", "Description", 45.99);

  product->markQualityPassed();
  order.addProduct(product, 10);
  order.assignCarrier("CAR001", 100.0);
  order.ship("TRACK12345");

  CHECK_EQUAL("SHIPPED", order.getStatus());
  CHECK_EQUAL("TRACK12345", order.getTrackingNumber());
  CHECK_EQUAL(true, product->getIsShipped());
}

TEST(DeliveryOrder_Ship_ThrowsWhenNotReady) {
  DeliveryOrder order("CUST001", "Address", "2025-02-15");
  CHECK_THROW(order.ship("TRACK12345"), ShippingException);
}

TEST(DeliveryOrder_MarkDelivered_ChangesStatusToDelivered) {
  DeliveryOrder order("CUST001", "Address", "2025-02-15");
  auto product =
      std::make_shared<Product>("PROD-001", "Widget", "Description", 45.99);

  product->markQualityPassed();
  order.addProduct(product, 10);
  order.assignCarrier("CAR001", 100.0);
  order.ship("TRACK12345");
  order.markDelivered();

  CHECK_EQUAL("DELIVERED", order.getStatus());
}

TEST(DeliveryOrder_MarkDelivered_ThrowsWhenNotShipped) {
  DeliveryOrder order("CUST001", "Address", "2025-02-15");

  CHECK_THROW(order.markDelivered(), ShippingException);
}

TEST(Shipment_Constructor_CreatesShipmentWithCorrectProperties) {
  Shipment shipment("CUST001", "Address", "2025-02-15");

  CHECK(!shipment.getShipmentId().empty());
  CHECK_EQUAL(0.0, shipment.getTotalWeight());
  CHECK_EQUAL(0.0, shipment.getTotalVolume());
  CHECK_EQUAL(false, shipment.getIsInsured());
}

TEST(Shipment_AddProductWithDetails_AddsProductAndUpdatesMetrics) {
  Shipment shipment("CUST001", "Address", "2025-02-15");
  auto product = std::make_shared<Product>("PROD-001", "Heavy Widget",
                                           "Description", 150.0);

  product->markQualityPassed();
  shipment.addProductWithDetails(product, 5, 10.5,
                                 0.02);  // 5 items, 10.5kg each, 0.02m3 each

  CHECK_CLOSE(52.5, shipment.getTotalWeight(), 0.01);  // 5 * 10.5
  CHECK_CLOSE(0.1, shipment.getTotalVolume(), 0.01);   // 5 * 0.02
}

TEST(Shipment_SetPackaging_SetsPackagingType) {
  Shipment shipment("CUST001", "Address", "2025-02-15");

  shipment.setPackaging("PALLET", 5);

  CHECK_EQUAL("PALLET", shipment.getPackagingType());
  CHECK_EQUAL(5, shipment.getPackageCount());
}

TEST(Shipment_SetPackaging_ThrowsForInvalidType) {
  Shipment shipment("CUST001", "Address", "2025-02-15");

  CHECK_THROW(shipment.setPackaging("INVALID_TYPE", 3), ShippingException);
}

TEST(Shipment_SetInsurance_SetsInsuranceFlagAndValue) {
  Shipment shipment("CUST001", "Address", "2025-02-15");

  shipment.setInsurance(5000.0);

  CHECK_EQUAL(true, shipment.getIsInsured());
  CHECK_CLOSE(5000.0, shipment.getDeclaredValue(), 0.01);
}

TEST(Shipment_GenerateCustomsDocumentation_GeneratesDocs) {
  Shipment shipment("CUST001", "Address", "2025-02-15");

  shipment.setInsurance(10000.0);
  shipment.generateCustomsDocumentation();

  std::string docs = shipment.getCustomsDocumentation();
  CHECK(docs.find("Customs Declaration") != std::string::npos);
  CHECK(docs.find("10000") != std::string::npos);
}

TEST(Shipment_GenerateCustomsDocumentation_ThrowsWhenNotInsured) {
  Shipment shipment("CUST001", "Address", "2025-02-15");

  CHECK_THROW(shipment.generateCustomsDocumentation(), ShippingException);
}

TEST(Shipment_CalculateDimensionalWeight_ReturnsCorrectWeight) {
  Shipment shipment("CUST001", "Address", "2025-02-15");

  auto product = std::make_shared<Product>("PROD-001", "Item", "Desc", 50.0);
  product->markQualityPassed();
  shipment.addProductWithDetails(product, 1, 50.0, 0.5);

  CHECK_CLOSE(100.0, shipment.calculateDimensionalWeight(), 1.0);
}

TEST(Shipment_RequiresSpecialHandling_ReturnsCorrectStatus) {
  Shipment shipment1("CUST001", "Address", "2025-02-15");
  Shipment shipment2("CUST001", "Address", "2025-02-15");
  Shipment shipment3("CUST001", "Address", "2025-02-15");

  auto product1 = std::make_shared<Product>("PROD-001", "Heavy", "Desc", 100.0);
  product1->markQualityPassed();
  shipment1.addProductWithDetails(product1, 1, 101.0, 0.1);

  auto product2 =
      std::make_shared<Product>("PROD-002", "Expensive", "Desc", 5000.0);
  product2->markQualityPassed();
  shipment2.addProduct(product2, 1);
  shipment2.setInsurance(6000.0);

  auto product3 =
      std::make_shared<Product>("PROD-003", "Normal", "Desc", 100.0);
  product3->markQualityPassed();
  shipment3.addProduct(product3, 1);

  CHECK_EQUAL(true, shipment1.requiresSpecialHandling());
  CHECK_EQUAL(true, shipment2.requiresSpecialHandling());
  CHECK_EQUAL(false, shipment3.requiresSpecialHandling());
}
