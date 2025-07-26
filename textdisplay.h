#pragma once
#include <vector>
#include <ostream>
#include "view.h"

class Game; // forward declaration

class TextDisplay : public View {
    std::vector<std::vector<char>> theDisplay;

public:
    TextDisplay(int gridSize);

    // Updates a single cell character (from Board notifications)
    // void notify(int row, int col, char symbol) override;

    // Prints the entire game state: both players + board
    void print(const Game& game, std::ostream &out) const override;
};
