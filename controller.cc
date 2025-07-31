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
#include "linkType.h"

using namespace std;

Controller::Controller(unique_ptr<View> view, unique_ptr<Game> game) : view(move(view)), game(move(game)) {
    
    // attach the main view as an observer
    attachObserver(this->view.get());
    
    // set up controller references in Game and Board
    this->game->setController(this);
    this->game->getBoard()->setController(this);
    
    // set game reference in views
    if (GraphicDisplay* gd = dynamic_cast<GraphicDisplay*>(this->view.get())) {
        gd->setGameRef(this->game.get());
    } else if (TextDisplay* td = dynamic_cast<TextDisplay*>(this->view.get())) {
        td->setGameRef(this->game.get());
    }
}

// observer management methods
void Controller::attachObserver(View* observer) {
    observers.push_back(observer);
}

void Controller::detachObserver(View* observer) {
    auto it = find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it);
    }
}

// notifies all observers of a change
void Controller::notifyObservers(const NotificationData& data) {
    for (auto observer : observers) {
        observer->notify(data);
    }
}

// convenience notification methods for different types of events
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

// parses and executes a single command, returns false if quit or game over
bool Controller::parseCommand(const string& cmd, istream& in, Player* currentPlayer, bool &moved, bool &abilityUsed) {
    if (cmd == "quit") {
        return false;
    }

    else if (cmd == "board") {
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
            // observer pattern should handle display update automatically
        } catch (const exception& e) {
            cout << "Ability failed: " << e.what() << endl;
        }
    }

    else if (cmd == "move") {
        char id; string dirStr;
        in >> id >> dirStr;

        Direction dir;
        if (dirStr == "up") dir = Direction::Up;
        else if (dirStr == "down") dir = Direction::Down;
        else if (dirStr == "left") dir = Direction::Left;
        else if (dirStr == "right") dir = Direction::Right;
        else {
            cout << "Invalid direction" << endl;
            return true;
        }

        bool success = game->playerMove(id, dir);
        if (success) {
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
                fileIn >> nextCmd;
                if (fileIn.eof()) {
                    return false; // Treat EOF as quit
                }
                if (!fileIn) {
                    cout << "Error reading sequence file\n";
                    break;
                }
                cont = parseCommand(nextCmd, fileIn, currentPlayer, moved, abilityUsed);
                if (!cont) return false; // quit or game over from inside sequence
            }
        }
        return true;
    }

    else {
        cout << "Unknown command" << endl;
    }

    return true;
}

// main game loop that handles turn management and player input
void Controller::play(istream &in) {
    // loop until the game ends
    while (!game->checkVictory() && in) {
        Player* currentPlayer = game->getPlayers()[game->getCurrentPlayerIdx()];
        cout << "Player " << (game->getCurrentPlayerIdx() + 1) << "'s turn:"<<endl;
        
        // print the board at the start of each turn
        view->print(*game, cout);

        bool abilityUsed = false;
        bool moved = false;

        // loop until the current player makes a valid move (turn ends after that)
        while (!moved && in) {
            string cmd;
            if (!(in >> cmd)) break; // end of file or stream

            if (!parseCommand(cmd, in, currentPlayer, moved, abilityUsed)) {
                return; // quit or EOF
            }
        }

        if (game->checkVictory()) break;

        // REMOVE THIS LINE - Game now handles turn switching
        // game->setCurrentPlayerIdx((game->getCurrentPlayerIdx() + 1) % game->getPlayers().size());
    }
}

// parses a string to determine the link type (Data or Virus)
LinkType Controller::parseLinkType(const string& s) {
    if (s.empty()) throw invalid_argument("Empty link type.");
    if (s[0] == 'D') return LinkType::Data;
    if (s[0] == 'V') return LinkType::Virus;
    throw invalid_argument("Invalid link type: " + s);
}

// parses a string to extract the strength value (1-4)
int Controller::parseStrength(const string& s) {
    if (s.length() < 2) throw invalid_argument("Internal Error:Link string too short.");
    int strength = s[1] - '0';
    if (strength < 1 || strength > 4) throw invalid_argument("Internal Error: Invalid strength: " + s);
    return strength;
}

// loads link configurations from a file for a player
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

    for (int i = 0; i < 8; i++) {
        LinkType type = parseLinkType(tokens[i]);
        int strength = parseStrength(tokens[i]);
        char id = isPlayer1 ? ('a' + i) : ('A' + i);
        auto link = make_unique<Link>(id, type, strength, player);
        player->addLink(move(link));
    }
}
