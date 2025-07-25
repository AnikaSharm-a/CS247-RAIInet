#include "controller.h"
#include "board.h"
#include "player.h"
#include <sstream>

Controller::Controller(View* view, Game* game)
    : view(view), game(game) {}

void Controller::play(std::istream &in) {
    std::string cmd;
    std::cout << "Player " << (game->getCurrentPlayerIdx() + 1) << "'s turn.\n";
    while (in >> cmd) {
        if (cmd == "quit") break;
        if (cmd == "board") {
            game->getBoard()->printBoard();
        } else if (cmd == "move") {
            char id; std::string dirStr;
            in >> id >> dirStr;
            Direction dir;
            if (dirStr == "up") dir = Direction::Up;
            else if (dirStr == "down") dir = Direction::Down;
            else if (dirStr == "left") dir = Direction::Left;
            else dir = Direction::Right;

            auto &players = game->getPlayers();
            Player* p = players[game->getCurrentPlayerIdx()];
            bool success = game->getBoard()->moveLink(id, p, dir);
            if (!success) {
                std::cout << "Invalid move\n";
            } else {
                game->getBoard()->printBoard();
                // Switch to the other player's turn
                int nextIdx = (game->getCurrentPlayerIdx() + 1) % game->getPlayers().size();
                game->setCurrentPlayerIdx(nextIdx);
                std::cout << "Player " << (game->getCurrentPlayerIdx() + 1) << "'s turn.\n";
            }
        }
    }
}

void Controller::parseCommand(std::istream &in) {
    play(in);
}
