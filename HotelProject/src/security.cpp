#include "security.h"
Security::Security(std::string id, Employee* emp)
	:securityId(std::move(id)), guard(emp), onDuty(false){}
void Security::startShift() {
	onDuty = true;
}
void Security::endShift() {
	onDuty = false;
}