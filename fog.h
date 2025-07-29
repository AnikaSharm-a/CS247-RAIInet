#ifndef FOG_H
#define FOG_H

#include "ability.h"

class Fog : public Ability {
public:
    Fog();  // Constructor sets name = "Fog"
    ~Fog() override = default;

    // Apply fog by setting cells in a 3x3 around (row, col) to CellType::Fog
    void use(Game* game, Player* player, int row, int col) override;
};

#endif // FOG_H
