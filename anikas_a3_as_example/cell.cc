#include "cell.h"
#include "game.h"
#include <unordered_set>
using namespace std;

Cell::Cell() : state{0}, prevState{0}, numNeighbours{0}, r{0}, c{0}, game{nullptr} {}

int Cell::getState() {
    return state;
}

void Cell::setState(int change) {
    prevState = state;
    state = change;
}

void Cell::setCoords(int row, int col) {
    r = row;
    c = col;
}

void Cell::setGame(Game *g) {
    game = g;
}

void Cell::addNeighbour(Cell *neighbour) {
    if (numNeighbours < maxNeighbours) {
        neighbours[numNeighbours++] = neighbour; // add a neighbour if under max neighbours
    }
}

void Cell::notifyGame() {
    if (game && prevState != state) { // if game exists and change made to state, notify the game
        game->notify(r, c, prevState, state);
    }
}

// shared visited set to avoid revisiting during flood fill
static unordered_set<Cell*> visited;

void Cell::notify(int change) {
    visited.clear();

    // change current state of cell
    prevState = state;
    state = change;

    // notify everyone
    notifyNeighbours();
    notifyGame();
}

void Cell::notify(int current, int previous) {
    if (visited.count(this)) return;  // if already visited cell, return
    if (state != previous) return;    // if no state change, return
    visited.insert(this);

    // change current state of cell
    prevState = state;
    state = current;

    // notify everyone
    notifyNeighbours();
    notifyGame();
}

void Cell::notifyNeighbours() {
    for (int i = 0; i < numNeighbours; ++i) {
        neighbours[i]->notify(state, prevState);
    }
}
