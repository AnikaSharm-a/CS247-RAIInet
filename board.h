#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
#include "link.h"
#include <vector>
#include <memory>
using namespace std;

// Forward declaration
class Controller;

// Move result enums and struct
enum class MoveResult {
    Invalid,
    Moved,
    BattleWon,
    BattleLost,
    DownloadedOffBoard,
    DownloadedOnServerPort,
    DownloadedByFirewall,
    Jammed
};

struct MoveOutcome {
    bool success = false;
    MoveResult result = MoveResult::Invalid;
    std::shared_ptr<Link> movedLink = nullptr;     // The link that moved
    std::shared_ptr<Link> affectedLink = nullptr;  // The link that was captured/downloaded (if any)
    int destRow = -1;
    int destCol = -1;
};

enum class Direction { Up, Down, Left, Right };

class Board {
    Cell grid[8][8];
    Controller* controller;  // Reference to controller for notifications

public:
    Board();

    Cell& at(int row, int col) { return grid[row][col]; }
    const Cell& at(int row, int col) const { return grid[row][col]; }
    MoveOutcome moveLink(char id, Player* player, Direction dir);
    pair<int, int> findLinkPosition(char id, Player* player);
    
    // Methods needed for abilities
    bool isServerPort(int row, int col) const;
    void addFirewall(int row, int col, Player* player);
    
    // Setter for controller reference
    void setController(Controller* c) { controller = c; }
};

#endif // BOARD_H
