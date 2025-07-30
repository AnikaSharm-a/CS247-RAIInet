#ifndef FOG_H
#define FOG_H

#include "ability.h"

// ability that applies fog to hide board contents in a 3x3 area
class Fog : public Ability {
public:
    Fog();
    ~Fog() override = default;

    // apply fog by setting cells in a 3x3 around (row, col) to type Fog
    void use(Game* game, Player* player, int row, int col) override;
};

#endif
