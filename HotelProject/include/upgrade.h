#pragma once
#include<string>
#include "booking.h"
#include"room.h"

class Upgrade {
private:
	std::string upgradeId;
	Booking* booking;
	Room* fromRoom;
	Room* toRoom;
	double priceAdjustment;
public:
	Upgrade(std::string id, Booking* b, Room* f, Room* t);
	void process();
	double getAdjustment() const { return priceAdjustment; }
	std::string getUpgardeId() const { return upgradeId; }
};