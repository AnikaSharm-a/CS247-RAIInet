#include "board.h"
#include "controller.h"
#include <stdexcept>
#include <iostream>
using namespace std;

Board::Board() : controller(nullptr) {
    // All cells default to Normal unless setup otherwise
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            CellType type = CellType::Normal;
            int ownerId = -1;
            if ((r == 0 || r == 7) && (c == 3 || c == 4)) {
                type = CellType::ServerPort;
                ownerId = (r == 0 ? 1 : 2);
            }
            grid[r][c] = Cell(type, ownerId);
        }
    }
}

pair<int, int> Board::findLinkPosition(char id, Player* player) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            auto* link = grid[r][c].getLink();
            if (link && link->getId() == id) {
                // Check ownership
                if (link->getOwner() != player) {
                    cout << "You cannot move link '" << id
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

MoveOutcome Board::moveLink(char id, Player* player, Direction dir) {
    MoveOutcome outcome;
    auto pos = findLinkPosition(id, player);
    if (pos.first == -1) {
        outcome.success = false;
        outcome.result = MoveResult::Invalid;
        return outcome;
    }
    int r = pos.first;
    int c = pos.second;
    int dr = 0, dc = 0;
    switch (dir) {
        case Direction::Up: dr = -1; break;
        case Direction::Down: dr = 1; break;
        case Direction::Left: dc = -1; break;
        case Direction::Right: dc = 1; break;
    }
    
    Link* moving = grid[r][c].getLink();
    if (!moving) {
        outcome.success = false;
        outcome.result = MoveResult::Invalid;
        return outcome;
    }
    
    // Check if the link is jammed
    if (moving->isJammed()) {
        outcome.success = false;
        outcome.result = MoveResult::Jammed;
        return outcome;
    }
    
    // Determine movement distance based on whether link is boosted
    int moveDistance = moving->isBoosted() ? 2 : 1;
    
    // Calculate destination coordinates
    int nr = r + (dr * moveDistance);
    int nc = c + (dc * moveDistance);
    
    // Check if the move would go out of bounds
    if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) {
        // Allow moving off the opponent's side (download)
        // Conditions:
        // - Player 1 moves upwards out of row 0
        // - Player 2 moves downwards out of row 7
        bool offOpponentSide =
            (player->getId() == 1 && nr >= 8) ||
            (player->getId() == 2 && nr < 0);

        if (offOpponentSide) {
            outcome.movedLink = moving;
            outcome.affectedLink = moving;
            outcome.affectedLink->reveal();
            grid[r][c].removeLink(); // Remove from board
            
            // Notify about cell change and link movement
            if (controller) {
                controller->notifyCellChanged(r, c);
                // controller->notifyLinkMoved(id);
            }
            
            outcome.success = true;
            outcome.result = MoveResult::DownloadedOffBoard;
            return outcome;
        }

        // Any other out-of-bounds is invalid
        outcome.success = false;
        outcome.result = MoveResult::Invalid;
        return outcome;
    }

    Cell& src = grid[r][c];
    Cell& dest = grid[nr][nc];

    outcome.movedLink = moving;
    outcome.destRow = nr;
    outcome.destCol = nc;

    // Check if destination has a firewall and apply firewall effects
    if (dest.getType() == CellType::Firewall) {
        // If the moving link is owned by the firewall owner, no effect
        if (dest.getOwnerId() == player->getId()) {
            // No firewall effect for owner's links
        } else {
            // Reveal the opponent link
            moving->reveal();
            if (controller) {
                controller->notifyLinkRevealed(id);
            }
            
            // If it's a virus, download it immediately
            if (moving->getType() == LinkType::Virus) {
                // Find the virus owner and download it
                Player* virusOwner = moving->getOwner();
                if (virusOwner) {
                    // Remove the link from the board
                    src.removeLink();
                    
                    // Notify about cell change and link movement
                    if (controller) {
                        controller->notifyCellChanged(r, c);
                        // controller->notifyLinkMoved(id);
                    }
                    
                    outcome.success = true;
                    outcome.result = MoveResult::DownloadedByFirewall;
                    outcome.affectedLink = moving;
                    outcome.movedLink = moving;
                    return outcome;
                }
            }
        }
    }

    if (!dest.isEmpty()) {
        if (dest.getLink()->getOwner() == player) {
            outcome.success = false;
            outcome.result = MoveResult::Invalid;
            return outcome;
        }

        Link* defender = dest.getLink();
        
        // Check if defender is on a firewall and apply firewall effects before battle
        if (dest.getType() == CellType::Firewall) {
            // If the attacker is owned by the firewall owner, no effect
            if (dest.getOwnerId() == player->getId()) {
                // No firewall effect for owner's links
            } else {
                // Reveal the attacker link
                moving->reveal();
                if (controller) {
                    controller->notifyLinkRevealed(id);
                }
                
                // If attacker is a virus, download it immediately
                if (moving->getType() == LinkType::Virus) {
                    // Remove the attacker from the board
                    src.removeLink();
                    
                    // Notify about cell change and link movement
                    if (controller) {
                        controller->notifyCellChanged(r, c);
                        // controller->notifyLinkMoved(id);
                    }
                    
                    outcome.success = true;
                    outcome.result = MoveResult::DownloadedByFirewall;
                    outcome.affectedLink = moving;
                    outcome.movedLink = moving;
                    return outcome;
                }
            }
        }
        
        Link* winner = moving->battle(defender);

        if (winner == moving) {
            // Moving link wins battle, replaces defender
            dest.setLink(moving);
            src.removeLink();

            // Notify about cell changes and link movement
            if (controller) {
                controller->notifyCellChanged(r, c);
                controller->notifyCellChanged(nr, nc);
                // controller->notifyLinkMoved(id);
            }

            outcome.success = true;
            outcome.result = MoveResult::BattleWon;
            outcome.affectedLink = defender;
            return outcome;
        } else {
            // Defender wins, attacker is removed
            src.removeLink();

            // Notify about cell change and link movement
            if (controller) {
                controller->notifyCellChanged(r, c);
                // controller->notifyLinkMoved(id);
            }

            outcome.success = true;
            outcome.result = MoveResult::BattleLost;
            outcome.affectedLink = moving;
            return outcome;
        }
    } else {

        // Prevent moving into own server port
        if (dest.getType() == CellType::ServerPort && dest.getOwnerId() == player->getId()) {
            outcome.success = false;
            outcome.result = MoveResult::Invalid;
            return outcome;
        }

        // Check for opponent's server port BEFORE placing the link
        if (dest.getType() == CellType::ServerPort && dest.getOwnerId() != player->getId()) {
            outcome.affectedLink = moving;
            outcome.affectedLink->reveal();
            src.removeLink();        // remove from source
            dest.removeLink();       // ensure dest is empty
            
            // Notify about cell changes and link movement
            if (controller) {
                controller->notifyCellChanged(r, c);
                controller->notifyCellChanged(nr, nc); // TODO: check if this is correct
                // controller->notifyLinkMoved(id);
            }
            
            outcome.success = true;
            outcome.result = MoveResult::DownloadedOnServerPort;
            return outcome;
        }

        // Empty destination cell
        dest.setLink(moving);
        src.removeLink();

        // Notify about cell changes and link movement
        if (controller) {
            controller->notifyCellChanged(r, c);
            controller->notifyCellChanged(nr, nc);
            // controller->notifyLinkMoved(id);
        }

        outcome.success = true;
        outcome.result = MoveResult::Moved;
        return outcome;
    }
}

bool Board::isServerPort(int row, int col) const {
    return (row == 0 || row == 7) && (col == 3 || col == 4);
}

void Board::addFirewall(int row, int col, Player* player) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        throw invalid_argument("Firewall position out of bounds");
    }
    
    // Set the cell type to Firewall and store the player ID
    grid[row][col] = Cell(CellType::Firewall, player->getId());
    
    // Notify about cell change
    if (controller) {
        controller->notifyCellChanged(row, col);
    }
}
