#pragma once
#include <string>

#include "guest.h"

enum class VoucherStatus { ACTIVE, USED, EXPIRED };  // expired - просрочен
class Voucher {
 private:
  std::string code;
  Guest* owner;  // owner of voucher(e.g. sertificate)
  double value;
  VoucherStatus status;

 public:
  Voucher(std::string c, Guest* g, double v);
  void redeem();
  double getValue() const { return value; }
  std::string getCode() const { return code; }
};
