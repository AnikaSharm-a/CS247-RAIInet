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
    vector<View*> observers;

    // returns false if quit or game over
    bool parseCommand(const string &cmd, istream &in, Player* currentPlayer, bool &moved, bool &abilityUsed);
    static LinkType parseLinkType(const string& s);
    static int parseStrength(const string& s);

public:
    Controller(unique_ptr<View> view, unique_ptr<Game> game);
    void play(istream &in);
    void loadLinksFromFile(const string& filename, Player* player, bool isPlayer1);
    
    void attachObserver(View* observer);
    void detachObserver(View* observer);
    void notifyObservers(const NotificationData& data);
    
    // notification methods for different event types
    void notifyCellChanged(int row, int col);
    void notifyPlayerChanged(int playerId);
    void notifyGameStateChanged();
    void notifyBoardChanged();
    void notifyLinkMoved(char linkId);
    void notifyLinkRevealed(char linkId);
    void notifyLinkDownloaded(char linkId);
    
    Game* getGame() { return game.get(); }
    View* getView() { return view.get(); }
};

#endif
