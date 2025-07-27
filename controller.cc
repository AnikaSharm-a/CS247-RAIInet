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
