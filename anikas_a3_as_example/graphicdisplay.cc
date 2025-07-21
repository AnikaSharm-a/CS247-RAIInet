#include "graphicdisplay.h"

GraphicDisplay::GraphicDisplay(int n) : gridSize{n} {
    // compute the cell dimensions based on window size (500x500)
    cellWidth = xw.getWidth() / gridSize;
    cellHeight = xw.getHeight() / gridSize;

    // initialize the grid to all white (looks like a blank window at first)
    for (int r = 0; r < gridSize; ++r) {
        for (int c = 0; c < gridSize; ++c) {
            xw.fillRectangle(c * cellWidth, r * cellHeight, cellWidth, cellHeight, Xwindow::White);
        }
    }
}

void GraphicDisplay::notify(int r, int c, int change) {
    int color = Xwindow::White;
    if (change >= 0 && change <= 4) { // if colour change is happening
        color = change;
    }

    xw.fillRectangle(c * cellWidth, r * cellHeight, cellWidth, cellHeight, color);
}

void GraphicDisplay::print(std::ostream &out) {
    // graphical display does not need to print text
}
