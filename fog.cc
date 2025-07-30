#include "fog.h"
#include "game.h"
#include "board.h"
#include <algorithm>
#include <iostream>
using namespace std;

Fog::Fog() : Ability("Fog") {}

void Fog::use(Game* game, Player* player, int row, int col) {
    if (!game) throw invalid_argument("Game pointer is null");

    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        throw out_of_range("Fog target coordinates out of bounds");
    }
    
    // redirects to Game, which tracks original cell types for restoration
    // we do this since Game has access to board, which is needed for fog effect
    game->applyFogEffect(row, col, player->getId());

    cout << "Fog ability used by player " << player->getId() 
              << " at (" << row << ", " << col << ").\n";
}
