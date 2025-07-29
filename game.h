#ifndef GAME_H
#define GAME_H

#include <vector>
#include "board.h"
#include "player.h"
// #include "controller.h"
using namespace std;

class Controller;  // forward declare

class Game {
    Board board;
    vector<Player*> players;
    int currentPlayerIdx;
    bool gameOver;
    Controller* controller;
    int turnNumber; // Track the current turn number
    // Map from cell coordinates to pair of (originalType, vector of fog effects (appliedTurn, ownerId))
    std::map<std::pair<int, int>, std::pair<CellType, std::vector<std::pair<int, int>>>> foggedCells;

public:
    void startGame();
    bool checkVictory();
    void download(Link* link, Player* targetPlayer);
    void useAbility(Player* player, int abilityId, char args[]);

    Board* getBoard();
    const Board* getBoard() const;
    vector<Player*>& getPlayers();
    const vector<Player*>& getPlayers() const; 
    int getCurrentPlayerIdx() const;
    bool isGameOver() const;
    Controller* getController();
    void setupLinksForPlayer(Player* p, bool isPlayer1);
    int getCurrentTurn() const; // Get current turn number
    const std::map<std::pair<int, int>, std::pair<CellType, std::vector<std::pair<int, int>>>>& getFoggedCells() const;
    void setTurnNumber(int t);


    bool playerMove(char linkId, Direction dir);
    Player* getOpponentPlayer();

    // Setters
    void setCurrentPlayerIdx(int idx);
    void setGameOver(bool over);
    void setController(Controller* c);

    void applyFogEffect(int row, int col, int ownerId);
    void removeFogEffect();
    void updateFog();
};

#endif // GAME_H
