#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <vector>
#include <ostream>
#include "view.h"
using namespace std;

class Game; // forward declaration

class TextDisplay : public View {
    vector<vector<char>> theDisplay;
    Game* gameRef;  // Reference to the game for redrawing
    bool hasRedrawnThisTurn;  // Flag to prevent multiple redraws per turn

public:
    TextDisplay(int gridSize);

    // Observer pattern notification method
    void notify(const NotificationData& data) override;

    // Set the game reference
    void setGameRef(Game* game) { gameRef = game; }

    // Updates a single cell character (from Board notifications)
    // void notify(int row, int col, char symbol) override;

    // Prints the entire game state: both players + board
    void print(const Game& game, ostream &out) const override;
};

#endif // TEXTDISPLAY_H
