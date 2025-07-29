#include "fog.h"
#include "game.h"
#include "board.h"
#include <algorithm> // for std::max, std::min
#include <iostream>

Fog::Fog() : Ability("Fog") {}

void Fog::use(Game* game, Player* player, int row, int col) {
    if (!game) throw std::invalid_argument("Game pointer is null");

    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        throw std::out_of_range("Fog target coordinates out of bounds");
    }
    
    // Delegate fog application to Game, which tracks original cell types for restoration
    game->applyFogEffect(row, col, player->getId());

    std::cout << "Fog ability used by player " << player->getId() 
              << " at (" << row << ", " << col << ").\n";
}
