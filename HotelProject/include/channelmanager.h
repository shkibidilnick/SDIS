#pragma once
#include <string>
#include <vector>
class Booking;
class RatePlan;
class ChannelManager {
private:
	std::string managerId;
	std::vector<Booking*> bookings;
	std::vector<RatePlan*> ratePlans;
	std::string channelName;
public:
	ChannelManager(std::string id, std::string channel);
	void addBooking(Booking* b);
	void addRatePlan(RatePlan* rp);
	std::string getManagerId() const { return managerId; }
};