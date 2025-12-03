#include"channelmanager.h"
ChannelManager::ChannelManager(std::string id, std::string channel)
	:managerId(std::move(id)), channelName(std::move(channel)){}
void ChannelManager::addBooking(Booking* b) {
	bookings.push_back(b);
}
void ChannelManager::addRatePlan(RatePlan* rp) {
	ratePlans.push_back(rp);
}