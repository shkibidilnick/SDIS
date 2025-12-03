#include "menuitem.h"

MenuItem::MenuItem(std::string id, std::string itemName, double p,
                   std::string cat)
    : itemId(std::move(id)),
      name(std::move(itemName)),
      price(p),
      category(std::move(cat)) {}
