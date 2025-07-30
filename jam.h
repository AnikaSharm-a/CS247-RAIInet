#ifndef JAM_H
#define JAM_H

#include "ability.h"

// ability that prevents an opponent's link from moving for 2 turns
class Jam : public Ability {
public:
    Jam();
    ~Jam() override = default;

    // requires a target opponent link ID
    void use(Game* game, Player* player, int row, int col) override;
};

#endif
