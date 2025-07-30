#ifndef AREASCAN_H
#define AREASCAN_H

#include "ability.h"

// ability that reveals all opponent links in a 3x3 area around a player's link
// inspired by "Scan"
class AreaScan : public Ability {
public:
    AreaScan();
    ~AreaScan() override = default;

    // requires coordinates (row, col) to specify the center of the scan area
    void use(Game* game, Player* player, int row, int col) override;
};

#endif
