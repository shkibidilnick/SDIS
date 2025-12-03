#include "lostfound.h"

LostFound::LostFound(std::string id, std::string desc, Guest* g)
    : itemId(std::move(id)),
      description(std::move(desc)),
      owner(g),
      returned(false) {}

void LostFound::markReturned() { returned = true; }
