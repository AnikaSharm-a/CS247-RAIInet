#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>

class View;
class Game;

class Controller {
    View* view;
    Game* game;

    // Returns false if quit or game over
    bool parseCommand(const std::string &cmd, std::istream &in);

public:
    Controller(View* view, Game* game);
    void play(std::istream &in);
};

#endif
