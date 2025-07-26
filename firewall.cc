#include "firewall.h"
#include "game.h"
#include "board.h"
#include "cell.h"

#include <stdexcept>
using namespace std;

FireWall::FireWall()
  : Ability("FireWall")
{}

void FireWall::use(Game* game,
                   Player* player,
                   int row,
                   int col)
{
    auto& board = game->getBoard();
    if (board.isServerPort(row, col))
        throw invalid_argument("FireWall: cannot place on server port");

    Cell* cell = board.getCell(row, col);
    if (!cell || !cell->isEmpty())
        throw invalid_argument("FireWall: target cell not empty");

    firewallPositions.emplace(row, col);
    board.addFirewall(row, col, player);
}

const set<pair<int,int>>& FireWall::getFirewallPositions() const {
    return firewallPositions;
}
