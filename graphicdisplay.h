#ifndef GRAPHICDISPLAY_H
#define GRAPHICDISPLAY_H

#include "view.h"
#include "window.h"
#include "cellType.h"
#include "linkType.h"
#include <vector>
using namespace std;

class Game;

class GraphicDisplay : public View {
    mutable Xwindow xw;
    int gridSize;
    int cellWidth;
    int cellHeight;
    int margin = 4;
    Game* gameRef;  // reference to the game for redrawing

    // keep a snapshot of last drawn characters to allow partial redraw
    struct DrawnState {
        char symbol;
        bool boosted;
        bool visible;
        LinkType type;
        bool jammed;
        bool fogged;
    };

    // storing previous states
    vector<vector<DrawnState>> lastDrawn;
    mutable string lastP1Info;
    mutable string lastP2Info;

    void drawCell(int r, int c, const class Cell &cell, const class Player *p1) const;

public:
    GraphicDisplay(int gridSize, int width = 500, int height = 500);

    // observer pattern notification method
    void notify(const NotificationData& data) override;

    // set the game reference
    void setGameRef(Game* game) { gameRef = game; }

    void print(const Game &game, ostream &out) const override;
};

#endif
