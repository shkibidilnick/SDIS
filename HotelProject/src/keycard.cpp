#include "keycard.h"
Keycard::Keycard(std::string id, Room* r, Guest* g)
	:cardId(std::move(id)), room(r), guest(g), isActive(true){}
void Keycard::activate() {
	isActive = true;
}
void Keycard::deactivate() {
	isActive = false;
}