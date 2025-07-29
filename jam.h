#ifndef JAM_H
#define JAM_H

#include "ability.h"

class Jam : public Ability {
public:
    Jam();
    ~Jam() override = default;

    /// Requires a target opponent link ID
    void use(Game* game,
             Player* player,
             int row,
             int col) override;
};

#endif