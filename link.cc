#include "link.h"
#include "player.h"

Link* Link::battle(Link* opponent) {
    if (!opponent) return this;

    int myPower = strength + (boosted ? 1 : 0);
    int opponentPower = opponent->getStrength() + (opponent->isBoosted() ? 1 : 0);

    this->reveal();
    opponent->reveal();

    if (myPower > opponentPower) {
        return this;
    } else {
        return opponent;
    }
}