#ifndef VIEW_H
#define VIEW_H

#include "cellType.h" 
#include <iostream>

class View {
public:
    virtual void notify(int row, int col, CellType state) = 0;

    virtual void print(std::ostream& out) const = 0;

    virtual ~View() = default;
};

#endif 