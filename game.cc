#include "game.h"
#include "controller.h"
#include "ability.h"
#include "abilityFactory.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <fstream> // Added for file loading
#include <random> // Added for random functionality
using namespace std;

// Accessors and mutators
Board* Game::getBoard() { return &board; }
const Board* Game::getBoard() const { return &board; }

vector<Player*>& Game::getPlayers() { 
    static vector<Player*> playerPtrs;
    playerPtrs.clear();
    for (auto& player : players) {
        playerPtrs.push_back(player.get());
    }
    return playerPtrs;
}

const vector<Player*>& Game::getPlayers() const { 
    static vector<Player*> playerPtrs;
    playerPtrs.clear();
    for (const auto& player : players) {
        playerPtrs.push_back(player.get());
    }
    return playerPtrs;
}

int Game::getCurrentPlayerIdx() const { return currentPlayerIdx; }
bool Game::isGameOver() const { return gameOver; }
Controller* Game::getController() { return controller; }
int Game::getCurrentTurn() const { return turnNumber; }
void Game::setTurnNumber(int t) { 
    turnNumber = t; 
    if (controller && notificationsEnabled) {
        controller->notifyGameStateChanged();
    }
}

void Game::setCurrentPlayerIdx(int idx) { 
    currentPlayerIdx = idx; 
    if (controller && notificationsEnabled) {
        controller->notifyGameStateChanged();
    }
}
void Game::setGameOver(bool over) { 
    gameOver = over; 
    if (over && controller && notificationsEnabled) {
        controller->notifyGameStateChanged();
    }
}
void Game::setController(Controller* c) { 
    controller = c; 
    notificationsEnabled = true; // Enable notifications once controller is set
}
const map<pair<int, int>, pair<CellType, vector<pair<int, int>>>>& Game::getFoggedCells() const { return foggedCells; }

void Game::addPlayer(unique_ptr<Player> player) {
    players.push_back(move(player));
}

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
    
    // Don't notify during setup - we'll notify once at the end of startGame
}

void Game::startGame() {
    // Temporarily disable notifications during initialization
    bool wasEnabled = notificationsEnabled;
    notificationsEnabled = false;
    
    currentPlayerIdx = 0;
    gameOver = false;
    turnNumber = 0; // Initialize turn number

    // Only do setup if no links have been placed yet
    if (players.size() < 2) return;
    // Place links already created for each player on the board
    setupLinksForPlayer(players[0].get(), true);
    setupLinksForPlayer(players[1].get(), false);
    
    // Re-enable notifications and send one notification after all setup is complete
    notificationsEnabled = wasEnabled;
    if (controller) {
        controller->notifyGameStateChanged();
    }
}

bool Game::checkVictory() {
    for (auto& p : players) {
        if (p->hasWon()) {
            cout << "Player " << p->getId() << " wins by downloading 4 data!\n";
            gameOver = true;
            if (controller) {
                controller->notifyGameStateChanged();
            }
            return true;
        }
        if (p->hasLost()) {
            // Find the opponent (assuming 2 players)
            int loserId = p->getId();
            int winnerId = (loserId == 1) ? 2 : 1;
            cout << "Player " << loserId << " loses by downloading 4 viruses!\n";
            cout << "Player " << winnerId << " wins!\n";
            gameOver = true;
            if (controller) {
                controller->notifyGameStateChanged();
            }
            return true;
        }
    }
    return false;
}

// Returns the opposing player (assuming 2 players)
Player* Game::getOpponentPlayer() {
    return players[(currentPlayerIdx + 1) % players.size()].get();
}

void Game::download(std::shared_ptr<Link> link, Player* targetPlayer) {
    targetPlayer->incrementDownload(link.get());
    
    // Notify about the download
    if (link && controller) {
        controller->notifyLinkDownloaded(link->getId());
        controller->notifyPlayerChanged(targetPlayer->getId());
    }
    
    checkVictory();
}

// Handles the move command, coordinating board and player state
bool Game::playerMove(char id, Direction dir) {
    Player* currentPlayer = players[currentPlayerIdx].get();
    Player* opponent = getOpponentPlayer();

    // Board::moveLink should return a MoveOutcome struct describing what happened.
    // For now, assume this function exists as below and returns true if move succeeded.
    // You will need to implement it accordingly.
    auto outcome = board.moveLink(id, currentPlayer, dir);
    // cout << "Board::moveLink returning " << static_cast<int>(outcome.result) << endl;

    if (!outcome.success) {
        if (outcome.result == MoveResult::Jammed) {
            cout << "Link '" << id << "' is jammed.\n";
        } else {
            cout << "Invalid move.\n";
        }
        return false;
    }

    // Notify about the link movement
    // if (controller) {
    //     controller->notifyLinkMoved(id);
    // }
    
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
            currentPlayer->incrementDownload(outcome.affectedLink.get());
            break;
        case MoveResult::BattleLost:
            // Opponent downloads current player's link
            // Remove boost effect from the downloaded link
            if (outcome.affectedLink && outcome.affectedLink->isBoosted()) {
                outcome.affectedLink->removeBoost();
            }
            opponent->incrementDownload(outcome.affectedLink.get());
            break;
        case MoveResult::DownloadedOffBoard:
            // Current player downloads their own link escaped off board
            // Remove boost effect from the downloaded link
            if (outcome.movedLink && outcome.movedLink->isBoosted()) {
                outcome.movedLink->removeBoost();
            }
            currentPlayer->incrementDownload(outcome.movedLink.get());
            break;
        case MoveResult::DownloadedOnServerPort:
            // Opponent downloads current player's link that moved onto their server port
            // Remove boost effect from the downloaded link
            if (outcome.movedLink && outcome.movedLink->isBoosted()) {
                outcome.movedLink->removeBoost();
            }
            opponent->incrementDownload(outcome.movedLink.get());
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
                    virusOwner->incrementDownload(outcome.affectedLink.get());
                }
            }
            break;
        case MoveResult::Invalid:
            break;
    }

    if (controller) {
        controller->notifyLinkMoved(id);
    }

    checkVictory();

    // Unjam links that have been jammed for 2+ turns
    for (auto* p : getPlayers()) {
        for (auto& entry : p->getLinks()) {
            auto link = entry.second;
            if (link->isJammed() && link->getJammedOnTurn() <= turnNumber - 2) {
                link->unjam();
            }
        }
    }

    // Update fog effects
    updateFog();

    // Increment turn number
    setTurnNumber(turnNumber + 1);

    return true;
}

void Game::useAbility(Player* player, int abilityId, char args[]) {
    if (abilityId < 1 || abilityId > player->getNumAbilities()) {
        throw invalid_argument("Invalid ability ID");
    }

    if (player->isAbilityUsed(abilityId)) {
        throw invalid_argument("Ability has already been used");
    }

    Ability* ability = player->getAbility(abilityId);
    if (!ability) {
        throw invalid_argument("Ability not found");
    }

    string abilityName = ability->getName();
    int row = -1, col = -1;

    if (abilityName == "LinkBoost") {
        if (args[0] == '\0') {
            throw invalid_argument("LinkBoost requires a link ID");
        }
        char linkId = args[0];
        auto pos = board.findLinkPosition(linkId, player);
        if (pos.first == -1) {
            throw invalid_argument("Link not found or not owned by player");
        }
        row = pos.first;
        col = pos.second;
    } else if (abilityName == "AreaScan") {
        if (args[0] == '\0') {
            throw invalid_argument("AreaScan requires a link ID");
        }
        char linkId = args[0];
        auto pos = board.findLinkPosition(linkId, player);
        if (pos.first == -1) {
            throw invalid_argument("Link not found or not owned by player");
        }
        row = pos.first;
        col = pos.second;
    } else if (abilityName == "Download" || abilityName == "Polarize" || abilityName == "Scan" || abilityName == "Jam") {
        if (args[0] == '\0') {
            throw invalid_argument(abilityName + " requires a link ID");
        }
        char linkId = args[0];
        // Find the link on the board (can be any player's link)
        bool found = false;
        for (int r = 0; r < 8 && !found; ++r) {
            for (int c = 0; c < 8 && !found; ++c) {
                auto link = board.at(r, c).getLink();
                if (link && link->getId() == linkId) {
                    row = r;
                    col = c;
                    found = true;
                }
            }
        }
        if (!found) {
            throw invalid_argument("Link '" + string(1, linkId) + "' not found on the board");
        }
    } else {
        string argsStr(args);
        size_t spacePos = argsStr.find(' ');
        if (spacePos == string::npos) {
            spacePos = argsStr.find(',');
        }

        if (spacePos == string::npos) {
            throw invalid_argument("Invalid coordinates format. Use 'row col' or 'row,col'");
        }

        try {
            row = stoi(argsStr.substr(0, spacePos));
            col = stoi(argsStr.substr(spacePos + 1));
        } catch (const exception&) {
            throw invalid_argument("Invalid coordinates");
        }

        if (row < 0 || row >= 8 || col < 0 || col >= 8) {
            throw invalid_argument("Coordinates out of bounds");
        }
    }

    try {
        ability->use(this, player, row, col);
        player->markAbilityUsed(abilityId);
        
        // Notify about ability usage and player state change
        if (controller) {
            controller->notifyPlayerChanged(player->getId());
            cout << "Notifying cell changed " << row << " " << col << endl;
            controller->notifyCellChanged(row, col);
        }
        
        cout << "Used " << abilityName << " successfully.\n";
    } catch (const exception& e) {
        throw invalid_argument(e.what());
    }
}

void Game::applyFogEffect(int row, int col, int ownerId) {
    Board* board = getBoard();
    int startRow = max(0, row - 1);
    int endRow = min(7, row + 1);
    int startCol = max(0, col - 1);
    int endCol = min(7, col + 1);

    for (int r = startRow; r <= endRow; ++r) {
        for (int c = startCol; c <= endCol; ++c) {
            Cell& cell = board->at(r, c);
            auto coord = make_pair(r, c);

            if (foggedCells.find(coord) == foggedCells.end()) {
                // First fog on this cell - store original type
                CellType originalType = cell.getType();
                foggedCells[coord] = make_pair(originalType, vector<pair<int, int>>{{turnNumber, ownerId}});
                cell.setType(CellType::Fog);
            } else {
                // Cell already fogged - add new fog to the list
                foggedCells[coord].second.push_back(make_pair(turnNumber, ownerId));
                cell.setType(CellType::Fog);
            }
            
            // Notify about cell change
            if (controller) {
                controller->notifyCellChanged(r, c);
            }
        }
    }
    
    // Notify about board change
    if (controller) {
        controller->notifyBoardChanged();
    }
}

void Game::removeFogEffect() {
    Board* board = getBoard();

    for (auto& entry : foggedCells) {
        int r = entry.first.first;
        int c = entry.first.second;
        
        // Get the original cell type
        CellType originalType = entry.second.first;
        board->at(r, c).setType(originalType);
        
        // Notify about cell change
        if (controller) {
            controller->notifyCellChanged(r, c);
        }
    }

    foggedCells.clear();

    // Notify about board change
    if (controller) {
        controller->notifyBoardChanged();
    }

    cout << "Fog effect removed and cells restored.\n";
}

void Game::updateFog() {
    Board* board = getBoard();
    vector<pair<int, int>> cellsToErase;

    // Check each fogged cell
    for (const auto& entry : foggedCells) {
        const auto& coord = entry.first;
        const auto& fogEntry = entry.second;
        const auto& fogList = fogEntry.second;
        
        // Remove expired fogs (older than 5 turns)
        auto& mutableFogList = foggedCells[coord].second;
        mutableFogList.erase(
            remove_if(mutableFogList.begin(), mutableFogList.end(),
                [this](const auto& fog) {
                    int appliedTurn = fog.first;
                    return (turnNumber - appliedTurn >= 5);
                }),
            mutableFogList.end()
        );

        // If no fogs remain, mark for restoration
        if (mutableFogList.empty()) {
            CellType originalType = fogEntry.first;
            board->at(coord.first, coord.second).setType(originalType);
            cellsToErase.push_back(coord);
            
            // Notify about cell change
            if (controller) {
                controller->notifyCellChanged(coord.first, coord.second);
            }
        }
    }
    
    // Erase empty fog entries after iteration is complete
    for (const auto& coord : cellsToErase) {
        foggedCells.erase(coord);
    }
    
    // Notify about board change if any cells were updated
    if (!cellsToErase.empty() && controller) {
        controller->notifyBoardChanged();
    }
}

void Game::setupPlayers(Player* p1, Player* p2,
                        const string& ability1Str, const string& ability2Str,
                        const string& link1File, const string& link2File) {
    auto p1Abilities = AbilityFactory::createAbilities(ability1Str);
    for (auto& ability : p1Abilities) p1->addAbility(move(ability));

    auto p2Abilities = AbilityFactory::createAbilities(ability2Str);
    for (auto& ability : p2Abilities) p2->addAbility(move(ability));

    if (!link1File.empty()) {
        controller->loadLinksFromFile(link1File, p1, true);
    } else {
        generateDefaultLinks(p1, true);
    }

    if (!link2File.empty()) {
        controller->loadLinksFromFile(link2File, p2, false);
    } else {
        generateDefaultLinks(p2, false);
    }
}

void Game::generateDefaultLinks(Player* player, bool isPlayer1) {
    vector<pair<LinkType,int>> configs = {
        {LinkType::Virus, 1}, {LinkType::Data, 4},
        {LinkType::Virus, 3}, {LinkType::Virus, 2},
        {LinkType::Data, 3}, {LinkType::Virus, 4},
        {LinkType::Data, 2}, {LinkType::Data, 1}
    };

    static random_device rd;
    static mt19937 g(rd());
    shuffle(configs.begin(), configs.end(), g);

    char id = isPlayer1 ? 'a' : 'A';
    for (const auto& cfg : configs) {
        auto link = make_unique<Link>(id, cfg.first, cfg.second, player);
        player->addLink(move(link));
        ++id;
    }
}
