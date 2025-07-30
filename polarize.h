#ifndef POLARIZE_H
#define POLARIZE_H

#include "ability.h"

// ability that changes a link's type from data to virus or vice versa
class Polarize : public Ability {
public:
    Polarize();
    ~Polarize() override = default;

    // flip opponent's data↔virus at (row,col)
    void use(Game* game,
             Player* player,
             int row,
             int col) override;
};

#endif // POLARIZE_H
