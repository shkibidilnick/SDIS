#include "waitlist.h"

WaitList::WaitList(std::string id, Room* r, int max)
	:listId(std::move(id)), desiredRoom(r), maxWait(max){}
void WaitList::addGuest(Guest* g) {
	if (guests.size() < maxWait) {
		guests.push_back(g);
	}
}
Guest* WaitList::popNext() {
	if (guests.empty()) return nullptr;
	Guest* g = guests.front();
	guests.erase(guests.begin());
	return g;
}