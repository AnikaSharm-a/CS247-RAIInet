#include "board.h"
#include "controller.h"
#include <stdexcept>
#include <iostream>
using namespace std;

//constructor initializes the board with default cells and server ports.
Board::Board() : controller(nullptr) {
    // all cells are Normal unless we specified otherwise in setup
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            CellType type = CellType::Normal;
            int ownerId = -1;
            if ((r == 0 || r == 7) && (c == 3 || c == 4)) {
                type = CellType::ServerPort;
                ownerId = (r == 0 ? 1 : 2); //player 1's server port is on top, player 2's is on bottom
            }
            grid[r][c] = Cell(type, ownerId);
        }
    }
}

// finds the position of a link on the board and validates ownership
// helper for Link moving
pair<int, int> Board::findLinkPosition(char id, Player* player) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            auto link = grid[r][c].getLink();
            if (link && link->getId() == id) {
                // make sure that the link is owned by the current player
                if (link->getOwner() != player) {
                    cout << "You cannot use link '" << id
                              << " - it belongs to the other player." << endl;
                    return {-1, -1};
                }
                return {r, c};
            }
        }
    }

    cout << "Link '" << id << "' not found on the board, "
                 "or it is not your link." << endl;
    return {-1, -1};
}

//move a link in the specified direction and returns the outcome
MoveOutcome Board::moveLink(char id, Player* player, Direction dir) {
    MoveOutcome outcome;
    auto pos = findLinkPosition(id, player);
    if (pos.first == -1) { //link not found
        outcome.success = false;
        outcome.result = MoveResult::Invalid;
        return outcome;
    }
    int r = pos.first, c = pos.second;
    int dr = 0, dc = 0;
    switch (dir) {
        case Direction::Up: dr = -1; break;
        case Direction::Down: dr = 1; break;
        case Direction::Left: dc = -1; break;
        case Direction::Right: dc = 1; break;
    }
    
    // get shared_ptr to the moving link
    auto movingLink = grid[r][c].getLink();
    if (!movingLink) {
        outcome.success = false;
        outcome.result = MoveResult::Invalid;
        return outcome;
    }
    
    Link* moving = movingLink.get();
    
    // check if the link is jammed, can't move if it is
    if (moving->isJammed()) {
        outcome.success = false;
        outcome.result = MoveResult::Jammed;
        return outcome;
    }
    
    int moveDistance = moving->isBoosted() ? 2 : 1; //boosted links move 2 spaces, normal links move 1 space
    
    // calculate destination coordinates
    int nr = r + (dr * moveDistance);
    int nc = c + (dc * moveDistance);
    
    // check if the move would go out of bounds
    if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) {
        // allowed to move off the opponent's side.
        bool offOpponentSide = (player->getId() == 1 && nr >= 8) ||(player->getId() == 2 && nr < 0);
        
        // if the link is moved off the opponent's side, it is downloaded
        if (offOpponentSide) {
            outcome.movedLink = movingLink;
            outcome.affectedLink = movingLink;
            outcome.affectedLink->reveal();
            grid[r][c].removeLink(); // remove from board
            
            // notify about cell change and link movement
            controller->notifyCellChanged(r, c);
            
            outcome.success = true;
            outcome.result = MoveResult::DownloadedOffBoard;
            return outcome;
        }

        // any other out-of-bounds is invalid
        outcome.success = false;
        outcome.result = MoveResult::Invalid;
        return outcome;
    }

    Cell& src = grid[r][c];
    Cell& dest = grid[nr][nc];

    outcome.movedLink = movingLink;
    outcome.destRow = nr;
    outcome.destCol = nc;

    // check if destination has a firewall and apply firewall effects
    if (dest.getType() == CellType::Firewall) {
        // firewall only has effect on opponent's links
        if (dest.getOwnerId() != player->getId()) {
            moving->reveal();
            
            // if it's a virus, download it immediately before battle
            if (moving->getType() == LinkType::Virus) {
                // remove the link from the board
                src.removeLink();
                
                // notify about cell change
                controller->notifyCellChanged(r, c);
                
                outcome.success = true;
                outcome.result = MoveResult::DownloadedByFirewall;
                outcome.affectedLink = movingLink;
                outcome.movedLink = movingLink;
                return outcome;
            }
        }
    }

    if (!dest.isEmpty()) {
        auto destLink = dest.getLink();
        // can't move on top of your own link
        if (destLink->getOwner() == player) {
            outcome.success = false;
            outcome.result = MoveResult::Invalid;
            return outcome;
        }

        auto defenderLink = dest.getLink();
        Link* defender = defenderLink.get();
        
        // check if defender is on a firewall and apply firewall effects before battle
        if (dest.getType() == CellType::Firewall) {
            if (dest.getOwnerId() != player->getId()) {
                moving->reveal();
                controller->notifyLinkRevealed(id);
                
                // if attacker is a virus, download it immediately
                if (moving->getType() == LinkType::Virus) {
                    src.removeLink();
                    controller->notifyCellChanged(r, c);
                    
                    outcome.success = true;
                    outcome.result = MoveResult::DownloadedByFirewall;
                    outcome.affectedLink = movingLink;
                    outcome.movedLink = movingLink;
                    return outcome;
                }
            }
        }
        
        Link* winner = moving->battle(defender);

        if (winner == moving) {
            // moving link wins battle, replaces defender
            dest.setLink(movingLink);
            src.removeLink();

            controller->notifyCellChanged(r, c);
            controller->notifyCellChanged(nr, nc);

            outcome.success = true;
            outcome.result = MoveResult::BattleWon;
            outcome.affectedLink = defenderLink;
            return outcome;
        } else {
            // defender wins, attacker is removed
            src.removeLink();

            controller->notifyCellChanged(r, c);

            outcome.success = true;
            outcome.result = MoveResult::BattleLost;
            outcome.affectedLink = movingLink;
            return outcome;
        }
    } else {
        // can't move into own server port
        if (dest.getType() == CellType::ServerPort && dest.getOwnerId() == player->getId()) {
            outcome.success = false;
            outcome.result = MoveResult::Invalid;
            return outcome;
        }

        // check for opponent's server port BEFORE placing the link
        if (dest.getType() == CellType::ServerPort && dest.getOwnerId() != player->getId()) {
            outcome.affectedLink = movingLink;
            outcome.affectedLink->reveal();
            src.removeLink();//remove from source
            dest.removeLink();
            
            controller->notifyCellChanged(r, c);
            controller->notifyCellChanged(nr, nc);
            
            outcome.success = true;
            outcome.result = MoveResult::DownloadedOnServerPort;
            return outcome;
        }

        dest.setLink(movingLink);
        src.removeLink();

        controller->notifyCellChanged(r, c);
        controller->notifyCellChanged(nr, nc);

        outcome.success = true;
        outcome.result = MoveResult::Moved;
        return outcome;
    }
}

//helper function check if the specified position is a server port
bool Board::isServerPort(int row, int col) const {
    return (row == 0 || row == 7) && (col == 3 || col == 4);
}

// adds a firewall at the specified position
void Board::addFirewall(int row, int col, Player* player) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        throw invalid_argument("Firewall position out of bounds");
    }
    
    // set the cell type to Firewall and store the player ID
    grid[row][col] = Cell(CellType::Firewall, player->getId());
    
    controller->notifyCellChanged(row, col);
}
