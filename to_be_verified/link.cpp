#include "link.h"
#include "player.h"

Link::Link(char id, LinkType type, int strength, Player* owner)
    : id(id), type(type), strength(strength), owner(owner) {}

char Link::getId() const {
    return id;
}

LinkType Link::getType() const {
    return type;
}

int Link::getStrength() const {
    return strength;
}

Player* Link::getOwner() const {
    return owner;
}

bool Link::isRevealed() const {
    return revealed;
}

void Link::reveal() {
    revealed = true;
}

bool Link::isBoosted() const {
    return boosted;
}

void Link::boost() {
    boosted = true;
}

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
