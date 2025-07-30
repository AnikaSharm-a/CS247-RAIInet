#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
#include "link.h"
#include <vector>
#include <memory>
using namespace std;

// forward declaration to avoid circular dependency
class Controller;

// possible results of a move operation
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

// structure to hold the outcome of a move operation
struct MoveOutcome {
    bool success = false;
    MoveResult result = MoveResult::Invalid;
    shared_ptr<Link> movedLink = nullptr;     // the link that moved
    shared_ptr<Link> affectedLink = nullptr;  // the link that was captured/downloaded (if any)
    int destRow = -1;
    int destCol = -1;
};

// possible movement directions
enum class Direction { Up, Down, Left, Right };

// represents the game board as an 8x8 grid of cells
class Board {
    Cell grid[8][8]; // 8x8 grid of cells
    Controller* controller;  // reference to controller for notifications

public:
    Board();

    // accessors for board cells
    Cell& at(int row, int col) { return grid[row][col]; }
    const Cell& at(int row, int col) const { return grid[row][col]; }
    
    // attempts to move a link in the specified direction
    MoveOutcome moveLink(char id, Player* player, Direction dir);
    
    // finds the position of a link on the board
    pair<int, int> findLinkPosition(char id, Player* player);
    
    // methods needed for abilities
    bool isServerPort(int row, int col) const;
    void addFirewall(int row, int col, Player* player);
    
    // setter for controller reference
    void setController(Controller* c) { controller = c; }
};

#endif
