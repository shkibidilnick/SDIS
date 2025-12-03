#pragma once
#include<string>
class Room;
class Guest;
class Keycard {
private:
	std::string cardId;
	Room* room;
	Guest* guest;
	bool isActive;
public:
	Keycard(std::string id, Room* r, Guest* g);
	void activate();
	void deactivate();
	bool getIsActive() const { return isActive; }
	std::string getCardId() const { return cardId; }
};