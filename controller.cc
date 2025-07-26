#include "controller.h"
#include "board.h"
#include "game.h"
#include "player.h"
#include "view.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>

Controller::Controller(View* view, Game* game)
    : view(view), game(game) {}

void Controller::play(std::istream &in) {
    std::string cmd;
    std::cout << "Player " << (game->getCurrentPlayerIdx() + 1) << "'s turn.\n";

    while (in >> cmd) {
        if (cmd == "quit") break;
        if (!parseCommand(cmd, in)) break;

        // Check if the game is over
        if (game->checkVictory()) break;

        std::cout << "Player " << (game->getCurrentPlayerIdx() + 1) << "'s turn.\n";
    }
}

bool Controller::parseCommand(const std::string &cmd, std::istream &in) {
    if (cmd == "board") {
        game->getBoard()->printBoard();       // print text board to console
        view->print(std::cout);               // update view output, e.g., GUI or text display
    }

    else if (cmd == "move") {
        char id; std::string dirStr;
        in >> id >> dirStr;

        Direction dir;
        if (dirStr == "up") dir = Direction::Up;
        else if (dirStr == "down") dir = Direction::Down;
        else if (dirStr == "left") dir = Direction::Left;
        else if (dirStr == "right") dir = Direction::Right;
        else {
            std::cout << "Invalid direction\n";
            return true;
        }

        Player* p = game->getPlayers()[game->getCurrentPlayerIdx()];
        bool success = game->getBoard()->moveLink(id, p, dir).success;

        if (!success) {
            std::cout << "Invalid move\n";
        } else {
            game->getBoard()->printBoard();
            view->print(std::cout);            // refresh the view display
            // Switch turn
            int nextIdx = (game->getCurrentPlayerIdx() + 1) % game->getPlayers().size();
            game->setCurrentPlayerIdx(nextIdx);
        }
    }


    else if (cmd == "abilities") {
        game->getPlayers()[game->getCurrentPlayerIdx()]->displayAbilities();
    }

    else if (cmd == "ability") {
        int abilityId;
        in >> abilityId;

        // Read the rest of the line as arguments (could be link ID or coordinates)
        std::string argsLine;
        std::getline(in, argsLine);
        // Trim leading spaces if any
        size_t start = argsLine.find_first_not_of(" \t");
        if (start != std::string::npos) {
            argsLine = argsLine.substr(start);
        } else {
            argsLine.clear(); // no arguments
        }

        Player* p = game->getPlayers()[game->getCurrentPlayerIdx()];
        try {
            // game->useAbility expects char array, so convert string:
            char args[64] = {0};
            strncpy(args, argsLine.c_str(), sizeof(args) - 1);

            game->useAbility(p, abilityId, args);
        } catch (const std::exception& e) {
            std::cout << "Ability failed or invalid: " << e.what() << "\n";
        }
    }


    else if (cmd == "sequence") {
        std::string filename;
        in >> filename;
        std::ifstream fileIn(filename);
        if (!fileIn) {
            std::cout << "Could not open sequence file\n";
        } else {
            play(fileIn); // Recursively play commands from file
        }
    }

    else {
        std::cout << "Unknown command\n";
    }

    return true;
}
