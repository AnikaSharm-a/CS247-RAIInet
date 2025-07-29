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
    bool jammed = false;
    int jammedOnTurn = -1; // Track which turn the jam was applied

public:
    Link(char id, LinkType type, int strength, Player* owner)
        : id(id), type(type), strength(strength), owner(owner) {}

    char getId() const { return id; }
    void setType(LinkType newType) { type = newType; }
    LinkType getType() const { return type; }
    int getStrength() const { return strength; }
    Player* getOwner() const { return owner; }
    bool isRevealed() const { return revealed; }
    void reveal() { revealed = true; }
    bool isBoosted() const { return boosted; }
    void boost() { boosted = true; }
    void removeBoost() { boosted = false; }
    bool isJammed() const { return jammed; }
    void jam(int turnNumber) { jammed = true; jammedOnTurn = turnNumber; }
    void unjam() { jammed = false; jammedOnTurn = -1; }
    int getJammedOnTurn() const { return jammedOnTurn; }

    Link* battle(Link* opponent);
};