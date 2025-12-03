#include <UnitTest++/UnitTest++.h>

#include "invoice.h"
#include "test_helpers.h"

TEST(Invoice_Constructor_CreatesInvoiceWithCorrectProperties) {
  Guest guest("G001", "John");
  Room room("101", RoomType::SINGLE, 100.0, 1);
  Booking booking("B001", &guest, &room, "2024-01-01", "2024-01-03");
  Invoice invoice("INV001", &booking);

  CHECK_EQUAL("INV001", invoice.getInvoiceId());
  CHECK_EQUAL(false, invoice.getPaidStatus());
  CHECK_EQUAL(0.0, invoice.getTaxAmount());
  CHECK_EQUAL(0.0, invoice.getPenaltyAmount());
}

TEST(Invoice_MarkAsPaid_SetsInvoiceAsPaid) {
  Guest guest("G002", "Jane");
  Room room("102", RoomType::DOUBLE, 150.0, 2);
  Booking booking("B002", &guest, &room, "2024-01-01", "2024-01-03");
  Invoice invoice("INV002", &booking);

  invoice.markAsPaid();
  CHECK_EQUAL(true, invoice.getPaidStatus());
  CHECK_EQUAL(0.0, invoice.getRemaining());
}
