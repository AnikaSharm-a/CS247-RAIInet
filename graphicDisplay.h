#ifndef GRAPHICDISPLAY_H
#define GRAPHICDISPLAY_H

#include "view.h"
#include "window.h"       // Assuming Xwindow is defined here
#include "cellType.h"
#include <iostream>

class GraphicDisplay : public View {
    Xwindow xw;
    int gridSize;
    int cellWidth;
    int cellHeight;

public:
    // Constructor
    GraphicDisplay(int gridSize, int width = 500, int height = 500);

    // Implements notify to update the graphical cell
    void notify(int row, int col, CellType state) override;

    // Implements print (possibly empty or just for debugging/logging)
    void print(std::ostream& out) const override;
};

#endif // GRAPHICDISPLAY_H
