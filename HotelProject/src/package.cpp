#include "package.h"

Package::Package(std::string id, std::string t, double disc)
    : packageId(std::move(id)),
      title(std::move(t)),
      discount(disc),
      buyer(nullptr) {}
void Package::addService(Service* s) { services.push_back(s); }
void Package::sellTo(Guest* g) {
  buyer = g;
  g->addPoints(static_cast<int>(getPrice() * 0.1));
}
double Package::getPrice() const {
  double total = 0.0;
  for (auto* s : services) {
    total += s->getPrice();
  }
  return total * (1.0 - discount);
}
