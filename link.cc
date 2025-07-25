#include "link.h"

Link* Link::battle(Link* opponent) {
    revealed = true;
    opponent->reveal();
    if (strength > opponent->getStrength()) return this;
    if (strength < opponent->getStrength()) return opponent;
    return this; // tie goes to initiating player
}
