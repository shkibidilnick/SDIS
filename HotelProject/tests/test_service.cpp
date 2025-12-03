#include <UnitTest++/UnitTest++.h>

#include "service.h"
#include "test_helpers.h"

TEST(Service_Constructor_CreatesServiceWithCorrectProperties) {
  Service service("S001", "Spa Massage", ServiceType::SPA, 50.0);

  CHECK_EQUAL("S001", service.getId());
  CHECK_EQUAL(ServiceType::SPA, service.getServiceType());
  CHECK_EQUAL(50.0, service.getPrice());
  CHECK_EQUAL(ServiceStatus::AVAILABLE, service.getStatus());
}

TEST(Service_Book_SuccessfullyBooksAvailableService) {
  Service service("S002", "Restaurant Dinner", ServiceType::RESTAURANT, 30.0);

  bool booked = service.book();
  CHECK_EQUAL(true, booked);
  CHECK_EQUAL(ServiceStatus::BOOKED, service.getStatus());
}

TEST(Service_Book_ThrowsWhenNotAvailable) {
  Service service("S003", "Airport Transfer", ServiceType::TRANSFER, 20.0);

  service.book();  // Первое бронирование успешно
  CHECK_THROW(service.book(),
              ServiceException);  // Второе должно бросить исключение
}

TEST(Service_Cancel_SuccessfullyCancelsBookedService) {
  Service service("S004", "Spa Package", ServiceType::SPA, 75.0);

  service.book();
  service.cancel();
  CHECK_EQUAL(ServiceStatus::CANCELLED, service.getStatus());
}

TEST(Service_Complete_SuccessfullyCompletesService) {
  Service service("S005", "VIP Dinner", ServiceType::RESTAURANT, 100.0);

  service.book();
  service.complete();
  CHECK_EQUAL(ServiceStatus::COMPLETED, service.getStatus());
}
