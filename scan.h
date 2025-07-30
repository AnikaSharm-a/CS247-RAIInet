#ifndef SCAN_H
#define SCAN_H

#include "ability.h"

// ability that reveals an opponent's link, showing its type and strength
class Scan : public Ability {
public:
    Scan();
    ~Scan() override = default;

    // reveal opponent's link at (row,col) (show type & strength)
    void use(Game* game,
             Player* player,
             int row,
             int col) override;
};

#endif // SCAN_H
