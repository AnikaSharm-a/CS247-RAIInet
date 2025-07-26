#ifndef POLARIZE_H
#define POLARIZE_H

#include "ability.h"

class Polarize : public Ability {
public:
    Polarize();
    ~Polarize() override = default;

    /// Flip opponent’s data↔virus at (row,col)
    void use(Game* game,
             Player* player,
             int row,
             int col) override;
};

#endif // POLARIZE_H
