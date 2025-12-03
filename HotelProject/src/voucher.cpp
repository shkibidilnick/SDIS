#include "voucher.h"

Voucher::Voucher(std::string c, Guest* g, double v)
    : code(std::move(c)), owner(g), value(v), status(VoucherStatus::ACTIVE) {}
void Voucher::redeem() { status = VoucherStatus::USED; }
