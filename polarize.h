#ifndef POLARIZE_H
#define POLARIZE_H

#include "ability.h"

//change a link from data to virus and the same vice versa
class Polarize : public Ability {
public:
    Polarize();
    ~Polarize() override = default;

    //flip data and virus
    void use(Game* game,
             Player* player,
             int row,
             int col) override;
};

#endif
