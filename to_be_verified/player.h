#pragma once
#include <map>
#include <vector>
#include <set>
#include "link.h"
#include "ability.h"

class Player {
    int id;
    std::map<char, Link*> links;
    std::vector<Ability*> abilities;
    std::set<int> usedAbilities;
    int downloadedVirus = 0;
    int downloadedData = 0;

public:
    Player(int id) : id(id) {}

    int getId() const { return id; }
    std::map<char, Link*>& getLinks() { return links; }
    void addLink(Link* link) { links[link->getId()] = link; }

    bool hasWon() const { return downloadedData >= 4; }
    bool hasLost() const { return downloadedVirus >= 4; }

    void incrementDownload(Link* link) {
        if (link->getType() == LinkType::Data) downloadedData++;
        else downloadedVirus++;
    }
};
