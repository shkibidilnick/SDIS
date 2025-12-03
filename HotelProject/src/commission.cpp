#include "commission.h"

Commission::Commission(std::string id, Booking* b, Payment* p, double r)
	:commissionId(std::move(id)), booking(b), payment(p), rate(r), amount(0.0){}
void Commission::calculate() {
	if (!payment) { return; }
	amount = payment->getAmount() * rate;
}