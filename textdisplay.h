#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <vector>
#include <ostream>
#include "view.h"
using namespace std;

class Game;

class TextDisplay : public View {
    vector<vector<char>> theDisplay;
    Game* gameRef;//reference to game
    bool hasRedrawnThisTurn;//prevent redraws

public:
    TextDisplay(int gridSize);

    //observer pattern notification method
    void notify(const NotificationData& data) override;

    //set game ref
    void setGameRef(Game* game) { gameRef = game; }

    //prints the entire game state: both players + board
    void print(const Game& game, ostream &out) const override;
};

#endif
