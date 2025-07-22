#pragma once
#include "cell.h"
#include "link.h"
#include <vector>

enum class Direction { Up, Down, Left, Right };

class Board {
    Cell grid[8][8];

public:
    Board();

    Cell& at(int row, int col) { return grid[row][col]; }
    bool moveLink(char id, Player* player, Direction dir);
    std::pair<int, int> findLinkPosition(char id, Player* player);
};
