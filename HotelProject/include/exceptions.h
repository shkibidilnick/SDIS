#pragma once
#include <stdexcept>
#include <string>

struct HotelException : std::runtime_error {
  explicit HotelException(const std::string& msg) : std::runtime_error(msg) {}
};

struct RoomException : HotelException {
  using HotelException::HotelException;
};
struct GuestException : HotelException {
  using HotelException::HotelException;
};
struct EmployeeException : HotelException {
  using HotelException::HotelException;
};
struct BookingException : HotelException {
  using HotelException::HotelException;
};
struct PaymentException : HotelException {
  using HotelException::HotelException;
};
struct ServiceException : HotelException {
  using HotelException::HotelException;
};
struct InvoiceException : HotelException {
  using HotelException::HotelException;
};
struct ReviewException : HotelException {
  using HotelException::HotelException;
};
struct LoyaltyException : HotelException {
  using HotelException::HotelException;
};
struct RestaurantException : HotelException {
  using HotelException::HotelException;
};
struct InventoryException : HotelException {
  using HotelException::HotelException;
};
struct ReportException : HotelException {
  using HotelException::HotelException;
};
struct TaxException : HotelException {
  using HotelException::HotelException;
};
struct PenaltyException : HotelException {
	using HotelException::HotelException;
};
