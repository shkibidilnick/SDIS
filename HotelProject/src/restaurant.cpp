#include "restaurant.h"

Restaurant::Restaurant(std::string id, std::string restaurantName)
    : restaurantId(std::move(id)), name(std::move(restaurantName)) {}

void Restaurant::addMenuItem(MenuItem* item) {
    if (item) menu.push_back(item);
}

void Restaurant::removeMenuItem(MenuItem* item) {
    menu.erase(std::remove(menu.begin(), menu.end(), item), menu.end());
}

void Restaurant::placeOrder(Order* order) {
    if (order) orders.push_back(order);
}