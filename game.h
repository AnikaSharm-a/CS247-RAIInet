#ifndef GAME_H
#define GAME_H

#include <vector>
#include "board.h"
#include "player.h"
// #include "controller.h"

class Controller;  // forward declare

class Game {
    Board board;
    std::vector<Player*> players;
    int currentPlayerIdx;
    bool gameOver;
    Controller* controller;

public:
    void startGame();
    bool checkVictory();
    void download(Link* link, Player* targetPlayer);
    void useAbility(Player* player, int abilityId, char args[]);

    Board* getBoard();
    std::vector<Player*>& getPlayers();
    int getCurrentPlayerIdx() const;
    bool isGameOver() const;
    Controller* getController();

    bool playerMove(char linkId, Direction dir);
    Player* getOpponentPlayer();

    // Setters
    void setCurrentPlayerIdx(int idx);
    void setGameOver(bool over);
    void setController(Controller* c);
};

#endif // GAME_H
