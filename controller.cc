#include "controller.h"
#include "board.h"
#include "game.h"
#include "player.h"
#include "view.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <random>
#include "abilityFactory.h"
#include "linkType.h"


Controller::Controller(View* view, Game* game)
    : view(view), game(game) {}

void Controller::play(std::istream &in) {
    // Loop until the game ends
    while (!game->checkVictory() && in) {
        Player* currentPlayer = game->getPlayers()[game->getCurrentPlayerIdx()];
        std::cout << "Player " << (game->getCurrentPlayerIdx() + 1) << "'s turn.\n";

        bool abilityUsed = false;
        bool moved = false;

        // Loop until the current player makes a valid move (turn ends after that)
        while (!moved && in) {
            std::string cmd;
            if (!(in >> cmd)) break; // end of file or stream

            if (cmd == "quit") return;

            if (cmd == "board") {
                // Show the board from the current player's perspective
                view->print(*game, std::cout);
            }

            else if (cmd == "abilities") {
                std::cout << "begin displaying abilities\n";
                currentPlayer->displayAbilities();
                std::cout << "end displaying abilities\n";
            }

            else if (cmd == "ability") {
                if (abilityUsed) {
                    std::cout << "You have already used an ability this turn.\n";
                    continue;
                }
                int abilityId;
                in >> abilityId;

                // Parse extra arguments (could be a link ID or coords)
                std::string argsLine;
                std::getline(in, argsLine);
                size_t start = argsLine.find_first_not_of(" \t");
                if (start != std::string::npos) {
                    argsLine = argsLine.substr(start);
                } else {
                    argsLine.clear();
                }

                char args[64] = {0};
                strncpy(args, argsLine.c_str(), sizeof(args) - 1);

                try {
                    game->useAbility(currentPlayer, abilityId, args);
                    abilityUsed = true;
                    view->print(*game, std::cout);
                } catch (const std::exception& e) {
                    std::cout << "Ability failed: " << e.what() << "\n";
                }
            }

            else if (cmd == "move") {
                char id; std::string dirStr;
                in >> id >> dirStr;

                Direction dir;
                if      (dirStr == "up") dir = Direction::Up;
                else if (dirStr == "down") dir = Direction::Down;
                else if (dirStr == "left") dir = Direction::Left;
                else if (dirStr == "right") dir = Direction::Right;
                else {
                    std::cout << "Invalid direction\n";
                    continue;
                }

                // bool success = game->getBoard()->moveLink(id, currentPlayer, dir).success;
                bool success = game->playerMove(id, dir);
                if (!success) {
                    std::cout << "Invalid move\n";
                } else {
                    view->print(*game, std::cout);
                    moved = true; // turn ends after a successful move
                }
            }

            else if (cmd == "sequence") {
                std::string filename;
                in >> filename;
                std::ifstream fileIn(filename);
                if (!fileIn) {
                    std::cout << "Could not open sequence file\n";
                } else {
                    play(fileIn); // process commands from file recursively
                }
            }

            else {
                std::cout << "Unknown command\n";
            }
        }

        // Check for game end after a move
        if (game->checkVictory()) break;

        // Switch to next player
        // game->setCurrentPlayerIdx((game->getCurrentPlayerIdx() + 1) % game->getPlayers().size());
    }
}

LinkType Controller::parseLinkType(const std::string& s) {
    if (s.empty()) throw std::invalid_argument("Empty link type.");
    if (s[0] == 'D') return LinkType::Data;
    if (s[0] == 'V') return LinkType::Virus;
    throw std::invalid_argument("Invalid link type: " + s);
}

int Controller::parseStrength(const std::string& s) {
    if (s.length() < 2) throw std::invalid_argument("Link string too short.");
    int strength = s[1] - '0';
    if (strength < 1 || strength > 4) throw std::invalid_argument("Invalid strength: " + s);
    return strength;
}

void Controller::loadLinksFromFile(const std::string& filename, Player* player, bool isPlayer1) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Failed to open link file: " + filename);

    std::vector<std::string> tokens;
    std::string s;
    while (in >> s) {
        tokens.push_back(s);
    }

    if (tokens.size() != 8) {
        throw std::runtime_error("Expected 8 links in file: " + filename);
    }

    for (int i = 0; i < 8; ++i) {
        LinkType type = parseLinkType(tokens[i]);
        int strength = parseStrength(tokens[i]);
        char id = isPlayer1 ? ('a' + i) : ('A' + i);
        Link* link = new Link(id, type, strength, player);
        player->addLink(link);
    }
}

void Controller::generateDefaultLinks(Player* player, bool isPlayer1) {
    std::vector<std::pair<LinkType,int>> configs = {
        {LinkType::Virus, 1}, {LinkType::Data, 4},
        {LinkType::Virus, 3}, {LinkType::Virus, 2},
        {LinkType::Data, 3}, {LinkType::Virus, 4},
        {LinkType::Data, 2}, {LinkType::Data, 1}
    };

    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(configs.begin(), configs.end(), g);

    char id = isPlayer1 ? 'a' : 'A';
    for (const auto& cfg : configs) {
        Link* link = new Link(id, cfg.first, cfg.second, player);
        player->addLink(link);
        ++id;
    }
}

void Controller::setupPlayers(Player* p1, Player* p2,
                              const std::string& ability1Str, const std::string& ability2Str,
                              const std::string& link1File, const std::string& link2File) {
    auto p1Abilities = AbilityFactory::createAbilities(ability1Str);
    for (Ability* ability : p1Abilities) p1->addAbility(ability);

    auto p2Abilities = AbilityFactory::createAbilities(ability2Str);
    for (Ability* ability : p2Abilities) p2->addAbility(ability);

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

