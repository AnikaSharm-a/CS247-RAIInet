#ifndef LINKBOOST_H
#define LINKBOOST_H

#include "ability.h"

class Link;

class LinkBoost : public Ability {
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
