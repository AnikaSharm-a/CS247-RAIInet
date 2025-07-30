#include "scan.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <stdexcept>
using namespace std;

//name
Scan::Scan() : Ability("Scan") {}

//reveal opponent link
void Scan::use(Game* game,
               Player* player,
               int row,
               int col)
{
    Board* board = game->getBoard();
    Cell& cell = board->at(row, col);
    if (cell.isEmpty())
        throw invalid_argument("Scan: no link at target cell");

    auto link = cell.getLink();
    //scan any link on the board
    //no effect on our own links
    //reveal opponent links
    
    if (link->getOwner() != player) {
        //reveal only opponent links
        link->reveal();
    }
    //error handling
    else{
        throw invalid_argument("Scan: cannot scan own link");
    }
}
