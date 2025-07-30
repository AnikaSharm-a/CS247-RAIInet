#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"
#include <set>
#include <utility>
using namespace std;

// ability that places defensive firewalls on the board
class FireWall : public Ability {
    set<pair<int,int>> firewallPositions; // tracks positions of firewalls placed by this ability

public:
    FireWall();
    ~FireWall() override = default;

    void use(Game* game, Player* player, int row, int col) override;

    // returns the positions of firewalls placed by this ability
    const set<pair<int,int>>& getFirewallPositions() const;
};

#endif
