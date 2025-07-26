#include "game.h"
#include "controller.h"
#include "ability.h"
#include <iostream>
#include <string>
#include <stdexcept>

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
    // Validate ability ID
    if (abilityId < 1 || abilityId > player->getNumAbilities()) {
        throw std::invalid_argument("Invalid ability ID");
    }
    
    // Check if ability has already been used
    if (player->isAbilityUsed(abilityId)) {
        throw std::invalid_argument("Ability has already been used");
    }
    
    // Get the ability
    Ability* ability = player->getAbility(abilityId);
    if (!ability) {
        throw std::invalid_argument("Ability not found");
    }
    
    // Parse arguments based on ability type
    std::string abilityName = ability->getName();
    int row = -1, col = -1;
    
    if (abilityName == "LinkBoost") {
        // LinkBoost expects a link ID (e.g., 'a', 'b', etc.)
        if (args[0] == '\0') {
            throw std::invalid_argument("LinkBoost requires a link ID");
        }
        char linkId = args[0];
        
        // Find the link on the board
        auto pos = board.findLinkPosition(linkId, player);
        if (pos.first == -1) {
            throw std::invalid_argument("Link not found or not owned by player");
        }
        row = pos.first;
        col = pos.second;
    } else {
        // Other abilities expect row and column coordinates
        // Parse row and col from args (assuming format like "3 4" or "3,4")
        std::string argsStr(args);
        size_t spacePos = argsStr.find(' ');
        if (spacePos == std::string::npos) {
            spacePos = argsStr.find(',');
        }
        
        if (spacePos == std::string::npos) {
            throw std::invalid_argument("Invalid coordinates format. Use 'row col' or 'row,col'");
        }
        
        try {
            row = std::stoi(argsStr.substr(0, spacePos));
            col = std::stoi(argsStr.substr(spacePos + 1));
        } catch (const std::exception&) {
            throw std::invalid_argument("Invalid coordinates");
        }
        
        // Validate coordinates
        if (row < 0 || row >= 8 || col < 0 || col >= 8) {
            throw std::invalid_argument("Coordinates out of bounds");
        }
    }
    
    // Use the ability
    try {
        ability->use(this, player, row, col);
        player->markAbilityUsed(abilityId);
        std::cout << "Used " << abilityName << " successfully.\n";
    } catch (const std::exception& e) {
        throw std::invalid_argument(std::string("Ability failed: ") + e.what());
    }
}
