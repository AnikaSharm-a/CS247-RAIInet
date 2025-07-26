#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"
#include <set>
#include <utility>
using namespace std;

class FireWall : public Ability {
    set<pair<int,int>> firewallPositions;

public:
    FireWall();
    ~FireWall() override = default;

    /// row/col must be empty and not a server port.
    void use(Game* game,
             Player* player,
             int row,
             int col) override;

    const set<pair<int,int>>& getFirewallPositions() const;
};

#endif // FIREWALL_H
