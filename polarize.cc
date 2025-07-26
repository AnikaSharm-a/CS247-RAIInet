#include "polarize.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <stdexcept>
using namespace std;

Polarize::Polarize() : Ability("Polarize"){}

void Polarize::use(Game* game,
                   Player* player,
                   int row,
                   int col)
{
    auto& board = game->getBoard();
    Cell* cell = board.getCell(row, col);
    if (!cell || cell->isEmpty())
        throw invalid_argument("Polarize: no link at target cell");

    Link* link = cell->getLink();
    // only on opponent’s link
    if (link->getOwner() == player)
        throw invalid_argument("Polarize: can only polarize opponent’s link");

    // flip type, leave strength alone
    link->setType(link->getType() == LinkType::Data ? LinkType::Virus : LinkType::Data);
}
