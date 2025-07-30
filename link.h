#ifndef LINK_H
#define LINK_H

#include "linkType.h"
#include "player.h"

class Player; // forward declaration

// represents a link (data or virus) that can move and battle on the board
class Link {
    char id;                    // unique identifier for this link
    LinkType type;             // whether this is data or virus
    int strength;              // base strength (1-4)
    Player* owner;             // player who owns this link
    bool revealed = false;     // whether this link's type is visible to opponents
    bool boosted = false;      // whether this link has a movement boost
    bool jammed = false;       // whether this link is currently jammed
    int jammedOnTurn = -1;     // track which turn the jam was applied

public:
    Link(char id, LinkType type, int strength, Player* owner)
        : id(id), type(type), strength(strength), owner(owner) {}

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

#endif // LINK_H
