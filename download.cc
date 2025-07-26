#include "download.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "link.h"

#include <stdexcept>
using namespace std;

Download::Download()
  : Ability("Download")
{}

void Download::use(Game* game,
                   Player* player,
                   int row,
                   int col)
{
    auto& board = game->getBoard();
    Cell* cell = board.getCell(row, col);
    if (!cell || cell->isEmpty())
        throw invalid_argument("Download: no link at target cell");

    Link* link = cell->getLink();
    if (link->getOwner() == player)
        throw invalid_argument("Download: cannot download your own link");

    // delegate to Game’s download logic so counters, removal, reveal, etc. happen
    game->download(link, player);
}
