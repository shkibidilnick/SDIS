#pragma once
#include <string>

class Guest;
class Transportation;
class Event;

class Concierge {
private:
	std::string conciergeId;
	Guest* guest;
	Transportation* transport;
	Event* event;
public:
	Concierge(std::string id, Guest* g);
	void arrangeTransport(Transportation* t);
	void bookEvent(Event* e);
	std::string getId() const { return conciergeId; }
};