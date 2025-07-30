#ifndef AREASCAN_H
#define AREASCAN_H

#include "ability.h"

class AreaScan : public Ability {
public:
    AreaScan();
    ~AreaScan() override = default;

    /// Requires coordinates (row, col) to specify the center of the scan area
    void use(Game* game,
             Player* player,
             int row,
             int col) override;
};

#endif
