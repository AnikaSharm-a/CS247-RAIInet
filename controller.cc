#include "controller.h"
#include "board.h"
#include "game.h"
#include "player.h"
#include "view.h"
#include "graphicdisplay.h"
#include "textdisplay.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <random>
#include "abilityFactory.h"
#include "linkType.h"

using namespace std;

Controller::Controller(unique_ptr<View> view, unique_ptr<Game> game)
    : view(move(view)), game(move(game)) {
    
    // Register the main view as an observer
    attachObserver(this->view.get());
    
    // Set up controller references in Game and Board
    this->game->setController(this);
    this->game->getBoard()->setController(this);
    
    // Set game reference in views
    if (GraphicDisplay* gd = dynamic_cast<GraphicDisplay*>(this->view.get())) {
        gd->setGameRef(this->game.get());
    } else if (TextDisplay* td = dynamic_cast<TextDisplay*>(this->view.get())) {
        td->setGameRef(this->game.get());
    }
}

// Observer management methods
void Controller::attachObserver(View* observer) {
    observers.push_back(observer);
}

void Controller::detachObserver(View* observer) {
    auto it = find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it);
    }
}

void Controller::notifyObservers(const NotificationData& data) {
    for (auto observer : observers) {
        observer->notify(data);
    }
}

// Convenience notification methods
void Controller::notifyCellChanged(int row, int col) {
    notifyObservers(NotificationData(NotificationType::CellChanged, row, col));
}

void Controller::notifyPlayerChanged(int playerId) {
    notifyObservers(NotificationData(NotificationType::PlayerChanged, playerId));
}

void Controller::notifyGameStateChanged() {
    notifyObservers(NotificationData(NotificationType::GameStateChanged));
}

void Controller::notifyBoardChanged() {
    notifyObservers(NotificationData(NotificationType::BoardChanged));
}

void Controller::notifyLinkMoved(char linkId) {
    notifyObservers(NotificationData(NotificationType::LinkMoved, linkId));
}

void Controller::notifyLinkRevealed(char linkId) {
    notifyObservers(NotificationData(NotificationType::LinkRevealed, linkId));
}

void Controller::notifyLinkDownloaded(char linkId) {
    notifyObservers(NotificationData(NotificationType::LinkDownloaded, linkId));
}

bool Controller::parseCommand(const string& cmd, istream& in, Player* currentPlayer, bool &moved, bool &abilityUsed) {
    if (cmd == "quit") {
        return false;
    }

    else if (cmd == "board") {
        // With observer pattern, we don't need to explicitly call print
        // The view will be notified of changes automatically
        // But we can still call print for immediate display
        view->print(*game, cout);
    }

    else if (cmd == "abilities") {
        cout << "~~~~~~~~~~~~~" << endl;
        currentPlayer->displayAbilities();
        cout << "~~~~~~~~~~~~~" << endl;
    }

    else if (cmd == "ability") {
        if (abilityUsed) {
            cout << "You have already used an ability this turn." << endl;
            return true;
        }

        int abilityId;
        in >> abilityId;

        string argsLine;
        getline(in, argsLine);
        size_t start = argsLine.find_first_not_of(" \t");
        if (start != string::npos) {
            argsLine = argsLine.substr(start);
        } else {
            argsLine.clear();
        }

        char args[64] = {0};
        strncpy(args, argsLine.c_str(), sizeof(args) - 1);

        try {
            game->useAbility(currentPlayer, abilityId, args);
            abilityUsed = true;
            // Observer pattern will handle the display update automatically
        } catch (const exception& e) {
            cout << "Ability failed: " << e.what() << endl;
        }
    }

    else if (cmd == "move") {
        char id; string dirStr;
        in >> id >> dirStr;

        Direction dir;
        if      (dirStr == "up") dir = Direction::Up;
        else if (dirStr == "down") dir = Direction::Down;
        else if (dirStr == "left") dir = Direction::Left;
        else if (dirStr == "right") dir = Direction::Right;
        else {
            cout << "Invalid direction" << endl;
            return true;
        }

        bool success = game->playerMove(id, dir);
        if (!success) {
            cout << "Invalid move" << endl;
        } else {
            moved = true;
        }
    }

    else if (cmd == "sequence") {
        string filename;
        in >> filename;
        ifstream fileIn(filename);
        if (!fileIn) {
            cout << "Could not open sequence file\n";
        } else {
            bool cont = true;
            while (cont) {
                string nextCmd;
                if (!(fileIn >> nextCmd)) break;  // No more commands in file
                cont = parseCommand(nextCmd, fileIn, currentPlayer, moved, abilityUsed);
                if (!cont) return false;  // Propagate quit upwards
            }
        }
        return true;
    }

    else {
        cout << "Unknown command" << endl;
    }

    return true;
}

void Controller::play(istream &in) {
    // Loop until the game ends
    while (!game->checkVictory() && in) {
        Player* currentPlayer = game->getPlayers()[game->getCurrentPlayerIdx()];
        cout << "Player " << (game->getCurrentPlayerIdx() + 1) << "'s turn:"<<endl;

        bool abilityUsed = false;
        bool moved = false;

        // Loop until the current player makes a valid move (turn ends after that)
        while (!moved && in) {
            string cmd;
            if (!(in >> cmd)) break; // end of file or stream

            if (!parseCommand(cmd, in, currentPlayer, moved, abilityUsed)) {
                return; // quit or EOF
            }
        }

        if (game->checkVictory()) break;

        for (auto* p : game->getPlayers()) {
            for (auto& entry : p->getLinks()) {
                Link* link = entry.second;
                if (link->isJammed() && link->getJammedOnTurn() <= game->getCurrentTurn() - 2) {
                    link->unjam();
                }
            }
        }

        game->setCurrentPlayerIdx((game->getCurrentPlayerIdx() + 1) % game->getPlayers().size());
        game->updateFog();
        game->setTurnNumber(game->getCurrentTurn() + 1);
        // Observer pattern will handle the display update automatically
    }
}

LinkType Controller::parseLinkType(const string& s) {
    if (s.empty()) throw invalid_argument("Empty link type.");
    if (s[0] == 'D') return LinkType::Data;
    if (s[0] == 'V') return LinkType::Virus;
    throw invalid_argument("Invalid link type: " + s);
}

int Controller::parseStrength(const string& s) {
    if (s.length() < 2) throw invalid_argument("Link string too short.");
    int strength = s[1] - '0';
    if (strength < 1 || strength > 4) throw invalid_argument("Invalid strength: " + s);
    return strength;
}

void Controller::loadLinksFromFile(const string& filename, Player* player, bool isPlayer1) {
    ifstream in(filename);
    if (!in) throw runtime_error("Failed to open link file: " + filename);

    vector<string> tokens;
    string s;
    while (in >> s) {
        tokens.push_back(s);
    }

    if (tokens.size() != 8) {
        throw runtime_error("Expected 8 links in file: " + filename);
    }

    for (int i = 0; i < 8; ++i) {
        LinkType type = parseLinkType(tokens[i]);
        int strength = parseStrength(tokens[i]);
        char id = isPlayer1 ? ('a' + i) : ('A' + i);
        auto link = make_unique<Link>(id, type, strength, player);
        player->addLink(move(link));
    }
}

void Controller::generateDefaultLinks(Player* player, bool isPlayer1) {
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

void Controller::setupPlayers(Player* p1, Player* p2,
                              const string& ability1Str, const string& ability2Str,
                              const string& link1File, const string& link2File) {
    auto p1Abilities = AbilityFactory::createAbilities(ability1Str);
    for (auto& ability : p1Abilities) p1->addAbility(move(ability));

    auto p2Abilities = AbilityFactory::createAbilities(ability2Str);
    for (auto& ability : p2Abilities) p2->addAbility(move(ability));

    if (!link1File.empty()) {
        loadLinksFromFile(link1File, p1, true);
    } else {
        generateDefaultLinks(p1, true);
    }

    if (!link2File.empty()) {
        loadLinksFromFile(link2File, p2, false);
    } else {
        generateDefaultLinks(p2, false);
    }
}

