#include "game.h"
#include "controller.h"
#include "ability.h"
#include <iostream>
#include <string>
#include <stdexcept>

// Accessors and mutators
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

bool Game::checkVictory() {
    for (auto p : players) {
        if (p->hasWon()) {
            std::cout << "Player " << p->getId() << " wins by downloading 4 data!\n";
            gameOver = true;
            return true;
        }
        if (p->hasLost()) {
            // Find the opponent (assuming 2 players)
            int loserId = p->getId();
            int winnerId = (loserId == 1) ? 2 : 1;
            std::cout << "Player " << loserId << " loses by downloading 4 viruses!\n";
            std::cout << "Player " << winnerId << " wins!\n";
            gameOver = true;
            return true;
        }
    }
    return false;
}

// Returns the opposing player (assuming 2 players)
Player* Game::getOpponentPlayer() {
    return players[(currentPlayerIdx + 1) % players.size()];
}

void Game::download(Link* link, Player* targetPlayer) {
    targetPlayer->incrementDownload(link);
    checkVictory();
}

// Handles the move command, coordinating board and player state
bool Game::playerMove(char id, Direction dir) {
    Player* currentPlayer = players[currentPlayerIdx];
    Player* opponent = getOpponentPlayer();

    // Board::moveLink should return a MoveOutcome struct describing what happened.
    // For now, assume this function exists as below and returns true if move succeeded.
    // You will need to implement it accordingly.
    auto outcome = board.moveLink(id, currentPlayer, dir);

    if (!outcome.success) {
        std::cout << "Invalid move.\n";
        return false;
    }

    // Handle downloads or battles based on outcome:
    switch (outcome.result) {
        case MoveResult::Moved:
            // Normal move, no download
            break;
        case MoveResult::BattleWon:
            // Current player downloads opponent's link
            currentPlayer->incrementDownload(outcome.affectedLink);
            break;
        case MoveResult::BattleLost:
            // Opponent downloads current player's link
            opponent->incrementDownload(outcome.affectedLink);
            break;
        case MoveResult::DownloadedOffBoard:
            // Current player downloads their own link escaped off board
            currentPlayer->incrementDownload(outcome.movedLink);
            break;
        case MoveResult::DownloadedOnServerPort:
            // Opponent downloads current player's link that moved onto their server port
            opponent->incrementDownload(outcome.movedLink);
            break;
    }

    checkVictory();

    // Advance turn if game is not over
    if (!gameOver) {
        currentPlayerIdx = (currentPlayerIdx + 1) % players.size();
    }

    return true;
}

void Game::useAbility(Player* player, int abilityId, char args[]) {
    if (abilityId < 1 || abilityId > player->getNumAbilities()) {
        throw std::invalid_argument("Invalid ability ID");
    }

    if (player->isAbilityUsed(abilityId)) {
        throw std::invalid_argument("Ability has already been used");
    }

    Ability* ability = player->getAbility(abilityId);
    if (!ability) {
        throw std::invalid_argument("Ability not found");
    }

    std::string abilityName = ability->getName();
    int row = -1, col = -1;

    if (abilityName == "LinkBoost") {
        if (args[0] == '\0') {
            throw std::invalid_argument("LinkBoost requires a link ID");
        }
        char linkId = args[0];
        auto pos = board.findLinkPosition(linkId, player);
        if (pos.first == -1) {
            throw std::invalid_argument("Link not found or not owned by player");
        }
        row = pos.first;
        col = pos.second;
    } else {
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

        if (row < 0 || row >= 8 || col < 0 || col >= 8) {
            throw std::invalid_argument("Coordinates out of bounds");
        }
    }

    try {
        ability->use(this, player, row, col);
        player->markAbilityUsed(abilityId);
        std::cout << "Used " << abilityName << " successfully.\n";
    } catch (const std::exception& e) {
        throw std::invalid_argument(std::string("Ability failed: ") + e.what());
    }
}
