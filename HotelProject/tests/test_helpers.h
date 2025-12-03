#pragma once
#include <iostream>
#include <ostream>

#include "booking.h"
#include "employee.h"
#include "room.h"
#include "service.h"

inline std::ostream& operator<<(std::ostream& os, const BookingStatus& status) {
  switch (status) {
    case BookingStatus::PENDING:
      return os << "PENDING";
    case BookingStatus::CONFIRMED:
      return os << "CONFIRMED";
    case BookingStatus::CHECKED_IN:
      return os << "CHECKED_IN";
    case BookingStatus::CHECKED_OUT:
      return os << "CHECKED_OUT";
    case BookingStatus::CANCELLED:
      return os << "CANCELLED";
    default:
      return os << "UNKNOWN";
  }
}

inline std::ostream& operator<<(std::ostream& os, const RoomType& type) {
  switch (type) {
    case RoomType::SINGLE:
      return os << "SINGLE";
    case RoomType::DOUBLE:
      return os << "DOUBLE";
    case RoomType::SUITE:
      return os << "SUITE";
    default:
      return os << "UNKNOWN";
  }
}

inline std::ostream& operator<<(std::ostream& os, const Role& role) {
  switch (role) {
    case Role::MANAGER:
      return os << "MANAGER";
    case Role::RECEPTIONIST:
      return os << "RECEPTIONIST";
    case Role::CLEANER:
      return os << "CLEANER";
    default:
      return os << "UNKNOWN";
  }
}

inline std::ostream& operator<<(std::ostream& os, const PaymentMethod& method) {
  switch (method) {
    case PaymentMethod::CASH:
      return os << "CASH";
    case PaymentMethod::CARD:
      return os << "CARD";
    case PaymentMethod::TRANSFER:
      return os << "TRANSFER";
    default:
      return os << "UNKNOWN";
  }
}

inline std::ostream& operator<<(std::ostream& os, const ServiceType& type) {
  switch (type) {
    case ServiceType::SPA:
      return os << "SPA";
    case ServiceType::RESTAURANT:
      return os << "RESTAURANT";
    case ServiceType::TRANSFER:
      return os << "TRANSFER";
    default:
      return os << "UNKNOWN";
  }
}

inline std::ostream& operator<<(std::ostream& os, const ServiceStatus& status) {
  switch (status) {
    case ServiceStatus::AVAILABLE:
      return os << "AVAILABLE";
    case ServiceStatus::BOOKED:
      return os << "BOOKED";
    case ServiceStatus::COMPLETED:
      return os << "COMPLETED";
    case ServiceStatus::CANCELLED:
      return os << "CANCELLED";
    default:
      return os << "UNKNOWN";
  }
}
