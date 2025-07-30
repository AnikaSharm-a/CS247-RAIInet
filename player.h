#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <vector>
#include <set>
#include <string>
#include <memory>
#include "linkType.h"
#include "ability.h"
using namespace std;

class Link; // forward declare

class Player {
    int id;
    map<char, unique_ptr<Link>> links;
    vector<unique_ptr<Ability>> abilities;
    set<int> usedAbilities;
    int downloadedVirus = 0;
    int downloadedData = 0;

public:
    Player(int id);

    //getters
    int getId() const;
    const map<char, unique_ptr<Link>>& getLinks() const;
    set<int>& getUsedAbilities();
    int getDownloadedVirus() const;
    int getDownloadedData() const;
    int getNumLinks() const;
    int getStrengthSum() const;

    //link management
    void addLink(unique_ptr<Link> link);
    std::shared_ptr<Link> getLink(char id) const;
    void removeLink(char id);
    bool ownsLink(char id) const;
    vector<std::shared_ptr<Link>> getAllLinks() const;

    //reveal links
    void revealLink(char id);
    bool isLinkRevealed(char id) const;

    //abilities pt 1
    bool canUseAbility(int abilityID) const;
    int getNumUnusedAbilities() const;

    //download
    void incrementDownload(Link* link);
    void resetDownloads();

    //game status
    bool hasWon() const;
    bool hasLost() const;
    bool hasLostOrWon() const;
    
    //ability management
    void addAbility(unique_ptr<Ability> ability);
    Ability* getAbility(int abilityId);
    bool isAbilityUsed(int abilityId) const;
    void markAbilityUsed(int abilityId);
    int getNumAbilities() const;
    
    void displayAbilities() const;
    bool hasUnusedAbilities() const;
    int getUnusedAbilityCount() const;
    vector<string> getAbilityNames() const;
    void resetAbilities();
};

#endif
