#ifndef SCAN_H
#define SCAN_H

#include "ability.h"

class Scan : public Ability {
public:
    Scan();
    ~Scan() override = default;

    /// Reveal opponent’s link at (row,col) (show type & strength)
    void use(Game* game,
             Player* player,
             int row,
             int col) override;
};

#endif // SCAN_H
