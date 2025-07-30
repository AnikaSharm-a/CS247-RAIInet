#include "link.h"
#include "player.h"

// determines the winner of a battle between two links
// returns the winning link (this link or the opponent)
Link* Link::battle(Link* opponent) {
    if (!opponent) return this;

    // calculate total power including boost effects
    int myPower = strength + (boosted ? 1 : 0);
    int opponentPower = opponent->getStrength() + (opponent->isBoosted() ? 1 : 0);

    // both links are revealed during battle
    this->reveal();
    opponent->reveal();

    // return the link with higher power (opponent wins ties)
    if (myPower < opponentPower) {
        return opponent;
    } else {
        return this;
    }
}
