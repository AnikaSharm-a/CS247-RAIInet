#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "view.h"
#include "cellType.h"
#include <vector>
#include <iostream>

class TextDisplay : public View {
    std::vector<std::vector<int>> theDisplay;

public:
    TextDisplay(int gridSize);

    void notify(int row, int col, CellType state) override;

    void print(std::ostream& out) const override;
};

#endif 