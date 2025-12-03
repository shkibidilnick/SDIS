#include "concierge.h"

Concierge::Concierge(std::string id, Guest* g)
	:conciergeId(std::move(id)), guest(g), transport(nullptr), event(nullptr){}
void Concierge::arrangeTransport(Transportation* t) {
	transport = t;
}
void Concierge::bookEvent(Event* e) {
	event = e;
}