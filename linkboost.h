#ifndef LINKBOOST_H
#define LINKBOOST_H

#include "ability.h"
#include <set>
using namespace std;

class Link;

class LinkBoost : public Ability {
    set<Link*> boostedLinks;

public:
    LinkBoost();
    ~LinkBoost() override = default;

    /// row/col should locate *your* link on the board.
    void use(Game* game,
             Player* player,
             int row,
             int col) override;
};

#endif
