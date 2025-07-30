#include "scan.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <stdexcept>
using namespace std;

// constructor sets the ability name
Scan::Scan() : Ability("Scan") {}

// reveals an opponent's link, making its type and strength visible
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
    // allow scanning any link on the field
    // for own links, we can scan them but they won't be revealed
    // for opponent links, reveal them
    
    if (link->getOwner() != player) {
        // reveal opponent links
        link->reveal();
    }
    // for own links, do nothing (they remain unrevealed)
    else{
        throw invalid_argument("Scan: cannot scan own link");
    }
}
