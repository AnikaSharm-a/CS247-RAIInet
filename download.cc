#include "download.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <stdexcept>
using namespace std;

Download::Download() : Ability("Download") {}

// immediately downloads the opponent's link at the specified position
void Download::use(Game* game, Player* player, int row, int col){
    Board* board = game->getBoard();
    Cell& cell = board->at(row, col);
    if (cell.isEmpty()){
        throw invalid_argument("Download: no link at target cell");
    }

    auto link = cell.getLink();
    if (link->getOwner() == player){
        throw invalid_argument("Download: cannot download your own link");
    }
    
    cell.removeLink();

    game->download(link, player);
}
