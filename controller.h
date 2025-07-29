#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>
using namespace std;

class View;
class Game;
class Player;
class Ability;
enum class LinkType;

class Controller {
    View* view;
    Game* game;

    // Returns false if quit or game over
    bool parseCommand(const string &cmd, istream &in, Player* currentPlayer, bool &moved, bool &abilityUsed);
    static LinkType parseLinkType(const string& s);
    static int parseStrength(const string& s);
    void loadLinksFromFile(const string& filename, Player* player, bool isPlayer1);
    void generateDefaultLinks(Player* player, bool isPlayer1);

public:
    Controller(View* view, Game* game);
    void play(istream &in);
    void setupPlayers(Player* p1, Player* p2, 
                      const string& ability1Str, const string& ability2Str,
                      const string& link1File, const string& link2File);
};

#endif
