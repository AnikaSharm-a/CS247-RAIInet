#include "board.h"
#include <stdexcept>

Board::Board() {
    // All cells default to Normal unless setup otherwise
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            CellType type = CellType::Normal;
            if ((r == 0 || r == 7) && (c == 3 || c == 4)) type = CellType::ServerPort;
            grid[r][c] = Cell(type);
        }
    }
}

std::pair<int, int> Board::findLinkPosition(char id, Player* player) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            auto* link = grid[r][c].getLink();
            if (link && link->getId() == id && link->getOwner() == player)
                return {r, c};
        }
    }
    throw std::runtime_error("Link not found on board");
}

bool Board::moveLink(char id, Player* player, Direction dir) {
    auto [r, c] = findLinkPosition(id, player);
    int dr = 0, dc = 0;
    switch (dir) {
        case Direction::Up: dr = -1; break;
        case Direction::Down: dr = 1; break;
        case Direction::Left: dc = -1; break;
        case Direction::Right: dc = 1; break;
    }
    int nr = r + dr, nc = c + dc;
    if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) return false;

    Cell& src = grid[r][c];
    Cell& dest = grid[nr][nc];

    Link* moving = src.getLink();
    if (!moving) return false;

    if (!dest.isEmpty()) {
        if (dest.getLink()->getOwner() == player) return false;

        Link* defender = dest.getLink();
        Link* winner = moving->battle(defender);
        if (winner == moving) {
            dest.setLink(moving);
            player->incrementDownload(defender);
        } else {
            src.removeLink();
            defender->getOwner()->incrementDownload(moving);
            return true;
        }
    } else {
        dest.setLink(moving);
    }

    src.removeLink();
    return true;
}
