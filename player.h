#pragma once
#include <map>
#include <vector>
#include <set>
#include "linkType.h"

class Link;      // forward declare
class Ability;   // forward declare

class Player {
    int id;
    std::map<char, Link*> links;
    std::vector<Ability*> abilities;
    std::set<int> usedAbilities;
    int downloadedVirus = 0;
    int downloadedData = 0;

public:
    Player(int id);

    int getId() const { return id; }
    std::map<char, Link*>& getLinks() { return links; }
    void addLink(Link* link);
    bool hasWon() const;
    bool hasLost() const;
    void incrementDownload(Link* link);
};
