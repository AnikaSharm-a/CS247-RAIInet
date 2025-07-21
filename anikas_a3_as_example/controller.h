#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <iostream>
#include "game.h"
#include "view.h"

// More documentation in controller.cc file

class Controller : public GameNotification {

    View *td; // The text display.
    View *gd; // graphic display (optional)
    Game *game;
    bool useGraphics;
    void init(std::istream & input, Game & g);

	///Helper to check if the game is in a win state
    //if yes, prints Won to stdout and returns true
    //if no, returns false
     bool checkWin(int movesLeft);

  public:
    Controller(bool useGraphics = false);
    ~Controller();
    void play();
    void notify(int r, int c, int state);
};

#endif
