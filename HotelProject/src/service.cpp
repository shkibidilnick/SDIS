#include "service.h"

Service::Service(std::string serviceId, std::string serviceName, ServiceType t,
                 double p)
    : id(std::move(serviceId)),
      name(std::move(serviceName)),
      type(t),
      price(p),
      status(ServiceStatus::AVAILABLE) {}
bool Service::book() {
  if (status != ServiceStatus::AVAILABLE)
    throw ServiceException("Not available");
  status = ServiceStatus::BOOKED;
  return true;
}
void Service::cancel() {
  if (status == ServiceStatus::COMPLETED)
    throw ServiceException("Cannot cancel completed");
  status = ServiceStatus::CANCELLED;
}
void Service::complete() { status = ServiceStatus::COMPLETED; }
