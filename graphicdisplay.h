#ifndef GRAPHICDISPLAY_H
#define GRAPHICDISPLAY_H

#include "view.h"
#include "window.h"
#include "cellType.h"
#include <vector>

class GraphicDisplay : public View {
    mutable Xwindow xw;
    int gridSize;
    int cellWidth;
    int cellHeight;

    // Keep a snapshot of last drawn characters to allow partial redraw
    std::vector<std::vector<char>> lastDrawn;

    // --- ADD THESE CACHES ---
    mutable std::string lastP1Info;
    mutable std::string lastP2Info;

    void drawCell(int r, int c, const class Cell &cell, const class Player *p1) const;

public:
    GraphicDisplay(int gridSize, int width = 500, int height = 500);

    // Unused in this approach, but left for completeness
    // void notify(int row, int col, CellType state) override;

    void print(const Game &game, std::ostream &out) const override;
};

#endif
