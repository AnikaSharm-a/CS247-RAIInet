#pragma once
#include "linkType.h"
#include "player.h"

class Player; // forward declaration

class Link {
    char id;
    LinkType type;
    int strength;
    Player* owner;
    bool revealed = false;
    bool boosted = false;

public:
    Link(char id, LinkType type, int strength, Player* owner)
        : id(id), type(type), strength(strength), owner(owner) {}

    char getId() const { return id; }
    LinkType getType() const { return type; }
    int getStrength() const { return strength; }
    Player* getOwner() const { return owner; }
    bool isRevealed() const { return revealed; }
    void reveal() { revealed = true; }
    bool isBoosted() const { return boosted; }
    void boost() { boosted = true; }

    // Simulate a battle between this and opponent
    Link* battle(Link* opponent); // implemented below
};
