#include "linkboost.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <stdexcept>
using namespace std;

// constructor sets the ability name
LinkBoost::LinkBoost()
  : Ability("LinkBoost")
{}

// applies a movement boost to the specified link
void LinkBoost::use(Game* game,
                    Player* player,
                    int row,
                    int col)
{
    Board* board = game->getBoard();
    Cell& cell = board->at(row, col);
    if (cell.isEmpty())
        throw invalid_argument("LinkBoost: no link at target cell");

    auto link = cell.getLink();
    if (link->getOwner() != player)
        throw invalid_argument("LinkBoost: can only boost your own link");

    // mark it boosted so when it moves it can jump one extra square
    link->boost();
}
