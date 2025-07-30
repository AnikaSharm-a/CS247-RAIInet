#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "view.h"
using namespace std;

class View;
class Game;
class Player;
class Ability;
enum class LinkType;

class Controller {
    unique_ptr<View> view;
    unique_ptr<Game> game;
    vector<View*> observers;  // List of observers to notify

    // Returns false if quit or game over
    bool parseCommand(const string &cmd, istream &in, Player* currentPlayer, bool &moved, bool &abilityUsed);
    static LinkType parseLinkType(const string& s);
    static int parseStrength(const string& s);
    void loadLinksFromFile(const string& filename, Player* player, bool isPlayer1);
    void generateDefaultLinks(Player* player, bool isPlayer1);

public:
    Controller(unique_ptr<View> view, unique_ptr<Game> game);
    void play(istream &in);
    void setupPlayers(Player* p1, Player* p2, 
                      const string& ability1Str, const string& ability2Str,
                      const string& link1File, const string& link2File);
    
    // Observer management
    void attachObserver(View* observer);
    void detachObserver(View* observer);
    void notifyObservers(const NotificationData& data);
    
    // Convenience notification methods
    void notifyCellChanged(int row, int col);
    void notifyPlayerChanged(int playerId);
    void notifyGameStateChanged();
    void notifyBoardChanged();
    void notifyLinkMoved(char linkId);
    void notifyLinkRevealed(char linkId);
    void notifyLinkDownloaded(char linkId);
    
    // Getter methods to access raw pointers (non-owning)
    Game* getGame() { return game.get(); }
    View* getView() { return view.get(); }
};

#endif