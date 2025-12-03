#include "deposit.h"

Deposit::Deposit(std::string id, Booking* b, Payment* p, double amt)
	:depositId(std::move(id)), booking(b), payment(p), amount(amt), isRefundable(true){}
void Deposit::hold() {
	if (payment && !payment->isCompleted()) {
		payment->processPayment();
	}
}
void Deposit::release() {
	if (payment && isRefundable && payment->isCompleted()) {
		payment->refund();
	}
}