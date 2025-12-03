#include "transportation.h"

Transportation::Transportation(std::string id, Guest* g, TransportType t, double c)
	:transportId(std::move(id)), guest(g), type(t), cost(c), isConfirmed(false){}
void Transportation::confirm() {
	isConfirmed = true;
 }

void Transportation::cancel() {
	isConfirmed = false;
}