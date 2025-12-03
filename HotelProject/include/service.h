#pragma once
#include <string>

#include "exceptions.h"
#include "interfaces.h"
enum class ServiceType { SPA, RESTAURANT, TRANSFER };
enum class ServiceStatus { AVAILABLE, BOOKED, COMPLETED, CANCELLED };
class Service : public IPricable, public IBookable {
 private:
  std::string id;
  std::string name;
  ServiceType type;
  double price;
  ServiceStatus status;

 public:
  Service(std::string serviceId, std::string serviceName, ServiceType t,
          double p);
  bool book() override;
  void cancel() override;
  double getPrice() const override { return price; }
  void complete();
  std::string getId() const { return id; }
  ServiceType getServiceType() const { return type; }
  ServiceStatus getStatus() const { return status; }
};
