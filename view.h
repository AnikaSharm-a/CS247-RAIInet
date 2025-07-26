#ifndef VIEW_H
#define VIEW_H

#include "cellType.h" 
#include <iostream>
class Game;  // forward declaration of Game

class View {
public:
    // virtual void notify(int row, int col, CellType state) = 0;

    virtual void print(const Game &game, std::ostream &out) const = 0;

    virtual ~View() = default;
};

#endif 