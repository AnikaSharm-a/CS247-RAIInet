#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include "board.h"
#include "player.h"
using namespace std;

class Controller;

class Game {
    Board board;
    vector<unique_ptr<Player>> players;
    int currentPlayerIdx;
    bool gameOver;
    Controller* controller;
    int turnNumber; // track the current turn number
    bool notificationsEnabled; //flag to control notifications during initialization

    // Map cell coordinates to pair of cellType, and a vector of fog effects (appliedTurn, ownerId)
    map<pair<int, int>, pair<CellType, vector<pair<int, int>>>> foggedCells;

public:
    void startGame();
    bool checkVictory();
    void download(shared_ptr<Link> link, Player* targetPlayer);
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
    const map<pair<int, int>, pair<CellType, vector<pair<int, int>>>>& getFoggedCells() const;
    void setTurnNumber(int t);

    // player management
    void addPlayer(unique_ptr<Player> player);

    bool playerMove(char linkId, Direction dir);
    Player* getOpponentPlayer();

    // setters
    void setCurrentPlayerIdx(int idx);
    void setGameOver(bool over);
    void setController(Controller* c);

    void applyFogEffect(int row, int col, int ownerId);
    void removeFogEffect();
    void updateFog();
    
    // player setup methods
    void generateDefaultLinks(Player* player, bool isPlayer1);
    void setupPlayers(Player* p1, Player* p2, const string& ability1Str, const string& ability2Str, const string& link1File, const string& link2File);
};

#endif
