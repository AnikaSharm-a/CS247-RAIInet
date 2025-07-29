#ifndef GRAPHICDISPLAY_H
#define GRAPHICDISPLAY_H

#include "view.h"
#include "window.h"
#include "cellType.h"
#include "linkType.h"
#include <vector>
using namespace std;

class GraphicDisplay : public View {
    mutable Xwindow xw;
    int gridSize;
    int cellWidth;
    int cellHeight;
    int margin = 4;

    // Keep a snapshot of last drawn characters to allow partial redraw
    struct DrawnState {
        char symbol;
        bool boosted;
        bool visible;
        LinkType type; // only meaningful if visible
        bool jammed;
        bool fogged;
    };

    vector<vector<DrawnState>> lastDrawn;

    // --- ADD THESE CACHES ---
    mutable string lastP1Info;
    mutable string lastP2Info;

    void drawCell(int r, int c, const class Cell &cell, const class Player *p1) const;

public:
    GraphicDisplay(int gridSize, int width = 500, int height = 500);

    // Unused in this approach, but left for completeness
    // void notify(int row, int col, CellType state) override;

    void print(const Game &game, ostream &out) const override;
};

#endif
