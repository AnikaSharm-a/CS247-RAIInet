#include "jam.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <iostream>
#include <stdexcept>
using namespace std;

Jam::Jam()
  : Ability("Jam")
{}

void Jam::use(Game* game,
              Player* player,
              int row,
              int col)
{
    Board* board = game->getBoard();
    Cell& cell = board->at(row, col);
    if (cell.isEmpty())
        throw invalid_argument("Jam: no link at target cell");

    auto link = cell.getLink();
    
    if (link->getOwner() == player)
        throw invalid_argument("Jam: can only jam opponent's link");

    // Mark the link as jammed with current turn number
    int currentTurn = game->getCurrentTurn();
    link->jam(currentTurn);
}
