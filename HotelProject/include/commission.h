#pragma once
#include <string>
#include "payment.h"
class Booking;
class Commission {
private:
	std::string commissionId;
	Booking* booking;
	Payment* payment;
	double rate; //commission - %
	double amount;
public:
	Commission(std::string id, Booking* b, Payment* p, double r);
	void calculate();
	double getAmount() const { return amount; }
	std::string getCommission() const { return commissionId; }
};