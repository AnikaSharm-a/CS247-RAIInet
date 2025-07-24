#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>
#include "view.h"
#include "game.h"

class Controller {
    View* view;
    Game* game;

public:
    Controller(View* view, Game* game);
    void play(std::istream& in);  // main loop
    void parseCommand(std::istream& in);
};

#endif
