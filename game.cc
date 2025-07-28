#include "game.h"
#include "controller.h"
#include "ability.h"
#include <iostream>
#include <string>
#include <stdexcept>

// Accessors and mutators
Board* Game::getBoard() { return &board; }
const Board* Game::getBoard() const { return &board; }
std::vector<Player*>& Game::getPlayers() { return players; }
const std::vector<Player*>& Game::getPlayers() const { return players; }
int Game::getCurrentPlayerIdx() const { return currentPlayerIdx; }
bool Game::isGameOver() const { return gameOver; }
Controller* Game::getController() { return controller; }

void Game::setCurrentPlayerIdx(int idx) { currentPlayerIdx = idx; }
void Game::setGameOver(bool over) { gameOver = over; }
void Game::setController(Controller* c) { controller = c; }

void Game::setupLinksForPlayer(Player* p, bool isPlayer1) {
    if (isPlayer1) {
        board.at(0,0).setLink(p->getLink('a'));
        board.at(0,1).setLink(p->getLink('b'));
        board.at(0,2).setLink(p->getLink('c'));
        board.at(0,5).setLink(p->getLink('f'));
        board.at(0,6).setLink(p->getLink('g'));
        board.at(0,7).setLink(p->getLink('h'));
        board.at(1,3).setLink(p->getLink('d'));
        board.at(1,4).setLink(p->getLink('e'));
    } else {
        board.at(7,0).setLink(p->getLink('A'));
        board.at(7,1).setLink(p->getLink('B'));
        board.at(7,2).setLink(p->getLink('C'));
        board.at(7,5).setLink(p->getLink('F'));
        board.at(7,6).setLink(p->getLink('G'));
        board.at(7,7).setLink(p->getLink('H'));
        board.at(6,3).setLink(p->getLink('D'));
        board.at(6,4).setLink(p->getLink('E'));
    }
}

void Game::startGame() {
    currentPlayerIdx = 0;
    gameOver = false;

    // Only do setup if no links have been placed yet
    if (players.size() < 2) return;
    // Place links already created for each player on the board
    setupLinksForPlayer(players[0], true);
    setupLinksForPlayer(players[1], false);
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
    // std::cout << "Board::moveLink returning " << static_cast<int>(outcome.result) << std::endl;

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
            // Remove boost effect from the downloaded link
            if (outcome.affectedLink && outcome.affectedLink->isBoosted()) {
                outcome.affectedLink->removeBoost();
            }
            currentPlayer->incrementDownload(outcome.affectedLink);
            break;
        case MoveResult::BattleLost:
            // Opponent downloads current player's link
            // Remove boost effect from the downloaded link
            if (outcome.affectedLink && outcome.affectedLink->isBoosted()) {
                outcome.affectedLink->removeBoost();
            }
            opponent->incrementDownload(outcome.affectedLink);
            break;
        case MoveResult::DownloadedOffBoard:
            // Current player downloads their own link escaped off board
            // Remove boost effect from the downloaded link
            if (outcome.movedLink && outcome.movedLink->isBoosted()) {
                outcome.movedLink->removeBoost();
            }
            currentPlayer->incrementDownload(outcome.movedLink);
            break;
        case MoveResult::DownloadedOnServerPort:
            // Opponent downloads current player's link that moved onto their server port
            // Remove boost effect from the downloaded link
            if (outcome.movedLink && outcome.movedLink->isBoosted()) {
                outcome.movedLink->removeBoost();
            }
            opponent->incrementDownload(outcome.movedLink);
            break;
        case MoveResult::DownloadedByFirewall:
            // Virus link was downloaded by firewall effect
            // Remove boost effect from the downloaded link
            if (outcome.affectedLink && outcome.affectedLink->isBoosted()) {
                outcome.affectedLink->removeBoost();
            }
            // The virus owner downloads their own virus
            if (outcome.affectedLink) {
                Player* virusOwner = outcome.affectedLink->getOwner();
                if (virusOwner) {
                    virusOwner->incrementDownload(outcome.affectedLink);
                }
            }
            break;
        case MoveResult::Invalid:
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
    } else if (abilityName == "Download" || abilityName == "Polarize" || abilityName == "Scan") {
        if (args[0] == '\0') {
            throw std::invalid_argument(abilityName + " requires a link ID");
        }
        char linkId = args[0];
        // Find the link on the board (can be any player's link)
        bool found = false;
        for (int r = 0; r < 8 && !found; ++r) {
            for (int c = 0; c < 8 && !found; ++c) {
                auto* link = board.at(r, c).getLink();
                if (link && link->getId() == linkId) {
                    row = r;
                    col = c;
                    found = true;
                }
            }
        }
        if (!found) {
            throw std::invalid_argument("Link '" + std::string(1, linkId) + "' not found on the board");
        }
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
