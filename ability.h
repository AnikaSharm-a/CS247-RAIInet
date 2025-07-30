#ifndef ABILITY_H
#define ABILITY_H

#include <string>
using namespace std;

// forward declarations (avoid circular dependencies)
class Game;
class Player;

class Ability {
protected:
    string name;

public:
    explicit Ability(const string& name);
    virtual ~Ability() = default;

    // pure virtual function that all abilities must implement
    virtual void use(Game* game, Player* player, int row, int col) = 0;

    const string& getName() const;
};

#endif
