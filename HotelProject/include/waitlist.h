#pragma once
#include<vector>
#include"guest.h"
#include"room.h"
class WaitList {
private:
	std::string listId;
	std::vector<Guest*> guests;
	Room* desiredRoom;
	int maxWait;
public:
	WaitList(std::string id, Room* r, int max);
	void addGuest(Guest* g);
	Guest* popNext();
	Room* getRoom() const { return desiredRoom; }
};