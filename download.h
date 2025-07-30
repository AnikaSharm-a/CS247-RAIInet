#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "ability.h"

class Download : public Ability {
public:
    Download();
    ~Download() override = default;

    // immediately download the opponent's link at (row,col)
    void use(Game* game, Player* player, int row, int col) override;
};

#endif
