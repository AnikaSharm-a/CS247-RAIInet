#ifndef ABILITY_H
#define ABILITY_H

#include <string>
using namespace std;

class Game;
class Player;

class Ability {
protected:
    string name;

public:
    explicit Ability(const string& name);
    virtual ~Ability();

    /// Apply this ability.  You must pass in your Game context so that
    /// downloads, reveals, board updates, etc. can be routed back into Game.
    virtual void use(Game* game,
                     Player* player,
                     int row,
                     int col) = 0;

    /// Human‐readable name (e.g. “LinkBoost”, “Firewall”, …)
    const string& getName() const;
};

#endif // ABILITY_H
