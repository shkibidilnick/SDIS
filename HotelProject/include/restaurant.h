#pragma once
#include <string>
#include <vector>
#include "menuitem.h"
#include "order.h"

class Restaurant {
private:
    std::string restaurantId;
    std::string name;
    std::vector<MenuItem*> menu;
    std::vector<Order*> orders;

public:
    Restaurant(std::string id, std::string restaurantName);

    void addMenuItem(MenuItem* item);
    void removeMenuItem(MenuItem* item);
    void placeOrder(Order* order);

    std::string getRestaurantId() const { return restaurantId; }
};
