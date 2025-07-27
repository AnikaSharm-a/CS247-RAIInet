#include "board.h"
#include <stdexcept>
#include <iostream>

Board::Board() {
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

std::pair<int, int> Board::findLinkPosition(char id, Player* player) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            auto* link = grid[r][c].getLink();
            if (link && link->getId() == id) {
                // Check ownership
                if (link->getOwner() != player) {
                    std::cout << "You cannot move link '" << id
                              << " - it belongs to the other player." << std::endl;
                    return {-1, -1};
                }
                return {r, c};
            }
        }
    }

    std::cout << "Link '" << id << "' not found on the board, "
                 "or it is not your link." << std::endl;
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
    int nr = r + dr, nc = c + dc;
    if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) {
        // Allow moving off the opponent's side (download)
        // Conditions:
        // - Player 1 moves upwards out of row 0
        // - Player 2 moves downwards out of row 7
        bool offOpponentSide =
            (player->getId() == 1 && nr >= 8) ||
            (player->getId() == 2 && nr < 0);

        if (offOpponentSide) {
            outcome.movedLink = grid[r][c].getLink();
            outcome.affectedLink = grid[r][c].getLink();
            outcome.affectedLink->reveal();
            grid[r][c].removeLink(); // Remove from board
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

    Link* moving = src.getLink();
    if (!moving) {
        outcome.success = false;
        outcome.result = MoveResult::Invalid;
        return outcome;
    }

    outcome.movedLink = moving;
    outcome.destRow = nr;
    outcome.destCol = nc;

    if (!dest.isEmpty()) {
        if (dest.getLink()->getOwner() == player) {
            outcome.success = false;
            outcome.result = MoveResult::Invalid;
            return outcome;
        }

        Link* defender = dest.getLink();
        Link* winner = moving->battle(defender);

        if (winner == moving) {
            // Moving link wins battle, replaces defender
            dest.setLink(moving);
            src.removeLink();

            outcome.success = true;
            outcome.result = MoveResult::BattleWon;
            outcome.affectedLink = defender;

            // std::cerr << "MoveResult = " << static_cast<int>(outcome.result)
        //   << " affectedLink=" << (outcome.affectedLink ? outcome.affectedLink->getId() : '?')
        //   << std::endl;
        //   std::cout << "typeid(outcome.result).name() = " << typeid(outcome.result).name() << std::endl;
            return outcome;
        } else {
            // Defender wins, attacker is removed
            src.removeLink();

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
            outcome.success = true;
            outcome.result = MoveResult::DownloadedOnServerPort;
            return outcome;
        }

        // Empty destination cell
        dest.setLink(moving);
        src.removeLink();

        outcome.success = true;
        outcome.result = MoveResult::Moved;
        return outcome;
    }
}

void Board::printBoard() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c].getLink()) {
                std::cout << grid[r][c].getLink()->getId();
            } else if (grid[r][c].getType() == CellType::ServerPort) {
                std::cout << 'S';
            } else if (grid[r][c].getType() == CellType::Firewall) {
                std::cout << (grid[r][c].getOwnerId() == 0 ? 'm' : 'w');
            } else {
                std::cout << '.';
            }
        }
        std::cout << "\n";
    }
}

bool Board::isServerPort(int row, int col) const {
    return (row == 0 || row == 7) && (col == 3 || col == 4);
}

void Board::addFirewall(int row, int col, Player* player) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        throw std::invalid_argument("Firewall position out of bounds");
    }
    
    // Set the cell type to Firewall and store the player ID
    grid[row][col] = Cell(CellType::Firewall, player->getId());
}
