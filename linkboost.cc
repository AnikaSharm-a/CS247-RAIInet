#include "linkboost.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <stdexcept>
using namespace std;

LinkBoost::LinkBoost()
  : Ability("LinkBoost")
{}

void LinkBoost::use(Game* game,
                    Player* player,
                    int row,
                    int col)
{
    Board* board = game->getBoard();
    Cell& cell = board->at(row, col);
    if (cell.isEmpty())
        throw invalid_argument("LinkBoost: no link at target cell");

    Link* link = cell.getLink();
    if (link->getOwner() != player)
        throw invalid_argument("LinkBoost: can only boost your own link");

    // Mark it boosted so when it moves it can jump one extra square
    link->boost(true);
    boostedLinks.insert(link);
}
