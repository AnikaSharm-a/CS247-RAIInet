#include "areascan.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <stdexcept>
using namespace std;

AreaScan::AreaScan(): Ability("AreaScan") {}

// scans a 3x3 area around the specified position to reveal opponent links
void AreaScan::use(Game* game, Player* player, int row, int col){
    Board* board = game->getBoard();
    
    // make sure that coordinates are within board bounds
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        throw invalid_argument("AreaScan: coordinates out of bounds");
    }
    
    //make sure that the cell actually has a link
    Cell& cell = board->at(row, col);
    if (cell.isEmpty()) {
        throw invalid_argument("AreaScan: no link at specified position");
    }
    
    // make sure that the link is owned by the current player
    auto link = cell.getLink();
    if (link->getOwner() != player) {
        throw invalid_argument("AreaScan: can only scan around your own link");
    }
    
    // scan the 8 surrounding squares plus the center square (3x3 area)
    int revealedCount = 0;
    for (int r = max(0, row - 1); r <= min(7, row + 1); r++) {
        for (int c = max(0, col - 1); c <= min(7, col + 1); c++) {
            Cell& scanCell = board->at(r, c);
            if (!scanCell.isEmpty()) {
                auto scanLink = scanCell.getLink();
                //we only reveal opponent links only
                if (scanLink->getOwner() != player) {
                    scanLink->reveal();
                    revealedCount++;
                }
            }
        }
    }
}
