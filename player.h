#pragma once
#include <map>
#include <vector>
#include <set>
#include <string>
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

    // Getters
    int getId() const;
    std::map<char, Link*>& getLinks();
    std::set<int>& getUsedAbilities();
    int getDownloadedVirus() const;
    int getDownloadedData() const;
    int getNumLinks() const;
    int getStrengthSum() const;

    // Link management
    void addLink(Link* link);
    Link* getLink(char id) const;
    void removeLink(char id);
    bool ownsLink(char id) const;
    std::vector<Link*> getAllLinks() const;

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
    void addAbility(Ability* ability);
    Ability* getAbility(int abilityId);
    bool isAbilityUsed(int abilityId) const;
    void markAbilityUsed(int abilityId);
    int getNumAbilities() const;
    
    // Enhanced ability management
    void displayAbilities() const;
    bool hasUnusedAbilities() const;
    int getUnusedAbilityCount() const;
    std::vector<std::string> getAbilityNames() const;
    void resetAbilities(); // For testing or new games
};
