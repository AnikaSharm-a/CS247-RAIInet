#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>

class View;
class Game;
class Player;
class Ability;
enum class LinkType;

class Controller {
    View* view;
    Game* game;

    // Returns false if quit or game over
    bool parseCommand(const std::string &cmd, std::istream &in, Player* currentPlayer, bool &moved, bool &abilityUsed);
    static LinkType parseLinkType(const std::string& s);
    static int parseStrength(const std::string& s);
    void loadLinksFromFile(const std::string& filename, Player* player, bool isPlayer1);
    void generateDefaultLinks(Player* player, bool isPlayer1);

public:
    Controller(View* view, Game* game);
    void play(std::istream &in);
    void setupPlayers(Player* p1, Player* p2, 
                      const std::string& ability1Str, const std::string& ability2Str,
                      const std::string& link1File, const std::string& link2File);
};

#endif
