#ifndef LINK_H
#define LINK_H

#include "linkType.h"
#include "player.h"

class Player;

// represents a link (data or virus) that can move and battle on the board
class Link {
    char id;
    LinkType type; // data or virus
    int strength;
    Player* owner;
    bool revealed = false;
    bool boosted = false;
    bool jammed = false;
    int jammedOnTurn = -1; // track which turn the jam was applied

public:
    Link(char id, LinkType type, int strength, Player* owner) : id(id), type(type), strength(strength), owner(owner) {}

    // basic getters and setters
    char getId() const { return id; }
    void setType(LinkType newType) { type = newType; }
    LinkType getType() const { return type; }
    int getStrength() const { return strength; }
    Player* getOwner() const { return owner; }
    
    // visibility management
    bool isRevealed() const { return revealed; }
    void reveal() { revealed = true; }
    
    // boost effect management
    bool isBoosted() const { return boosted; }
    void boost() { boosted = true; }
    void removeBoost() { boosted = false; }
    
    // jam effect management
    bool isJammed() const { return jammed; }
    void jam(int turnNumber) { jammed = true; jammedOnTurn = turnNumber; }
    void unjam() { jammed = false; jammedOnTurn = -1; }
    int getJammedOnTurn() const { return jammedOnTurn; }

    // battle system
    Link* battle(Link* opponent);
};

#endif
