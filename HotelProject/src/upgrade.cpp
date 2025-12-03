#include"upgrade.h"
Upgrade::Upgrade(std::string id, Booking* b, Room* f, Room* t)
	:upgradeId(std::move(id)), booking(b), fromRoom(f), toRoom(t), priceAdjustment(0.0){}
void Upgrade::process() {
	if (!toRoom->book()) {
		throw RoomException("Target room not available for upgrade");
	}
	fromRoom->cancel();
	priceAdjustment = toRoom->getPrice() - fromRoom->getPrice();
}