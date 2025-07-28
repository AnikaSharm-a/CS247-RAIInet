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
    // Allow scanning any link on the field
    // For own links, we can scan them but they won't be revealed
    // For opponent links, reveal them
    
    if (link->getOwner() != player) {
        // Reveal opponent links
        link->reveal();
    }
    // For own links, do nothing (they remain unrevealed)
    else{
        throw invalid_argument("Scan: cannot scan own link");
    }
}
