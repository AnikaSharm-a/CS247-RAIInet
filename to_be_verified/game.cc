#include "game.h"
#include "controller.h"
#include <iostream>

Board* Game::getBoard() { return &board; }
std::vector<Player*>& Game::getPlayers() { return players; }
int Game::getCurrentPlayerIdx() const { return currentPlayerIdx; }
bool Game::isGameOver() const { return gameOver; }
Controller* Game::getController() { return controller; }

void Game::setCurrentPlayerIdx(int idx) { currentPlayerIdx = idx; }
void Game::setGameOver(bool over) { gameOver = over; }
void Game::setController(Controller* c) { controller = c; }

void Game::startGame() {
    currentPlayerIdx = 0;
    gameOver = false;
}

void Game::checkVictory() {
    for (auto p : players) {
        if (p->hasWon()) {
            std::cout << "Player " << p->getId() << " wins by downloading 4 data!\n";
            gameOver = true;
        }
        if (p->hasLost()) {
            std::cout << "Player " << p->getId() << " loses by downloading 4 viruses!\n";
            gameOver = true;
        }
    }
}

void Game::resolveBattle(Link* attacker, Link* defender) {
    Link* winner = attacker->battle(defender);
    if (winner == attacker) {
        Player* loser = defender->getOwner();
        attacker->getOwner()->incrementDownload(defender);
        // The board already moved attacker into position
    } else {
        Player* loser = attacker->getOwner();
        defender->getOwner()->incrementDownload(attacker);
        // Attacker is removed in Board
    }
    checkVictory();
}

void Game::download(Link* link, Player* targetPlayer) {
    targetPlayer->incrementDownload(link);
    checkVictory();
}

void Game::useAbility(Player* player, int abilityId, char args[]) {
    // Stub for now
    std::cout << "Ability system not implemented yet.\n";
}
