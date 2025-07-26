#include "scan.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <stdexcept>
using namespace std;

Scan::Scan() : Ability("Scan") {}

void Scan::use(Game* game,
               Player* player,
               int row,
               int col)
{
    Board* board = game->getBoard();
    Cell& cell = board->at(row, col);
    if (cell.isEmpty())
        throw invalid_argument("Scan: no link at target cell");

    Link* link = cell.getLink();
    if (link->getOwner() == player)
        throw invalid_argument("Scan: cannot scan your own link");

    // reveal the link so its type & strength become visible
    link->reveal();
}
