#ifndef GAME_H
#define GAME_H

#include <vector>
#include "board.h"
#include "player.h"
#include "controller.h"

class Game {
    Board board;
    std::vector<Player*> players;
    int currentPlayerIdx;
    bool gameOver;
    Controller* controller;

public:
    void startGame();
    void checkVictory();
    void resolveBattle(Link* attacker, Link* defender);
    void download(Link* link, Player* targetPlayer);
    void useAbility(Player* player, int abilityId, char args[]);

    Board* getBoard();
    std::vector<Player*>& getPlayers();
    int getCurrentPlayerIdx() const;
    bool isGameOver() const;
    Controller* getController();

    // Setters
    void setCurrentPlayerIdx(int idx);
    void setGameOver(bool over);
    void setController(Controller* c);
};

#endif // GAME_H
