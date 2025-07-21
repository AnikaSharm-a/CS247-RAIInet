#include "game.h"
#include "cell.h"
#include <vector>
using namespace std;

Game::Game(): gridSize{0}, notification{nullptr} {
    for (unsigned int &c : colours) c = 0; // set every cell to white
}

void Game::init(int n, GameNotification *gameNotification) {
    // reset all components
    theGrid.clear();
    theGrid.resize(n, vector<Cell>(n));
    gridSize = n;
    notification = gameNotification;
    for (unsigned int &c : colours) c = 0;

    // setup each cell
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            theGrid[r][c].setCoords(r, c);
            theGrid[r][c].setGame(this);
            colours[0]++;
        }
    }

    // link neighbours
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (r > 0) theGrid[r][c].addNeighbour(&theGrid[r - 1][c]);
            if (r < n - 1) theGrid[r][c].addNeighbour(&theGrid[r + 1][c]);
            if (c > 0) theGrid[r][c].addNeighbour(&theGrid[r][c - 1]);
            if (c < n - 1) theGrid[r][c].addNeighbour(&theGrid[r][c + 1]);
        }
    }
}

void Game::init(int r, int c, int change) {
    if (r < 0 || r >= gridSize || c < 0 || c >= gridSize) return; 

    int oldState = theGrid[r][c].getState();
    
    if (oldState != change) { // difference in colours
        // manage colours array
        colours[oldState]--;
        colours[change]++;
        
        // set new state and notify
        theGrid[r][c].setState(change);
        if (notification) notification->notify(r, c, change);
    }
}

void Game::notify(int r, int c, int oldState, int newState) {
    if (oldState != newState) { 
        // manage colour arrays and notify
        colours[oldState]--;
        colours[newState]++;
        if (notification) notification->notify(r, c, newState);
    }
}

bool Game::isWon() {
    int total = gridSize * gridSize;

    // if one of the colours fills the entire board (num of that colour = num cells), the game is won
    for (int i = 0; i < 5; ++i) {
        if (colours[i] == total) return true; 
    }
    return false;
}

void Game::change(int c) {
    if (gridSize == 0) return;

    if (theGrid[0][0].getState() != c) { // if changing colour, notify
        theGrid[0][0].notify(c);
    }
}
