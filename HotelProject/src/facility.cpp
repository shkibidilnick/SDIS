#include "facility.h"

Facility::Facility(std::string id, std::string facilityName, Service* s,
                   Employee* m)
    : facilityId(std::move(id)),
      name(std::move(facilityName)),
      mainService(s),
      manager(m),
      isOpen(false) {}
void Facility::open() { isOpen = true; }
void Facility::close() { isOpen = false; }
