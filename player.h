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

class Link;      // forward declare

class Player {
    int id;
    map<char, unique_ptr<Link>> links;
    vector<unique_ptr<Ability>> abilities;
    set<int> usedAbilities;
    int downloadedVirus = 0;
    int downloadedData = 0;

public:
    Player(int id);

    // Getters
    int getId() const;
    const map<char, unique_ptr<Link>>& getLinks() const;
    set<int>& getUsedAbilities();
    int getDownloadedVirus() const;
    int getDownloadedData() const;
    int getNumLinks() const;
    int getStrengthSum() const;

    // Link management
    void addLink(unique_ptr<Link> link);
    std::shared_ptr<Link> getLink(char id) const;
    void removeLink(char id);
    bool ownsLink(char id) const;
    vector<std::shared_ptr<Link>> getAllLinks() const;

    // Reveal
    void revealLink(char id);
    bool isLinkRevealed(char id) const;

    // Abilities
    bool canUseAbility(int abilityID) const;
    int getNumUnusedAbilities() const;

    // Download counters
    void incrementDownload(Link* link);
    void resetDownloads();

    // Game status
    bool hasWon() const;
    bool hasLost() const;
    bool hasLostOrWon() const;
    
    // Ability management
    void addAbility(unique_ptr<Ability> ability);
    Ability* getAbility(int abilityId);
    bool isAbilityUsed(int abilityId) const;
    void markAbilityUsed(int abilityId);
    int getNumAbilities() const;
    
    // Enhanced ability management
    void displayAbilities() const;
    bool hasUnusedAbilities() const;
    int getUnusedAbilityCount() const;
    vector<string> getAbilityNames() const;
    void resetAbilities(); // For testing or new games
};

#endif // PLAYER_H
