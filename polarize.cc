#include "polarize.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <stdexcept>
using namespace std;

// constructor sets the ability name
Polarize::Polarize() : Ability("Polarize"){}

// flips the type of a link from data to virus or vice versa
void Polarize::use(Game* game,
                   Player* player,
                   int row,
                   int col)
{
    Board* board = game->getBoard();
    Cell& cell = board->at(row, col);
    if (cell.isEmpty())
        throw invalid_argument("Polarize: no link at target cell");

    auto link = cell.getLink();

    // flip type, leave strength alone
    link->setType(link->getType() == LinkType::Data ? LinkType::Virus : LinkType::Data);
}
