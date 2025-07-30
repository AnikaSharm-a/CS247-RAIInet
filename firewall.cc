#include "firewall.h"
#include "game.h"
#include "board.h"
#include "cell.h"

#include <stdexcept>
using namespace std;

FireWall::FireWall() : Ability("FireWall") {}

void FireWall::use(Game* game, Player* player,int row, int col){
    Board* board = game->getBoard();
    if (board->isServerPort(row, col))
        throw invalid_argument("FireWall: cannot place on server port");

    Cell& cell = board->at(row, col);
    if (!cell.isEmpty()){
        throw invalid_argument("FireWall: target cell not empty");
    }

    //Track thee firewall position
    firewallPositions.emplace(row, col);
    board->addFirewall(row, col, player);
}

const set<pair<int,int>>& FireWall::getFirewallPositions() const {
    return firewallPositions;
}
