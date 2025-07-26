#pragma once
#include "cell.h"
#include "link.h"
#include <vector>

// Move result enums and struct
enum class MoveResult {
    Invalid,
    Moved,
    BattleWon,
    BattleLost,
    DownloadedOffBoard,
    DownloadedOnServerPort
};

struct MoveOutcome {
    bool success = false;
    MoveResult result = MoveResult::Invalid;
    Link* movedLink = nullptr;     // The link that moved
    Link* affectedLink = nullptr;  // The link that was captured/downloaded (if any)
    int destRow = -1;
    int destCol = -1;
};

enum class Direction { Up, Down, Left, Right };

class Board {
    Cell grid[8][8];

public:
    Board();

    Cell& at(int row, int col) { return grid[row][col]; }
    MoveOutcome moveLink(char id, Player* player, Direction dir);
    std::pair<int, int> findLinkPosition(char id, Player* player);
    void printBoard();
    
    // Methods needed for abilities
    bool isServerPort(int row, int col) const;
    void addFirewall(int row, int col, Player* player);
};
