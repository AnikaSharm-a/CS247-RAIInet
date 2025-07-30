#include "polarize.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <stdexcept>
using namespace std;

//name
Polarize::Polarize() : Ability("Polarize"){}

//change the type of link from data to virus and vice versa
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

    //flip type
    link->setType(link->getType() == LinkType::Data ? LinkType::Virus : LinkType::Data);
}
